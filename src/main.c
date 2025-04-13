#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "adc.h"
#include "repl.h"
#include "button.h"
#include "ssd1306.h"
#include "ntc10k.h"

typedef enum {
    TEMP,
    ADC,
    INFO,
} screen_t;

typedef struct {
    screen_t screen;
    bool force_clean;
    absolute_time_t start_time;
    int counter;
} state_t;

volatile state_t state;

state_t get_initial_state() {
    state_t state = {
        .screen = INFO,
        .force_clean = false,
        .start_time = get_absolute_time(),
        .counter = 0
    };

    return state;
}

void render_info() {
    char str[100];

    write_string("        INFO", 0, 0);

    int64_t uptime = absolute_time_diff_us(state.start_time, get_absolute_time()) / 1000000;
    sprintf(str, "UPTIME: %d SEC", uptime);
    write_string(str, 2, 0);

    write_string("CHIP: RP2040", 3, 0);

    sprintf(str, "CORE TEMP: %.1f", read_onboard_temp());
    write_string(str, 4, 0);

    sleep_ms(250);
}

void render_temperature() {
    char str[100];

    write_string("     TEMPERATURES", 0, 0);

    sprintf(str, "ROOM:   %.1f", ntc10k_read_temperature(2));
    write_string(str, 2, 0);

    sprintf(str, "CASE:   %.1f %+0.1f", ntc10k_read_temperature(1), ntc10k_read_temperature(1) - ntc10k_read_temperature(2));
    write_string(str, 3, 0);

    sprintf(str, "WATER:  %.1f %+0.1f", ntc10k_read_temperature(0), ntc10k_read_temperature(0) - ntc10k_read_temperature(2));
    write_string(str, 4, 0);
}

void render_adc() {
    char str[100];

    write_string("       VOLTAGES", 0, 0);

    for (int i = 0; i < 4; i++) {
        sprintf(str, "ADC%d:   %.3fV", i, adc_read_voltage(i));
        write_string(str, i + 2, 0);
    }
}


void gpio_callback(uint gpio, uint32_t events) {
    switch (gpio) {
    case 20:
        // ssd1306_start_horizontal_scroll(true, 3, 5, 0x03);
        break;
    case 21:
        gpio_put(25, !gpio_get(25));
        break;
    case 22:
        if (state.screen == INFO) {
            state.screen = 0; 
        } else {
            state.screen++;
        }
        state.force_clean = true;
        break;
    }

    // print screen 
    // printf("Screen: %d\n", state.screen);

    // printf("GPIO %d event %d\n", gpio, events);
}

void render_display() {
    // absolute_time_t start = get_absolute_time();

    if (state.force_clean) {
        clear_display();
        state.force_clean = false;
    }

    switch(state.screen) {
    case TEMP:
        render_temperature();
        break;
    case ADC:
        render_adc();
        break;
    case INFO:
        render_info();
        break;
    }

    // printf("Display frequency: %lldHz\n", 1000 / (absolute_time_diff_us(start, get_absolute_time()) / 1000));
    // printf("Render time: %lldms\n", absolute_time_diff_us(start, get_absolute_time()) / 1000);
}

void command_help() {
    printf("Available commands:\n");
}

int main() {
    state = get_initial_state();

    stdio_init_all();

    // Initialize I2C bus 0
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);

    // Initialize onboard LED
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // Initialize buttons with interrupts
    int button_pins[] = {20, 21, 22};
    button_init(button_pins, 3, gpio_callback);

    // Initialize SSD1306 display with default configuration
    ssd1306_config_t ssd1306_config = ssd1306_get_default_config();
    ssd1306_init(&ssd1306_config);

    // repl_init();
    // repl_register_command("help", command_help);

    while(1) {
        render_display();
        // repl_execute_non_blocking();
    }

    return 0;
}
