// ssd1306.h
#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Define the I2C configuration and SSD1306's I2C address
// #define I2C_PORT i2c0
// #define SDA_PIN 4
// #define SCL_PIN 5
// #define SSD1306_ADDRESS 0x3C

typedef struct {
    i2c_inst_t *i2c_port;
    uint8_t i2c_address;
    uint8_t width;
    uint8_t height;
} ssd1306_config_t;

// Function declarations
void ssd1306_init(const ssd1306_config_t *config);
void ssd1306_deinit(void);
void send_command(const uint8_t *commands, size_t length);
void send_data(const uint8_t *data, size_t length);
void set_page_column(uint8_t page, uint8_t column);
void clear_display(void);
void write_char(char c, uint8_t page, uint8_t column);
void write_string(const char *str, uint8_t page, uint8_t column);

#endif // SSD1306_H