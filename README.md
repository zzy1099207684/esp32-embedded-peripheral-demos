# ESP32 Embedded Peripheral Demos

Small ESP-IDF demos for embedded peripheral integration on ESP32-P4 and ESP32-S3.

This repository is organized as two standalone ESP-IDF projects. Each project keeps only source code, reusable components, build metadata, and documentation. Generated build output and downloaded managed components are intentionally excluded.

## Demos

### ESP32-P4 Camera JPEG Stream

Captures frames from an OV2640 camera through the ESP32-P4 DVP/ISP pipeline, encodes frames as JPEG, and exports the image stream over UART to a host PC.

Skills demonstrated:

- ESP-IDF project structure
- ESP32-P4 DVP camera capture
- ISP-based frame processing
- JPEG encoder usage
- FreeRTOS task, semaphore, and queue usage
- UART frame transfer to a host script

### ESP32-S3 TCA9555 GPIO Expander

Implements a reusable C++ driver for the TCA9555 16-bit I2C GPIO expander and demonstrates LED/button I/O patterns.

Skills demonstrated:

- I2C peripheral communication
- Register-level GPIO expander control
- Pin and port direction configuration
- Pin and port read/write APIs
- C++ embedded component design

### ESP32-S3 Stepper Motor Component

Provides a basic 4-phase stepper motor component with full-step and half-step GPIO sequencing.

Skills demonstrated:

- GPIO-based motor sequencing
- Full-step and half-step control patterns
- Direction control through sequence ordering
- Timing control with FreeRTOS delays

## Repository Layout

```text
ESP32P4_Camera_JPEG_Stream/
  main/
  components/Camera/
  CMakeLists.txt
  sdkconfig.defaults
  dependencies.lock

ESP32S3_Peripheral_Demos/
  main/
  components/GPIO_Expander/
  components/Stepper/
  CMakeLists.txt
```

## Build

Each demo is built from its own directory:

```bash
cd ESP32P4_Camera_JPEG_Stream
idf.py set-target esp32p4
idf.py build
```

```bash
cd ESP32S3_Peripheral_Demos
idf.py set-target esp32s3
idf.py build
```

## 中文说明

这是一个用于展示 ESP32 嵌入式外设开发经验的 demo 集合，包含 ESP32-P4 和 ESP32-S3 两个独立 ESP-IDF 项目。

仓库只保留源码、可复用组件、构建配置和说明文档。`build/`、`managed_components/`、IDE 配置、缓存文件和生成文件不放进仓库。

### ESP32-P4 Camera JPEG Stream

这个 demo 使用 OV2640 摄像头，通过 ESP32-P4 的 DVP/ISP 流程采集图像，将帧数据编码成 JPEG，再通过 UART 发送到电脑端脚本保存。

展示技能：

- ESP-IDF 项目结构
- ESP32-P4 DVP 摄像头采集
- ISP 图像处理流程
- JPEG 编码器使用
- FreeRTOS task、semaphore、queue
- UART 帧数据传输

### ESP32-S3 TCA9555 GPIO Expander

这个 demo 实现了 TCA9555 16 位 I2C GPIO 扩展芯片的 C++ 驱动，并提供 LED/按钮相关示例。

展示技能：

- I2C 外设通信
- 寄存器级 GPIO 扩展芯片控制
- 单引脚和端口方向配置
- 单引脚和端口读写 API
- 嵌入式 C++ 组件封装

### ESP32-S3 Stepper Motor Component

这个组件提供基础 4 相步进电机 GPIO 时序控制，包含全步和半步模式。

展示技能：

- GPIO 电机相序控制
- 全步和半步控制 pattern
- 通过反向时序控制方向
- 使用 FreeRTOS delay 控制步进间隔
