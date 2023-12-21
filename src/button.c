#include "hardware/gpio.h"

void button_init(int pins[], int num_pins, gpio_irq_callback_t callback) {
    for (int i = 0; i < num_pins; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
        gpio_set_irq_enabled_with_callback(pins[i], GPIO_IRQ_EDGE_RISE, true, callback);
    }
}
