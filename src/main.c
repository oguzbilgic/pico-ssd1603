#include <stdio.h>

#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "adc.h"
#include "button.h"
#include "ssd1306.h"
#include "ntc10k.h"

typedef enum {
    INFO,
    TEMPERATURES
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

    sprintf(str, "CPU FREQ: %dMHZ", clock_get_hz(clk_sys) / 1000000);
    write_string(str, 4, 0);

    sprintf(str, "CORE TEMP: %.1f", read_onboard_temp());
    write_string(str, 5, 0);

    sprintf(str, "VOLTAGE: %.3f", read_voltage());
    write_string(str, 6, 0);

    sprintf(str, "I2C IRQ: %d", irq_is_enabled(5));
    write_string(str, 7, 0);
}

void render_temperature() {
    char str[100];

    write_string("     TEMPERATURES", 0, 0);

    sprintf(str, "ROOM:   %.1f", ntc10k_read_temperature(2));
    write_string(str, 2, 0);

    sprintf(str, "CASE:   %.1f", ntc10k_read_temperature(1));
    write_string(str, 3, 0);

    sprintf(str, "WATER: %.1f", ntc10k_read_temperature(0));
    write_string(str, 4, 0);
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
        state.screen = state.screen == INFO ? TEMPERATURES : INFO;
        state.force_clean = true;
        break;
    }

    printf("GPIO %d event %d\n", gpio, events);
}

void main() {
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

    while(1) {
        absolute_time_t start = get_absolute_time();

        if (state.force_clean) {
            clear_display();
            state.force_clean = false;
        }
        
        if (state.screen == INFO) {
            render_info();
        } else {
            render_temperature();
        }

        // printf("Display frequency: %lldHz\n", 1000 / (absolute_time_diff_us(start, get_absolute_time()) / 1000));
        printf("Render time: %lldms\n", absolute_time_diff_us(start, get_absolute_time()) / 1000);

        sleep_ms(100);
    }

    return 0;
}