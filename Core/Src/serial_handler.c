#include "serial_handler.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;

#define RX_BUFFER_SIZE 256

static uint8_t rx_byte;
static char rx_buffer[RX_BUFFER_SIZE];
static uint16_t rx_index = 0;
static SerialPredictCallback predict_callback = NULL;
static volatile uint8_t data_ready = 0;

void Serial_Handler_Init(void)
{
    rx_index = 0;
    memset(rx_buffer, 0, sizeof(rx_buffer));
    data_ready = 0;
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void Serial_Handler_SetCallback(SerialPredictCallback callback)
{
    predict_callback = callback;
}

static int ParseFloats(const char* str, float* values, int max_count)
{
    int count = 0;
    const char* ptr = str;
    char* end;
    
    while (count < max_count && *ptr != '\0') {
        while (*ptr == ' ' || *ptr == ',' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n') {
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        values[count] = strtof(ptr, &end);
        
        if (end == ptr) break;
        
        count++;
        ptr = end;
    }
    
    return count;
}

static void ProcessLine(void)
{
    IrisInput input;
    float values[4];
    int count;
    char buffer[128];
    
    if (rx_buffer[0] == '\0') return;
    
    count = ParseFloats(rx_buffer, values, 4);
    
    if (count == 4) {
        input.sepal_length = values[0];
        input.sepal_width = values[1];
        input.petal_length = values[2];
        input.petal_width = values[3];
        
        snprintf(buffer, sizeof(buffer), 
            "\r\nInput: (%.2f, %.2f, %.2f, %.2f)\r\n",
            values[0], values[1], values[2], values[3]);
        Serial_Handler_SendString(buffer);
        
        if (predict_callback != NULL) {
            predict_callback(&input, SERIAL_CMD_SINGLE_PREDICT);
        } else {
            Serial_Handler_SendString("ERROR: No callback!\r\n");
        }
    } else {
        snprintf(buffer, sizeof(buffer), 
            "\r\nError: Got %d values, need 4\r\n"
            "Format: sepal_len sepal_wid petal_len petal_wid\r\n"
            "Example: 5.1 3.5 1.4 0.2\r\n", count);
        Serial_Handler_SendString(buffer);
    }
}

void Serial_Handler_Process(void)
{
    if (rx_byte == '\r' || rx_byte == '\n') {
        if (rx_index > 0) {
            rx_buffer[rx_index] = '\0';
            data_ready = 1;
        }
    } else if (rx_index < RX_BUFFER_SIZE - 1) {
        rx_buffer[rx_index++] = (char)rx_byte;
    } else {
        rx_index = 0;
        memset(rx_buffer, 0, sizeof(rx_buffer));
    }
    
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void Serial_Handler_Loop(void)
{
    if (data_ready) {
        data_ready = 0;
        ProcessLine();
        rx_index = 0;
        memset(rx_buffer, 0, sizeof(rx_buffer));
    }
}

void Serial_Handler_SendResult(const IrisResult* result, const IrisInput* input)
{
    char buffer[128];
    (void)input;
    
    snprintf(buffer, sizeof(buffer), "\r\nResult: %s (%.1f%%)\r\n",
             AI_Inference_GetClassName(result->predicted_class),
             result->confidence);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
}

void Serial_Handler_SendString(const char* str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
}

void Serial_Handler_SendInfo(void)
{
    char info[256];
    
    snprintf(info, sizeof(info), 
        "\r\n=== Iris Classification Model ===\r\n"
        "Input: 4 features\r\n"
        "Output: 3 classes\r\n"
        "=================================\r\n");
    
    HAL_UART_Transmit(&huart1, (uint8_t*)info, strlen(info), 1000);
}
