#ifndef TCA9555_H
#define TCA9555_H

#include "driver/i2c.h"
#include "esp_err.h"

class TCA9555 {
public:
    /**
     * Create a TCA9555 driver instance.
     *
     * @param i2c_num ESP-IDF I2C controller number.
     * @param addr TCA9555 I2C address, usually 0x20 when A2/A1/A0 are tied to GND.
     */
    TCA9555(i2c_port_t i2c_num = I2C_NUM_0, uint8_t addr = 0x20);

    /**
     * Configure and install the I2C master driver used by the TCA9555.
     *
     * @param sda_pin SDA GPIO.
     * @param scl_pin SCL GPIO.
     * @param freq I2C clock frequency in Hz.
     * @return ESP_OK on success, otherwise an ESP-IDF error code.
     */
    esp_err_t init(gpio_num_t sda_pin, gpio_num_t scl_pin, uint32_t freq = 400000);

    /**
     * Read one byte from a TCA9555 register.
     *
     * @param reg Register address.
     * @param value Output byte pointer.
     * @return ESP_OK on success.
     */
    esp_err_t read(uint8_t reg, uint8_t *value);

    /**
     * Write one byte to a TCA9555 register.
     *
     * @param reg Register address.
     * @param value Byte to write.
     * @return ESP_OK on success.
     */
    esp_err_t write(uint8_t reg, uint8_t value);

    /**
     * Configure one TCA9555 pin as input or output.
     *
     * @param pin Pin index 0-15. P00-P07 map to 0-7, P10-P17 map to 8-15.
     * @param is_input true for input, false for output.
     * @return ESP_OK on success.
     */
    esp_err_t set_pin_direction(uint8_t pin, bool is_input);

    /**
     * Configure all 8 pins in one TCA9555 port.
     *
     * @param port Port index: 0 for P00-P07, 1 for P10-P17.
     * @param dir_mask Direction bit mask. 1=input, 0=output.
     * @return ESP_OK on success.
     */
    esp_err_t set_port_direction(uint8_t port, uint8_t dir_mask);

    /**
     * Read one TCA9555 input pin.
     *
     * @param pin Pin index 0-15.
     * @return true for high level, false for low level or read failure.
     */
    bool read_pin(uint8_t pin);

    /**
     * Read one full TCA9555 input port.
     *
     * @param port Port index: 0 for P00-P07, 1 for P10-P17.
     * @param value Output byte pointer.
     * @return ESP_OK on success.
     */
    esp_err_t read_port(uint8_t port, uint8_t *value);

    /**
     * Write one TCA9555 output pin.
     *
     * @param pin Pin index 0-15.
     * @param level true for high level, false for low level.
     * @return ESP_OK on success.
     */
    esp_err_t write_pin(uint8_t pin, bool level);

    /**
     * Write all 8 output bits in one TCA9555 port.
     *
     * @param port Port index: 0 for P00-P07, 1 for P10-P17.
     * @param value Output byte.
     * @return ESP_OK on success.
     */
    esp_err_t write_port(uint8_t port, uint8_t value);

    /**
     * Enable or disable polarity inversion for one input pin.
     *
     * @param pin Pin index 0-15.
     * @param invert true to invert the read value.
     * @return ESP_OK on success.
     */
    esp_err_t set_pin_polarity(uint8_t pin, bool invert);

    /**
     * Configure polarity inversion for all 8 pins in one port.
     *
     * @param port Port index: 0 for P00-P07, 1 for P10-P17.
     * @param invert_mask Inversion bit mask. 1=invert, 0=normal.
     * @return ESP_OK on success.
     */
    esp_err_t set_port_polarity(uint8_t port, uint8_t invert_mask);

    static const uint8_t REG_INPUT_PORT0   = 0x00;
    static const uint8_t REG_INPUT_PORT1   = 0x01;
    static const uint8_t REG_OUTPUT_PORT0  = 0x02;
    static const uint8_t REG_OUTPUT_PORT1  = 0x03;
    static const uint8_t REG_POLARITY0     = 0x04;
    static const uint8_t REG_POLARITY1     = 0x05;
    static const uint8_t REG_CONFIG0       = 0x06;
    static const uint8_t REG_CONFIG1       = 0x07;

private:
    i2c_port_t _i2c_num;
    uint8_t _addr;
};

#endif
