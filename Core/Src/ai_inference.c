#include "ai_inference.h"
#include "network.h"
#include "network_data.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern UART_HandleTypeDef huart1;

static void debug_print(const char* msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}

static void debug_int(const char* prefix, int val)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%s%d\r\n", prefix, val);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
}

static ai_handle network_handle = AI_HANDLE_NULL;

AI_ALIGNED(4)
static ai_u8 ai_activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE + AI_NETWORK_ACTIVATIONS_ALIGNMENT];

int AI_Inference_Init(void)
{
    ai_error err;
    ai_network_params params;
    ai_handle act_addr;
    
    debug_print("[AI] Initializing network...\r\n");
    
    act_addr = AI_HANDLE_PTR(ai_activations);
    
    err = ai_network_create(&network_handle, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
        return -1;
    }
    
    if (!ai_network_data_params_get(&params)) {
        ai_network_destroy(network_handle);
        network_handle = AI_HANDLE_NULL;
        return -2;
    }
    
    params.activations.data = act_addr;
    
    if (!ai_network_init(network_handle, &params)) {
        ai_network_destroy(network_handle);
        network_handle = AI_HANDLE_NULL;
        return -3;
    }
    
    debug_print("[AI] Init OK\r\n");
    return 0;
}

static void softmax(float *input, float *output, int size)
{
    float max_val = input[0];
    int i;
    float sum = 0.0f;
    
    for (i = 1; i < size; i++) {
        if (input[i] > max_val) {
            max_val = input[i];
        }
    }
    
    for (i = 0; i < size; i++) {
        output[i] = expf(input[i] - max_val);
        sum += output[i];
    }
    
    for (i = 0; i < size; i++) {
        output[i] /= sum;
    }
}

int AI_Inference_Predict(const IrisInput* input, IrisResult* result)
{
    ai_i32 batch;
    ai_buffer *ai_input;
    ai_buffer *ai_output;
    ai_u16 n_input, n_output;
    ai_float *in_data;
    ai_float *out_data;
    float logits[3];
    int i;
    int max_idx;
    float max_val;
    
    if (network_handle == AI_HANDLE_NULL) {
        return -1;
    }
    
    ai_input = ai_network_inputs_get(network_handle, &n_input);
    ai_output = ai_network_outputs_get(network_handle, &n_output);
    
    if (ai_input == NULL || ai_output == NULL) {
        return -2;
    }
    
    if (ai_input[0].data == NULL || ai_output[0].data == NULL) {
        return -3;
    }
    
    in_data = (ai_float*)ai_input[0].data;
    in_data[0] = input->sepal_length;
    in_data[1] = input->sepal_width;
    in_data[2] = input->petal_length;
    in_data[3] = input->petal_width;
    
    batch = ai_network_run(network_handle, ai_input, ai_output);
    
    if (batch <= 0) {
        return -4;
    }
    
    out_data = (ai_float*)ai_output[0].data;
    
    logits[0] = out_data[0];
    logits[1] = out_data[1];
    logits[2] = out_data[2];
    
    softmax(logits, result->probabilities, 3);
    
    max_idx = 0;
    max_val = result->probabilities[0];
    for (i = 1; i < 3; i++) {
        if (result->probabilities[i] > max_val) {
            max_val = result->probabilities[i];
            max_idx = i;
        }
    }
    
    result->predicted_class = (IrisClass)max_idx;
    result->confidence = max_val * 100.0f;
    
    return 0;
}

void AI_Inference_Deinit(void)
{
    if (network_handle != AI_HANDLE_NULL) {
        ai_network_destroy(network_handle);
        network_handle = AI_HANDLE_NULL;
    }
}

const char* AI_Inference_GetClassName(IrisClass cls)
{
    switch (cls) {
        case IRIS_SETOSA:     return "Setosa";
        case IRIS_VERSICOLOR: return "Versicolor";
        case IRIS_VIRGINICA:  return "Virginica";
        default:              return "Unknown";
    }
}

int AI_Inference_GetInputSize(void)
{
    return AI_NETWORK_IN_1_SIZE;
}

int AI_Inference_GetOutputSize(void)
{
    return AI_NETWORK_OUT_1_SIZE;
}
