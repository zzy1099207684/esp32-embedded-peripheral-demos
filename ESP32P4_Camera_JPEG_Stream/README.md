# ESP32-P4 Camera JPEG Stream

Captures JPEG frames from an OV2640 camera through the ESP32-P4 DVP/ISP pipeline and streams them over UART to a host PC.

## Architecture

```text
OV2640 camera
  -> ESP32-P4 DVP camera controller
  -> ISP RGB565 frame processing
  -> JPEG encoder
  -> UART hex stream
  -> host script saves image.jpg
```

## Hardware

- ESP32-P4 development board
- OV2640 DVP camera module
- USB serial connection to the host PC

## Key Implementation

- Custom `camera_ov2640` C++ component wrapping ESP-IDF camera controller APIs
- FreeRTOS task for continuous frame capture and JPEG conversion
- Semaphore signal from camera interrupt callback to frame processing task
- Triple JPEG buffer pool with `QueueHandle_t` for producer/consumer handoff
- UART output using `JPEG_START` and `JPEG_END` frame markers
- Host-side Python script that reconstructs JPEG bytes from the UART hex stream

## Build

```bash
idf.py set-target esp32p4
idf.py build
idf.py flash monitor
```

## Receive Frames On Host

Install `pyserial`, edit the serial port in `main/grab_jpeg.py`, then run:

```bash
python main/grab_jpeg.py
```

The script writes the latest frame to `image.jpg`.

## Project Structure

```text
main/
  main.cpp       Application entry, UART setup, camera task, JPEG output loop
  grab_jpeg.py   Host receiver script for UART JPEG frames

components/
  Camera/        OV2640 camera component, DVP/ISP setup, JPEG buffering
```

## 中文说明

这个 demo 使用 ESP32-P4 和 OV2640 摄像头实现 JPEG 图像采集与串口传输。

### 架构

```text
OV2640 camera
  -> ESP32-P4 DVP camera controller
  -> ISP RGB565 frame processing
  -> JPEG encoder
  -> UART hex stream
  -> host script saves image.jpg
```

### 硬件

- ESP32-P4 开发板
- OV2640 DVP 摄像头模块
- 连接电脑的 USB 串口

### 关键实现

- 自定义 `camera_ov2640` C++ 组件，封装 ESP-IDF camera controller API
- 使用 FreeRTOS task 持续采集图像并转换 JPEG
- 摄像头中断 callback 通过 semaphore 通知处理任务
- 使用 3 个 JPEG buffer 和 `QueueHandle_t` 做生产者/消费者交接
- UART 使用 `JPEG_START` 和 `JPEG_END` 标记一帧 JPEG 数据
- 电脑端 Python 脚本从 UART hex stream 还原 JPEG 图片

### 构建

```bash
idf.py set-target esp32p4
idf.py build
idf.py flash monitor
```

### 电脑端接收

安装 `pyserial`，修改 `main/grab_jpeg.py` 里的串口名，然后运行：

```bash
python main/grab_jpeg.py
```

脚本会把最新一帧保存为 `image.jpg`。
