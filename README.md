# STM32 鸢尾花分类项目

## 项目概述

本项目在STM32F411RE Nucleo板上部署了鸢尾花(Iris)分类神经网络模型，通过串口接收特征数据并返回分类结果。

**神经网络模型信息：**
- 输入：4个特征值（花萼长度、花萼宽度、花瓣长度、花瓣宽度）
- 输出：3个类别（Setosa、Versicolor、Virginica）
- 结构：3层全连接网络
- 参数量：323个参数

---

## 串口配置

| 参数 | 值 |
|------|-----|
| **波特率** | 115200 |
| **数据位** | 8 |
| **停止位** | 1 |
| **校验位** | 无 |
| **发送格式** | ASCII/文本 |
| **接收格式** | ASCII/文本 |

---

## 使用方法

### 输入格式
```
花萼长度 花萼宽度 花瓣长度 花瓣宽度
```

**支持的分隔符：** 空格、逗号、Tab

### 示例输入
```
5.1 3.5 1.4 0.2
```

---

## 运行结果示例

```
=== Iris Classification System ===
Initializing AI Model...
[AI] Initializing network...
[AI] Init OK
AI Model Loaded Successfully!

Usage: Enter 4 float values separated by space or comma
Format: sepal_len sepal_wid petal_len petal_wid
Example: 5.1 3.5 1.4 0.2
=================================
» 5.1 3.5 1.4 0.2
«
Input: (5.10, 3.50, 1.40, 0.20)

[DEBUG] In callback, calling AI_Predict...
[DEBUG] AI_Predict returned: 0
Predicted: Versicolor (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 100.0%, Virginica: 0.0%

» 7.0 3.2 4.7 1.4
«
Input: (7.00, 3.20, 4.70, 1.40)

[DEBUG] In callback, calling AI_Predict...
[DEBUG] AI_Predict returned: 0
Predicted: Virginica (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 0.0%, Virginica: 100.0%

» 6.3 3.3 6.0 2.5
«
Input: (6.30, 3.30, 6.00, 2.50)

[DEBUG] In callback, calling AI_Predict...
[DEBUG] AI_Predict returned: 0
Predicted: Virginica (100.0%)
Probabilities:
  Setosa: 0.0%, Versicolor: 0.0%, Virginica: 100.0%
```

---

## 测试数据

| 花萼长度 | 花萼宽度 | 花瓣长度 | 花瓣宽度 | 预期类别 |
|----------|----------|----------|----------|----------|
| 5.1 | 3.5 | 1.4 | 0.2 | Setosa |
| 4.9 | 3.0 | 1.4 | 0.2 | Setosa |
| 4.7 | 3.2 | 1.3 | 0.2 | Setosa |
| 7.0 | 3.2 | 4.7 | 1.4 | Versicolor |
| 6.4 | 3.2 | 4.5 | 1.5 | Versicolor |
| 6.3 | 3.3 | 6.0 | 2.5 | Virginica |
| 5.9 | 3.0 | 5.1 | 1.8 | Virginica |

---

## 类别说明

| 类别编号 | 英文名称 | 中文名称 |
|----------|----------|----------|
| 0 | Setosa | 山鸢尾 |
| 1 | Versicolor | 变色鸢尾 |
| 2 | Virginica | 维吉尼亚鸢尾 |

---

## 输入特征范围

| 特征 | 范围 (cm) |
|------|-----------|
| 花萼长度 (sepal length) | 4.3 ~ 7.9 |
| 花萼宽度 (sepal width) | 2.0 ~ 4.4 |
| 花瓣长度 (petal length) | 1.0 ~ 6.9 |
| 花瓣宽度 (petal width) | 0.1 ~ 2.5 |

---

## 项目结构

```
NUCLEO-F411RE/
├── Core/
│   ├── Inc/
│   │   ├── ai_inference.h      # AI推理接口
│   │   ├── serial_handler.h    # 串口处理接口
│   │   └── statistics.h        # 统计模块接口
│   └── Src/
│       ├── main.c              # 主程序
│       ├── ai_inference.c      # AI推理实现
│       ├── serial_handler.c    # 串口处理实现
│       └── statistics.c        # 统计模块实现
├── .ai/output/                  # X-CUBE-AI生成的网络代码
│   ├── network.c               # 网络结构
│   ├── network_data.c          # 网络数据
│   └── network_data_params.c   # 网络参数
├── X-CUBE-AI/App/              # X-CUBE-AI应用代码
├── MDK-ARM/                    # Keil项目
│   └── NUCLEO-F411RE.sct       # 链接脚本 (已配置AI内存区域)
└── README.md                   # 本文档
```

---

## 技术要点

### 内存配置
- 修改了链接脚本 `NUCLEO-F411RE.sct`，为AI保留 `0x20000000` 到 `0x20000200` 的内存区域
- 使用静态内存分配避免动态内存分配失败

### AI推理
- 使用X-CUBE-AI运行时库进行神经网络推理
- 输出经过softmax转换为概率值

### 串口处理
- 中断接收，主循环处理
- 支持空格、逗号分隔的输入格式

---

## 开发环境

- **IDE:** Keil MDK-ARM 5
- **目标芯片:** STM32F411RE
- **开发板:** NUCLEO-F411RE
- **AI工具:** X-CUBE-AI 8.1.0
- **模型格式:** ONNX
