# Ball-sorting · 智能小球分拣系统
> **OpenMV 颜色识别 + STM32F103C8T6 舵机分拣** —— 实时识别 7 种颜色小球，串口下发颜色码，主控驱动舵机按颜色分拣，OLED 实时统计各类球数，全自动分拣闭环！
[![OpenMV](https://img.shields.io/badge/Vision-OpenMV-blue.svg)](https://openmv.io/)
[![STM32](https://img.shields.io/badge/MCU-STM32F103C8T6%20HAL-green.svg)](https://www.st.com/)
[![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-orange.svg)](https://www.st.com/en/development-tools/stm32cubeide.html)

---
## 📖 项目简介
本项目是一套**智能小球分拣系统**，采用 **双端协同** 架构：

- **视觉识别端（OpenMV）**：`xiao_qiu.py` 通过摄像头在 ROI 区域检测**红、绿、黄、蓝、粉、浅蓝、橙** 7 种颜色小球（基于 HSV 色域阈值 + 最大色块筛选），每秒通过串口下发一次颜色识别码。
- **主控执行端（STM32F103C8T6 · HAL）**：`xiao_qiu/` 为 STM32CubeIDE 工程，USART2 中断接收 OpenMV 下发的颜色码，对应驱动 **舵机（PWM）** 将小球分拣到对应颜色通道，并通过 **OLED** 实时统计各类小球数量。

系统形成 **视觉识别 → 串口下发 → 舵机分拣 → 计数显示** 的完整自动分拣闭环，代码基于 OpenMV MicroPython 与 STM32 HAL 库，结构清晰、直接可跑，可作为视觉分拣类赛题的开发模板。✨

---
## ✨ 核心特性
- 🎨 **7 色小球识别**：红 / 绿 / 黄 / 蓝 / 粉 / 浅蓝 / 橙，HSV 色域阈值精确区分
- 📐 **ROI 区域检测**：固定 ROI 检测 + 最大色块筛选，排除环境干扰
- ⚙️ **舵机分拣**：STM32 收到颜色码后设置 PWM 占空比，按颜色转到对应分拣通道
- 🔢 **OLED 实时统计**：分别显示蓝 / 红 / 绿 / 黄 / 粉 / 浅 / 橙的累计分拣球数
- 📡 **UART 双端通信**：OpenMV（UART3 115200）⇄ STM32（USART2）单字节颜色码协议
- 🔄 **空闲检测**：无目标时下发 `'1'`（无球标志），避免重复触发

---
## 🧩 已集成模块清单
### 👁️ OpenMV 视觉识别（根目录）
| 文件 | 功能说明 |
| :--- | :--- |
| xiao_qiu.py | 7 色小球识别（HSV 阈值）、最大色块筛选、ROI 检测、串口下发颜色码 |

### ⚙️ STM32F103C8T6 主控（xiao_qiu/）
| 模块名称 | 功能说明 | 接口 |
| :--- | :--- | :--- |
| 舵机分拣控制 | 收到颜色码设置 PWM 占空比（TIM2_CH2 / TIM4_CH3），按颜色分拣 | PWM/TIM |
| oled | OLED 显示各类小球计数（蓝/红/绿/黄/粉/浅/橙） | I2C |
| usart | USART2 中断接收 OpenMV 颜色码（HAL_UART_Receive_IT） | USART2 |
| tim | 舵机 PWM（TIM2 / TIM4 / TIM3） | TIM |

---
## 🚀 快速上手
### 1. 环境要求
- **视觉端**：OpenMV Cam，MicroPython（RGB565 摄像头）
- **主控**：STM32F103C8T6，STM32CubeIDE（HAL 库）
- **执行机构**：舵机（50Hz PWM，占空比范围约 2.5%~12.5%）

### 2. 部署步骤
1.  克隆本仓库到本地：
    ```bash
    git clone https://github.com/kout520/Ball-sorting.git
    ```
2.  **OpenMV 端**：用 OpenMV IDE 打开并运行 `xiao_qiu.py`，确认摄像头 ROI 与颜色阈值
3.  **STM32 端**：用 STM32CubeIDE 打开 `xiao_qiu/` 工程，编译烧录到 STM32F103C8T6
4.  接线：OpenMV（UART3）⇄ STM32（USART2）波特率一致（115200），舵机 PWM 接到对应定时器通道
5.  上电运行：OpenMV 识别小球颜色 → 串口下发 → STM32 舵机分拣 → OLED 统计球数

### 3. 串口颜色码协议
| 颜色码 | 含义 |
| :--- | :--- |
| `'1'` | 未检测到目标（无球） |
| `'2'` | 蓝色 |
| `'3'` | 红色 |
| `'4'` | 绿色 |
| `'5'` | 黄色 |
| `'6'` | 粉色 |
| `'7'` | 浅蓝色 |
| `'8'` | 橙色 |

> OpenMV 每秒下发一次识别码；STM32 收到后对应设置舵机 PWM 占空比分拣并累加计数。

---
## 📂 项目结构
```
Ball-sorting/
├── xiao_qiu.py           # OpenMV 主程序（7 色识别 + 串口下发）
├── xiao_qiu/             # STM32F103C8T6 主控工程（STM32CubeIDE）
│   ├── xiao_qiu.ioc      # STM32CubeMX 工程配置
│   ├── Core/Src/         # main.c / usart / tim / i2c / oled / gpio
│   ├── Drivers/          # STM32 HAL 驱动
│   └── Debug/            # 构建产物（elf/map/list）
└── README.md             # 项目说明文档
```

---
## 📄 开源协议
本项目**未指定开源许可证**，如需对外开源请自行添加 `LICENSE` 文件。

---
## 🎉 致谢
感谢 OpenMV 与 STMicroelectronics 提供的优秀开发平台，祝各位在竞赛中取得优异成绩！
---
