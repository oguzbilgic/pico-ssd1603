#ifndef NTC10K_H
#define NTC10K_H

// Initialize the temperature sensor
void ntc10k_init();

// Read the temperature from the specified ADC channel
float ntc10k_read_temperature(int adc_channel);

#endif // NTC10K_H