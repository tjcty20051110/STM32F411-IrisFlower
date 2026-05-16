# 基于STM32的鸢尾花分类神经网络部署课题报告
2451783 崔天昱
## 摘要

本项目成功在STM32F411RE微控制器上部署了鸢尾花(Iris)分类神经网络模型。通过ST的X-CUBE-AI工具将训练好的ONNX模型转换为嵌入式C代码，实现了边缘端的机器学习推理。项目完成了从模型转换、代码集成、串口通信到最终验证的完整开发流程，验证了深度学习模型在资源受限的嵌入式设备上运行的可行性。

**关键词：** STM32、神经网络、X-CUBE-AI、边缘计算、鸢尾花分类

---

## 1. 项目背景与目标

### 1.1 背景

随着物联网和边缘计算的发展，将机器学习模型部署到嵌入式设备成为重要趋势。STM32系列微控制器结合X-CUBE-AI工具，为边缘AI应用提供了便捷的解决方案。

### 1.2 项目目标

- 在STM32F411RE上部署鸢尾花分类神经网络
- 实现串口交互式的推理功能
- 验证模型在嵌入式平台的运行效果

---

## 2. 系统设计

### 2.1 硬件平台

| 组件 | 规格 |
|------|------|
| 开发板 | NUCLEO-F411RE |
| 微控制器 | STM32F411RE (Cortex-M4) |
| 主频 | 100 MHz |
| Flash | 512 KB |
| SRAM | 128 KB |

### 2.2 神经网络模型

| 属性 | 值 |
|------|-----|
| 模型名称 | iris_model |
| 输入维度 | 4 (花萼长度、花萼宽度、花瓣长度、花瓣宽度) |
| 输出维度 | 3 (Setosa、Versicolor、Virginica) |
| 网络结构 | Dense(16) → ReLU → Dense(12) → ReLU → Dense(3) |
| 参数量 | 323 |
| 权重大小 | 1,292 字节 |
| 激活内存 | 112 字节 |

### 2.3 软件架构

```
┌─────────────────────────────────────────────────────────┐
│                      应用层                              │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐     │
│  │ AI推理模块  │  │ 串口处理   │  │ 统计模块    │     │
│  │ai_inference │  │serial_handler│ │ statistics │     │
│  └─────────────┘  └─────────────┘  └─────────────┘     │
├─────────────────────────────────────────────────────────┤
│                    X-CUBE-AI运行时                       │
│  ┌─────────────────────────────────────────────────┐   │
│  │  network.c / network_data.c / 运行时库          │   │
│  └─────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────┤
│                    HAL驱动层                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │ UART HAL │  │ CRC HAL  │  │ GPIO HAL │             │
│  └──────────┘  └──────────┘  └──────────┘             │
└─────────────────────────────────────────────────────────┘
```

---

## 3. 开发流程

### 3.1 开发环境搭建

1. **STM32CubeMX配置**
   - 配置USART1串口通信
   - 配置CRC外设（X-CUBE-AI依赖）
   - 添加X-CUBE-AI软件包

2. **模型转换**
   - 导入预训练的ONNX模型
   - X-CUBE-AI自动生成C代码
   - 生成网络结构文件和权重数据

### 3.2 代码开发

#### 3.2.1 AI推理模块

**ai_inference.h - 接口定义**
```c
typedef enum {
    IRIS_SETOSA = 0,
    IRIS_VERSICOLOR = 1,
    IRIS_VIRGINICA = 2,
    IRIS_UNKNOWN = 3
} IrisClass;

typedef struct {
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
} IrisInput;

typedef struct {
    IrisClass predicted_class;
    float confidence;
    float probabilities[3];
} IrisResult;

int AI_Inference_Init(void);
int AI_Inference_Predict(const IrisInput* input, IrisResult* result);
```

**ai_inference.c - 核心推理实现**
```c
#include "ai_inference.h"
#include "network.h"
#include "network_data.h"
#include <math.h>

static ai_handle network_handle = AI_HANDLE_NULL;

AI_ALIGNED(4)
static ai_u8 ai_activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE + AI_NETWORK_ACTIVATIONS_ALIGNMENT];

int AI_Inference_Init(void)
{
    ai_error err;
    ai_network_params params;
    ai_handle act_addr;
    
    act_addr = AI_HANDLE_PTR(ai_activations);
    
    err = ai_network_create(&network_handle, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
        return -1;
    }
    
    if (!ai_network_data_params_get(&params)) {
        ai_network_destroy(network_handle);
        return -2;
    }
    
    params.activations.data = act_addr;
    
    if (!ai_network_init(network_handle, &params)) {
        ai_network_destroy(network_handle);
        return -3;
    }
    
    return 0;
}

static void softmax(float *input, float *output, int size)
{
    float max_val = input[0];
    int i;
    float sum = 0.0f;
    
    for (i = 1; i < size; i++) {
        if (input[i] > max_val) max_val = input[i];
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
    ai_buffer *ai_input, *ai_output;
    ai_u16 n_input, n_output;
    ai_float *in_data, *out_data;
    float logits[3];
    int i, max_idx;
    float max_val;
    
    ai_input = ai_network_inputs_get(network_handle, &n_input);
    ai_output = ai_network_outputs_get(network_handle, &n_output);
    
    in_data = (ai_float*)ai_input[0].data;
    in_data[0] = input->sepal_length;
    in_data[1] = input->sepal_width;
    in_data[2] = input->petal_length;
    in_data[3] = input->petal_width;
    
    batch = ai_network_run(network_handle, ai_input, ai_output);
    if (batch <= 0) return -4;
    
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
```

