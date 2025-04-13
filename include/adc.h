#ifndef ADC_H
#define ADC_H

#include <stdint.h>

float adc_read_voltage(int adc_channel);

// Function to read the onboard temperature
float read_onboard_temp();

// Function to read the voltage
float read_voltage();

#endif // ADC_H