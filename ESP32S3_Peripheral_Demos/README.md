# ESP32-S3 Peripheral Demos

ESP-IDF demos for ESP32-S3 peripheral components: a TCA9555 I2C GPIO expander driver and a basic 4-phase stepper motor component.

## Included Components

### TCA9555 GPIO Expander

The `GPIO_Expander` component implements a C++ driver for the TCA9555 16-bit I2C GPIO expander.

Supported operations:

- Initialize the I2C bus
- Read and write TCA9555 registers
- Configure single-pin and full-port direction
- Read single-pin and full-port input state
- Write single-pin and full-port output state
- Configure polarity inversion

The current `main.cpp` demo initializes the TCA9555 on ESP32-S3 GPIO8/GPIO9 and runs an LED pattern on port 0.

### Stepper Motor

The `Stepper` component provides basic GPIO sequencing for a 4-phase stepper motor.

Supported operations:

- Full-step sequence
- Half-step sequence
- Direction selection by reversing the sequence order
- Motor output shutdown through `close_all()`

This component is kept as a standalone reusable module to show basic motor-control exposure without mixing it into the TCA9555 demo loop.

## Hardware

- ESP32-S3 development board
- TCA9555 I2C GPIO expander module
- LEDs connected to TCA9555 P00-P07 through current-limiting resistors
- Optional 4-phase stepper motor driver module connected to four ESP32-S3 GPIO pins

## Wiring

```text
ESP32-S3 GPIO8  -> TCA9555 SDA
ESP32-S3 GPIO9  -> TCA9555 SCL
TCA9555 P00-P07 -> LEDs
```

Stepper pins are selected when constructing `Stepper_monitor`.

## Build

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

## Project Structure

```text
main/
  main.cpp

components/
  GPIO_Expander/
    include/TCA9555.h
    src/TCA9555.cpp

  Stepper/
    include/Stepper_monitor.h
    src/Stepper_monitor.cpp
```

## 中文说明

这个 ESP32-S3 demo 包含两个外设组件：TCA9555 I2C GPIO 扩展芯片驱动，以及基础 4 相步进电机控制组件。

### TCA9555 GPIO Expander

`GPIO_Expander` 组件实现了 TCA9555 16 位 I2C GPIO 扩展芯片的 C++ 驱动。

支持功能：

- 初始化 I2C 总线
- 读写 TCA9555 寄存器
- 配置单个引脚和整个端口的方向
- 读取单个引脚和整个端口的输入状态
- 写入单个引脚和整个端口的输出状态
- 配置输入极性反转

当前 `main.cpp` demo 使用 ESP32-S3 GPIO8/GPIO9 初始化 TCA9555，并在 port 0 上运行 LED 流水灯。

### Stepper Motor

`Stepper` 组件提供基础 4 相步进电机 GPIO 时序控制。

支持功能：

- 全步时序
- 半步时序
- 通过反转时序顺序控制方向
- 通过 `close_all()` 关闭电机输出

这个组件作为独立可复用模块保留，用于展示基础电机控制接触经验，不强行混进 TCA9555 的主 demo 循环。

### 硬件

- ESP32-S3 开发板
- TCA9555 I2C GPIO 扩展模块
- TCA9555 P00-P07 接 LED 和限流电阻
- 可选 4 相步进电机驱动模块，接 ESP32-S3 的 4 个 GPIO

### 接线

```text
ESP32-S3 GPIO8  -> TCA9555 SDA
ESP32-S3 GPIO9  -> TCA9555 SCL
TCA9555 P00-P07 -> LEDs
```

Stepper 引脚在创建 `Stepper_monitor` 对象时指定。

### 构建

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```
