#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ai_inference.h"

#define SERIAL_BUFFER_SIZE 256
#define SERIAL_FRAME_HEAD 0xAA
#define SERIAL_FRAME_TAIL 0x55

#define SERIAL_CMD_SINGLE_PREDICT 0x01
#define SERIAL_CMD_BATCH_PREDICT  0x02
#define SERIAL_CMD_VALIDATE_MODE  0x03
#define SERIAL_CMD_GET_INFO       0x04

typedef enum {
    SERIAL_STATE_IDLE = 0,
    SERIAL_STATE_HEAD,
    SERIAL_STATE_LEN,
    SERIAL_STATE_CMD,
    SERIAL_STATE_DATA,
    SERIAL_STATE_CHECKSUM,
    SERIAL_STATE_TAIL
} SerialState;

typedef struct {
    uint8_t cmd;
    uint8_t data_len;
    uint8_t data[SERIAL_BUFFER_SIZE];
    uint8_t checksum;
} SerialFrame;

typedef void (*SerialPredictCallback)(const IrisInput* input, uint8_t cmd);

void Serial_Handler_Init(void);
void Serial_Handler_Process(void);
void Serial_Handler_Loop(void);
void Serial_Handler_SetCallback(SerialPredictCallback callback);
void Serial_Handler_SendResult(const IrisResult* result, const IrisInput* input);
void Serial_Handler_SendString(const char* str);
void Serial_Handler_SendInfo(void);

#ifdef __cplusplus
}
#endif

#endif
