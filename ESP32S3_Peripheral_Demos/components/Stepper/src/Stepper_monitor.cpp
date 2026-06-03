//
// Created by zhiyong on 11/17/25.
//

#include "Stepper_monitor.h"

Stepper_monitor::Stepper_monitor(gpio_num_t pin1, gpio_num_t pin2, gpio_num_t pin3, gpio_num_t pin4)
    : step_pin{pin1, pin2, pin3, pin4}
{
    gpio_set_direction(pin1, GPIO_MODE_OUTPUT);
    gpio_set_direction(pin2, GPIO_MODE_OUTPUT);
    gpio_set_direction(pin3, GPIO_MODE_OUTPUT);
    gpio_set_direction(pin4, GPIO_MODE_OUTPUT);
}

void Stepper_monitor::full_step_sequence(int step, int interval_ms, bool direction)
{
    uint8_t direction_true[4] = {0, 1, 2, 3};
    uint8_t direction_false[4] = {3, 2, 1, 0};
    uint8_t* step_pin_sequence = direction == true ? direction_true : direction_false;

    for (int st = 0; st < step; ++st)
    {
        for (int i = 0; i < 4; ++i)
        {
            gpio_set_level(step_pin[step_pin_sequence[i]], 1);
            gpio_set_level(step_pin[(step_pin_sequence[i] + 1) % 4], 0);
            gpio_set_level(step_pin[(step_pin_sequence[i] + 2) % 4], 0);
            gpio_set_level(step_pin[(step_pin_sequence[i] + 3) % 4], 0);
            vTaskDelay(pdMS_TO_TICKS(interval_ms));
        }
    }
}

void Stepper_monitor::half_step_sequence(int step, int interval_ms, bool direction)
{
    uint8_t step_patterns[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}
    };

    for (int st = 0; st < step; ++st)
    {
        for (int i = 0; i < 8; ++i)
        {
            int idx = direction ? i : (7 - i);
            for (int pin = 0; pin < 4; ++pin)
            {
                gpio_set_level(step_pin[pin], step_patterns[idx][pin]);
            }
            vTaskDelay(pdMS_TO_TICKS(interval_ms));
        }
    }
}

void Stepper_monitor::close_all()
{
    gpio_set_level(step_pin[0], 0);
    gpio_set_level(step_pin[1], 0);
    gpio_set_level(step_pin[2], 0);
    gpio_set_level(step_pin[3], 0);
    vTaskDelay(pdMS_TO_TICKS(100));
}
