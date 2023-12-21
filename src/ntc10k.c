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

// #include "pico/stdlib.h"

#include "hardware/adc.h"

#define ADC_PINS (int[]){26, 27, 28} // Change this to the GPIO number connected to the thermistor
#define ADC_PINS_COUNT 3
#define BETA 3950  // Beta value - change this to your thermistor's Beta value
#define R0 10000   // Resistance of the thermistor at 25 degrees C (10kΩ)
#define R1 10000.0f   // Pull up resistor value (10kΩ)
#define T0 298.15  // Reference temperature in Kelvin (25°C)

/**
 * @brief Initializes the temperature sensor.
 * 
 * This function initializes the temperature sensor by initializing the ADC and selecting the input channels.
 * It also sets the ADC voltage reference to 3.3V.
 */
void ntc10k_init() {
    adc_init();

    // loop over adc_PINS and select each one
    for (int i = 0; i < ADC_PINS_COUNT; i++) {
        adc_select_input(ADC_PINS[i]);
    }
}

/**
 * @brief Read the temperature from the specified ADC channel.
 * 
 * @param adc_channel The ADC channel ID (0, 1, or 2).
 * @return The temperature in Celsius.
 */
float ntc10k_read_temperature(int adc_channel) {
    adc_set_temp_sensor_enabled(true);

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