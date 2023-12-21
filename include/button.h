#ifndef BUTTON_H
#define BUTTON_H

#include "hardware/gpio.h"

void button_init(int pins[], int num_pins, gpio_irq_callback_t *callback);

#endif // BUTTON_H