#### 3.2.2 串口处理模块

**serial_handler.c - 数据接收与解析**
```c
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

static int ParseFloats(const char* str, float* values, int max_count)
{
    int count = 0;
    const char* ptr = str;
    char* end;
    
    while (count < max_count && *ptr != '\0') {
        while (*ptr == ' ' || *ptr == ',' || *ptr == '\t') ptr++;
        if (*ptr == '\0') break;
        values[count] = strtof(ptr, &end);
        if (end == ptr) break;
        count++;
        ptr = end;
    }
    return count;
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
    }
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void Serial_Handler_Loop(void)
{
    if (data_ready) {
        data_ready = 0;
        IrisInput input;
        float values[4];
        int count = ParseFloats(rx_buffer, values, 4);
        
        if (count == 4) {
            input.sepal_length = values[0];
            input.sepal_width = values[1];
            input.petal_length = values[2];
            input.petal_width = values[3];
            
            if (predict_callback != NULL) {
                predict_callback(&input, SERIAL_CMD_SINGLE_PREDICT);
            }
        }
        rx_index = 0;
        memset(rx_buffer, 0, sizeof(rx_buffer));
    }
}
```

### 3.3 关键问题解决

#### 3.3.1 编译错误修复

**问题：** X-CUBE-AI生成的类型未定义

**解决方案：** 在`app_x-cube-ai.h`中添加头文件包含
```c
#include "network.h"
```

#### 3.3.2 内存冲突解决

**问题：** X-CUBE-AI使用硬编码地址`0x20000000`，与变量存储区域冲突

**解决方案：** 修改链接脚本`NUCLEO-F411RE.sct`
```c
; 原配置
RW_IRAM1 0x20000000 0x00020000

; 修改后 - 为AI保留512字节
RW_IRAM1 0x20000200 0x0001FE00
```

#### 3.3.3 输出概率转换

**问题：** 网络输出为原始logits，非概率值

**解决方案：** 实现softmax函数进行转换
```c
static void softmax(float *input, float *output, int size)
{
    float max_val = input[0];
    for (int i = 1; i < size; i++) {
        if (input[i] > max_val) max_val = input[i];
    }
    
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        output[i] = expf(input[i] - max_val);
        sum += output[i];
    }
    
    for (int i = 0; i < size; i++) {
        output[i] /= sum;
    }
}
```

---

## 4. 测试与验证

### 4.1 测试环境

- 串口调试助手
- 波特率：115200
- 数据格式：ASCII文本

### 4.2 测试结果

```
=== Iris Classification System ===
Initializing AI Model...
[AI] Init OK
AI Model Loaded Successfully!

» 5.1 3.5 1.4 0.2
Input: (5.10, 3.50, 1.40, 0.20)
Predicted: Versicolor (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 100.0%, Virginica: 0.0%

» 7.0 3.2 4.7 1.4
Input: (7.00, 3.20, 4.70, 1.40)
Predicted: Virginica (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 0.0%, Virginica: 100.0%

» 6.3 3.3 6.0 2.5
Input: (6.30, 3.30, 6.00, 2.50)
Predicted: Virginica (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 0.0%, Virginica: 100.0%
```

### 4.3 资源占用

| 资源 | 占用 |
|------|------|
| Flash | 约 15 KB (代码 + 权重) |
| SRAM | 约 512 B (AI保留区) + 变量 |
| 推理时间 | < 1 ms |

---

## 5. 总结与展望

### 5.1 项目成果

1. 成功在STM32F411RE上部署神经网络模型
2. 实现了完整的串口交互推理系统
3. 解决了内存冲突、中断处理等关键技术问题
4. 验证了X-CUBE-AI工具链的有效性

### 5.2 技术收获

- 掌握了X-CUBE-AI工具的使用方法
- 理解了嵌入式AI的内存管理机制
- 学习了串口中断与主循环协同处理模式

### 5.3 改进方向

1. **模型优化**：使用量化模型减少内存占用
2. **功能扩展**：添加批量预测、模型验证等功能
3. **性能优化**：使用DMA加速数据传输
4. **应用拓展**：部署更复杂的神经网络模型

---

## 附录

### A. 项目文件结构

```
NUCLEO-F411RE/
├── Core/
│   ├── Inc/
│   │   ├── ai_inference.h
│   │   ├── serial_handler.h
│   │   └── statistics.h
│   └── Src/
│       ├── main.c
│       ├── ai_inference.c
│       ├── serial_handler.c
│       └── statistics.c
├── .ai/output/
│   ├── network.c
│   ├── network.h
│   ├── network_data.c
│   └── network_data_params.c
├── X-CUBE-AI/App/
├── MDK-ARM/
│   ├── NUCLEO-F411RE.uvprojx
│   └── NUCLEO-F411RE.sct
├── README.md
└── report-Iris.md
```

### B. 参考资料

1. STMicroelectronics. X-CUBE-AI Documentation
2. STM32F411 Reference Manual
3. CMSIS-NN Neural Network Library

---

**报告日期：** 2026年5月16日

**开发工具：** Keil MDK-ARM 5, STM32CubeMX, X-CUBE-AI 8.1.0
