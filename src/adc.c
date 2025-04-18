#include "hardware/adc.h"

float adc_read_voltage(adc_channel) {
    adc_init();

    if (adc_channel == 4) {
        // Select the onboard temperature sensor
        adc_set_temp_sensor_enabled(true);
    }

    adc_select_input(adc_channel);

    const float conversion_factor = 3.225f / (1 << 12); // 12-bit ADC, 3.3V reference voltage
    uint16_t result = adc_read();

    // Convert the result to voltage
    return result * conversion_factor;
}

float read_onboard_temp() {
    adc_init();

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