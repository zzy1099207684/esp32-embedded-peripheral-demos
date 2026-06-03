//
// Created by zhiyong on 11/17/25.
//

#ifndef STEPPER_MONITOR_H
#define STEPPER_MONITOR_H

#include "freertos/FreeRTOS.h"
#include "soc/gpio_num.h"
#include <driver/gpio.h>

#define DEFAULT_INTERVAL_MS 50

class Stepper_monitor
{
    gpio_num_t step_pin[4] = {GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14};

public:
    Stepper_monitor(gpio_num_t pin1, gpio_num_t pin2, gpio_num_t pin3, gpio_num_t pin4);
    ~Stepper_monitor() = default;

    // int interval_ms=100, bool direction=true
    void step_sequence(void *param);
    // 512 step /circle
    void full_step_sequence(int step, int interval_ms=DEFAULT_INTERVAL_MS, bool direction=true);
    void half_step_sequence(int step, int interval_ms=DEFAULT_INTERVAL_MS, bool direction=true);
    void close_all();

};


#endif //STEPPER_MONITOR_H
