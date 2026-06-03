#include "TCA9555.h"
#include "esp_log.h"

static const char *TAG = "TCA9555";

TCA9555::TCA9555(i2c_port_t i2c_num, uint8_t addr)
    : _i2c_num(i2c_num), _addr(addr) {
}

esp_err_t TCA9555::init(gpio_num_t sda_pin, gpio_num_t scl_pin, uint32_t freq) {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda_pin;
    conf.scl_io_num = scl_pin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = freq;

    esp_err_t ret = i2c_param_config(_i2c_num, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C parameter configuration failed");
        return ret;
    }

    ret = i2c_driver_install(_i2c_num, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver installation failed");
        return ret;
    }

    ESP_LOGI(TAG, "TCA9555 initialized at address 0x%02X", _addr);
    return ESP_OK;
}

esp_err_t TCA9555::read(uint8_t reg, uint8_t *value) {
    return i2c_master_write_read_device(
        _i2c_num,
        _addr,
        &reg,
        1,
        value,
        1,
        pdMS_TO_TICKS(100)
    );
}

esp_err_t TCA9555::write(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};

    return i2c_master_write_to_device(
        _i2c_num,
        _addr,
        data,
        2,
        pdMS_TO_TICKS(100)
    );
}

esp_err_t TCA9555::set_pin_direction(uint8_t pin, bool is_input) {
    if (pin > 15) {
        ESP_LOGE(TAG, "Invalid pin: %d, expected 0-15", pin);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (pin >= 8) ? REG_CONFIG1 : REG_CONFIG0;
    uint8_t bit = pin % 8;

    uint8_t current;
    esp_err_t ret = read(reg, &current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read direction register");
        return ret;
    }

    if (is_input) {
        current |= (1 << bit);
    } else {
        current &= ~(1 << bit);
    }

    ret = write(reg, current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write direction register");
    }

    return ret;
}

esp_err_t TCA9555::set_port_direction(uint8_t port, uint8_t dir_mask) {
    if (port > 1) {
        ESP_LOGE(TAG, "Invalid port: %d, expected 0 or 1", port);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (port == 0) ? REG_CONFIG0 : REG_CONFIG1;
    return write(reg, dir_mask);
}

bool TCA9555::read_pin(uint8_t pin) {
    if (pin > 15) {
        ESP_LOGE(TAG, "Invalid pin: %d", pin);
        return false;
    }

    uint8_t reg = (pin >= 8) ? REG_INPUT_PORT1 : REG_INPUT_PORT0;
    uint8_t bit = pin % 8;

    uint8_t value;
    if (read(reg, &value) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read input port");
        return false;
    }

    return (value >> bit) & 0x01;
}

esp_err_t TCA9555::read_port(uint8_t port, uint8_t *value) {
    if (port > 1) {
        ESP_LOGE(TAG, "Invalid port: %d", port);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (port == 0) ? REG_INPUT_PORT0 : REG_INPUT_PORT1;
    return read(reg, value);
}

esp_err_t TCA9555::write_pin(uint8_t pin, bool level) {
    if (pin > 15) {
        ESP_LOGE(TAG, "Invalid pin: %d", pin);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (pin >= 8) ? REG_OUTPUT_PORT1 : REG_OUTPUT_PORT0;
    uint8_t bit = pin % 8;

    uint8_t current;
    esp_err_t ret = read(reg, &current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read output port");
        return ret;
    }

    if (level) {
        current |= (1 << bit);
    } else {
        current &= ~(1 << bit);
    }

    ret = write(reg, current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write output port");
    }

    return ret;
}

esp_err_t TCA9555::write_port(uint8_t port, uint8_t value) {
    if (port > 1) {
        ESP_LOGE(TAG, "Invalid port: %d", port);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (port == 0) ? REG_OUTPUT_PORT0 : REG_OUTPUT_PORT1;
    return write(reg, value);
}

esp_err_t TCA9555::set_pin_polarity(uint8_t pin, bool invert) {
    if (pin > 15) {
        ESP_LOGE(TAG, "Invalid pin: %d", pin);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (pin >= 8) ? REG_POLARITY1 : REG_POLARITY0;
    uint8_t bit = pin % 8;

    uint8_t current;
    esp_err_t ret = read(reg, &current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read polarity register");
        return ret;
    }

    if (invert) {
        current |= (1 << bit);
    } else {
        current &= ~(1 << bit);
    }

    ret = write(reg, current);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write polarity register");
    }

    return ret;
}

esp_err_t TCA9555::set_port_polarity(uint8_t port, uint8_t invert_mask) {
    if (port > 1) {
        ESP_LOGE(TAG, "Invalid port: %d", port);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg = (port == 0) ? REG_POLARITY0 : REG_POLARITY1;
    return write(reg, invert_mask);
}
