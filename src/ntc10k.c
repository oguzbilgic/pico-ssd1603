/**
 * @file ntc10k.c
 * @brief Implementation of the NTC10K temperature sensor module.
 *
 * This module provides functions to initialize the NTC10K temperature sensor,
 * read the temperature from the sensor, and calculate the moving average of
 * temperature readings.
 */

#include <math.h>
#include <stdio.h>

#include "pico/time.h"

#include "hardware/adc.h"

#include "moving_average.h"

#define BETA 3950  // Beta value - change this to your thermistor's Beta value
#define R0 10000   // Resistance of the thermistor at 25 degrees C (10kΩ)
#define R1 10000.0f   // Pull up resistor value (10kΩ)
#define T0 298.15  // Reference temperature in Kelvin (25°C)

// Flag to check if the module has been initialized
bool is_initialized = false;
// Moving average filters for each ADC channel
moving_average moving_averages[3];
// Timer to read the sensors every 250ms
repeating_timer_t timer;

bool timer_callback(struct repeating_timer *t);

/**
 * @brief Initializes the NTC10K temperature sensor module.
 *
 * This function initializes the ADC, sets up the moving average filters for each
 * ADC channel, and starts a repeating timer to read the sensors every 250ms.
 */
void ntc10k_init() {
    adc_init();

    for(int adc_channel = 0; adc_channel < 3; adc_channel++) {
        init_moving_average(&moving_averages[adc_channel]);
    }

    add_repeating_timer_ms(250, timer_callback, NULL, &timer);

    // Call the timer callback function once to read the sensors
    timer_callback(NULL);

    is_initialized = true;
}

/**
 * @brief Reads the temperature from the NTC10K temperature sensor.
 *
 * @param adc_channel The ADC channel to read the temperature from.
 * @return The temperature in Celsius.
 */
float ntc10k_read_sensor(int adc_channel) {
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

/**
 * @brief Reads the moving average temperature from the NTC10K temperature sensor.
 *
 * This function returns the moving average temperature reading for the specified
 * ADC channel. If the module has not been initialized, it will be initialized
 * before reading the temperature.
 *
 * @param adc_channel The ADC channel to read the temperature from.
 * @return The moving average temperature in Celsius.
 */
float ntc10k_read_temperature(int adc_channel) {
    if (!is_initialized) {
        ntc10k_init();
    }

    return get_moving_average(&moving_averages[adc_channel]);
}

/**
 * @brief Timer callback function to read the temperature sensors.
 *
 * This function is called by the repeating timer every 250ms to read the temperature
 * from each ADC channel and update the moving average filters.
 *
 * @param t The repeating timer structure.
 * @return Always returns true.
 */
bool timer_callback(struct repeating_timer *t) {
    for (int adc_channel = 0; adc_channel < 3; adc_channel++) {
        float temperature = ntc10k_read_sensor(adc_channel);

        update_moving_average(&moving_averages[adc_channel], temperature);
    }

    return true;
}