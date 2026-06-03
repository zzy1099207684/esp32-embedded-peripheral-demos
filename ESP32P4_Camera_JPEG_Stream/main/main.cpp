/*
 * ESP32-P4 Camera JPEG UART Stream
 *
 * Captures frames from an OV2640 camera, encodes them as JPEG,
 * and prints each frame as a hex stream over UART.
 */

#include <memory>

#include "camera_ov2640.h"
#include "driver/uart.h"

static const char* TAG = "cam_main";

void camera_task(void* arg) {
    auto* cam = static_cast<camera_ov2640*>(arg);
    cam->get_data();
}

extern "C" void app_main() {
    uart_config_t uart_config = {
        .baud_rate = 5*1024000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_0, &uart_config);

    camera_ov2640 cam(80);

    if (cam.get_ret() != ESP_OK) {
        ESP_LOGE("main", "Camera initialization failed");
        return;
    }

    // Start the capture task.
    xTaskCreate(camera_task, "cam_task", 4096, &cam, 1, nullptr);

    // Read the latest JPEG and export it over UART.
    while (1) {
        uint32_t size;
        const uint8_t* jpeg = cam.get_latest_jpeg(&size);
        if (jpeg && size > 0) {
            // Copy the frame before printing so the producer cannot overwrite it mid-transfer.
            uint8_t* local_buf = (uint8_t*)malloc(size);
            if (local_buf) {
                memcpy(local_buf, jpeg, size);

                printf("JPEG_START\n");
                for (uint32_t i = 0; i < size; i++) {
                    printf("%02x", local_buf[i]);
                    if ((i + 1) % 1024 == 0) {
                        printf("\n");
                        fflush(stdout);
                        vTaskDelay(pdMS_TO_TICKS(5));
                    }
                }
                printf("\nJPEG_END\n");
                free(local_buf);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
