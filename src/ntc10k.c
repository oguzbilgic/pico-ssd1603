/**
 * @file ntc10k.c
 * @brief Library for reading temperature from a NTC 10k thermistor using ADC.
 * 
 * This library provides functions to initialize the temperature sensor and read the temperature
 * from the specified ADC channel. It uses the Steinhart-Hart equation to calculate the temperature
 * based on the resistance measured by the ADC.
 * 
 * The library assumes that the thermistor is connected to the specified GPIO pins and uses a
 * pull-up resistor of 10kΩ. The Beta value and the resistance at 25 degrees Celsius (R0) can be
 * adjusted according to the thermistor's specifications.
 * 
 * @note This library requires the Pico SDK and the pico_stdlib library.
 */

#include <math.h>

#include "hardware/adc.h"

#define BETA 3950  // Beta value - change this to your thermistor's Beta value
#define R0 10000   // Resistance of the thermistor at 25 degrees C (10kΩ)
#define R1 10000.0f   // Pull up resistor value (10kΩ)
#define T0 298.15  // Reference temperature in Kelvin (25°C)

bool is_initialized = false;

/**
 * @brief Read the temperature from the specified ADC channel.
 * 
 * @param adc_channel The ADC channel ID (0, 1, or 2).
 * @return The temperature in Celsius.
 */
float ntc10k_read_temperature(int adc_channel) {
    if (!is_initialized) {
        adc_init();
        is_initialized = true;
    }

    // ADC channels start from GPIO 26
    adc_select_input(adc_channel); 

    uint16_t adc_value = adc_read();
    float R = (R1 * adc_value) / (4095 - adc_value);

    // Calculate temperature in Kelvin
    float temperature_k = BETA / (log(R / R0) + (BETA / T0));

    // Convert Kelvin to Celsius
    float temperature_c = temperature_k - 273.15;

    return temperature_c;
}