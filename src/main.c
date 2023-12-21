#include <stdio.h>

#include "pico/stdlib.h"

#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

#include "ssd1306.h"
#include "ntc10k.h"

float read_onboard_temp() {
    // Select the onboard temperature sensor
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // ADC input 4 is typically reserved for the temperature sensor

    const float conversion_factor = 3.3f / (1 << 12); // 12-bit ADC, 3.3V reference voltage
    uint16_t result = adc_read();

    // Convert the result to voltage
    float voltage = result * conversion_factor;

    // Convert voltage to temperature
    // According to the RP2040 datasheet, the temperature sensor has an output of 0.706V at 0°C
    // with a slope of 1.721mV/°C
    return (27 - ((voltage - 0.706) / 0.001721));
}

float read_voltage() {
    // Select the onboard temperature sensor
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // ADC input 4 is typically reserved for the temperature sensor

    const float conversion_factor = 3.3f / (1 << 12); // 12-bit ADC, 3.3V reference voltage
    uint16_t result = adc_read();

    // Convert the result to voltage
    return result * conversion_factor;
}

int main() {
    stdio_init_all();

    // initialize ntc10k
    ntc10k_init();

    // Initialize I2C
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);

    // Initialize SSD1306 display
    ssd1306_config_t ssd1306_config = ssd1306_get_default_config();
    ssd1306_init(&ssd1306_config);

    int counter = 0;
    char str[100];
    while(1) {
        write_string("BOARD: PI PICO RP2040", 0, 0);

        sprintf(str, "CPU FREQ: %dMHZ", clock_get_hz(clk_sys) / 1000000);
        write_string(str, 1, 0);

        sprintf(str, "TEMP 1: %.1f", ntc10k_read_temperature(2));
        write_string(str, 2, 0);

        sprintf(str, "TEMP 2: %.1f", ntc10k_read_temperature(1));
        write_string(str, 3, 0);

        sprintf(str, "TEMP 3: %.1f", ntc10k_read_temperature(0));
        write_string(str, 4, 0);

        sprintf(str, "CORE TEMP: %.1f", read_onboard_temp());
        write_string(str, 5, 0);

        sprintf(str, "VOLTAGE: %.3f", read_voltage());
        write_string(str, 6, 0);

        sprintf(str, "UPTIME: %d SEC", counter);
        write_string(str, 7, 0);

        sleep_ms(1000);
        counter++;
    }

    return 0;
}