#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "TCA9555.h"
#include "Stepper_monitor.h"

static const char *TAG = "MAIN";

// TCA9555 on I2C bus 0. Address 0x20 is used when A2/A1/A0 are tied to GND.
TCA9555 io(I2C_NUM_0, 0x20);

/**
 * Blink one LED connected to a TCA9555 output pin.
 *
 * Hardware: TCA9555 P03 -> LED -> resistor -> GND.
 */
void example_led_blink(void *param) {
    ESP_LOGI(TAG, "=== Example 1: LED blink ===");

    io.set_pin_direction(3, false);

    while (1) {
        io.write_pin(3, true);
        ESP_LOGI(TAG, "LED on");
        vTaskDelay(pdMS_TO_TICKS(1000));

        io.write_pin(3, false);
        ESP_LOGI(TAG, "LED off");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * Read one button connected to a TCA9555 input pin.
 *
 * Hardware: TCA9555 P05 -> button -> GND, with pull-up to VCC.
 */
void example_button_read(void *param) {
    ESP_LOGI(TAG, "=== Example 2: Button input ===");

    io.set_pin_direction(5, true);
    bool last_state = false;

    while (1) {
        bool current_state = io.read_pin(5);

        if (current_state != last_state) {
            ESP_LOGI(TAG, current_state ? "Button released" : "Button pressed");
            last_state = current_state;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * Run an 8-LED pattern using a full TCA9555 port write.
 *
 * Hardware: TCA9555 P00-P07 -> LEDs.
 */
void example_multiple_leds(void *param) {
    ESP_LOGI(TAG, "=== Example 3: 8-LED running pattern ===");

    io.set_port_direction(0, 0b00000000);

    while (1) {
        for (int i = 0; i < 8; i++) {
            io.write_port(0, (1 << i));
            ESP_LOGI(TAG, "LED %d on", i);
            vTaskDelay(pdMS_TO_TICKS(200));
        }

        io.write_port(0, 0xFF);
        ESP_LOGI(TAG, "All LEDs on");
        vTaskDelay(pdMS_TO_TICKS(500));

        io.write_port(0, 0x00);
        ESP_LOGI(TAG, "All LEDs off");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * Read 8 buttons through one TCA9555 port.
 *
 * Hardware: TCA9555 P10-P17 -> buttons to GND, with pull-ups.
 */
void example_keypad(void *param) {
    ESP_LOGI(TAG, "=== Example 4: 8-button input port ===");

    io.set_port_direction(1, 0b11111111);
    uint8_t last_keys = 0xFF;

    while (1) {
        uint8_t current_keys;
        io.read_port(1, &current_keys);

        uint8_t changed = last_keys ^ current_keys;

        if (changed) {
            for (int i = 0; i < 8; i++) {
                if (changed & (1 << i)) {
                    ESP_LOGI(TAG, (current_keys & (1 << i)) ? "P1%d released" : "P1%d pressed", i);
                }
            }
            last_keys = current_keys;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * Demonstrate TCA9555 input polarity inversion.
 *
 * Hardware: TCA9555 P04 -> button -> VCC.
 */
void example_polarity_invert(void *param) {
    ESP_LOGI(TAG, "=== Example 5: Polarity inversion ===");

    io.set_pin_direction(4, true);
    io.set_pin_polarity(4, true);

    while (1) {
        bool pressed = !io.read_pin(4);

        if (pressed) {
            ESP_LOGI(TAG, "Button pressed after polarity inversion");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * Toggle one TCA9555 output LED from one TCA9555 input button.
 *
 * Hardware: P00 -> LED -> GND, P05 -> button -> GND with pull-up.
 */
void example_led_button_interactive(void *param) {
    ESP_LOGI(TAG, "=== Example 6: Button-controlled LED ===");

    io.set_pin_direction(0, false);
    io.set_pin_direction(5, true);

    bool led_state = false;
    bool last_button = true;

    while (1) {
        bool button = io.read_pin(5);

        if (!button && last_button) {
            led_state = !led_state;
            io.write_pin(0, led_state);
            ESP_LOGI(TAG, "Button pressed, LED %s", led_state ? "on" : "off");
        }

        last_button = button;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/**
 * Show low-level TCA9555 register access.
 *
 * This example writes configuration/output registers directly, then reads input port 0.
 */
void example_register_access(void *param) {
    ESP_LOGI(TAG, "=== Example 7: Register access ===");

    io.write(TCA9555::REG_CONFIG0, 0b11111100);
    io.write(0x02, 0x01);

    uint8_t value;
    io.read(TCA9555::REG_INPUT_PORT0, &value);
    ESP_LOGI(TAG, "Input port 0 state: 0x%02X", value);

    vTaskDelete(NULL);
}

/**
 * Drive a 4-phase stepper motor with half-step GPIO sequencing.
 *
 * Hardware: ESP32-S3 GPIO11-GPIO14 -> stepper driver input pins.
 */
void example_stepper_half_step(void *param) {
    ESP_LOGI(TAG, "=== Example 8: Stepper half-step sequence ===");

    Stepper_monitor stepper(GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14);

    while (1) {
        stepper.half_step_sequence(128, 5, true);
        stepper.close_all();
        vTaskDelay(pdMS_TO_TICKS(1000));

        stepper.half_step_sequence(128, 5, false);
        stepper.close_all();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 peripheral demo started");

    esp_err_t ret = io.init(GPIO_NUM_8, GPIO_NUM_9, 400000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TCA9555 initialization failed: %s", esp_err_to_name(ret));
        return;
    }
    ESP_LOGI(TAG, "TCA9555 initialized");

    // Select one demo task to run by uncommenting one line.
    // xTaskCreate(example_led_blink, "led_blink", 2048, NULL, 5, NULL);
    // xTaskCreate(example_button_read, "button_read", 2048, NULL, 5, NULL);
    xTaskCreate(example_multiple_leds, "multi_leds", 2048, NULL, 5, NULL);
    // xTaskCreate(example_keypad, "keypad", 2048, NULL, 5, NULL);
    // xTaskCreate(example_polarity_invert, "polarity", 2048, NULL, 5, NULL);
    // xTaskCreate(example_led_button_interactive, "interactive", 2048, NULL, 5, NULL);
    // xTaskCreate(example_register_access, "register", 2048, NULL, 5, NULL);
    // xTaskCreate(example_stepper_half_step, "stepper", 2048, NULL, 5, NULL);
}
