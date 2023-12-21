// ssd1306.c
#include "ssd1306.h"

// Basic 5x7 font
const uint8_t font[][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x00, 0x00, 0x60, 0x60, 0x00}, // .
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E} // @
    // Add more characters as needed
};

const uint8_t init_commands[] = {
    // 0xAE,             // Display off
    // 0xD5, 0x80,       // Set display clock divide ratio/oscillator frequency
    // 0xA8, 0x3F,       // Set multiplex ratio (1 to 64) - adjust if your display is 32 or 64 pixels high
    // 0xD3, 0x00,       // Set display offset
    // 0x40,             // Set start line address
    // 0x8D, 0x14,       // Enable charge pump regulator
    // 0x20, 0x00,       // Set Memory Addressing Mode to Horizontal Addressing Mode
    0xA1,             // Set segment re-map
    0xC8,             // Set COM Output Scan Direction
    // 0xDA, 0x12,       // Set COM Pins hardware configuration - adjust if needed
    // 0x81, 0xCF,       // Set contrast control
    // 0xD9, 0xF1,       // Set pre-charge period
    // 0xDB, 0x40,       // Set VCOMH deselect level
    // 0xA4,             // Disable entire display on
    // 0xA6,             // Disable inverse display on
    // 0x2E,             // Deactivate scroll
    0xAF              // Turn on SSD1306 panel
    // You can add more commands as needed for your specific application
};

/**
 * Holds the current configuration for the SSD1306 display.
 */
static ssd1306_config_t ssd1306_config;

// Default configuration values
static const ssd1306_config_t DEFAULT_SSD1306_CONFIG = {
    .i2c_port = i2c0,
    .i2c_address = 0x3C,
    .width = 128,
    .height = 64
};

ssd1306_config_t ssd1306_get_default_config() {
    return DEFAULT_SSD1306_CONFIG;
}

void ssd1306_init(const ssd1306_config_t *config) {
    ssd1306_config = *config;

    clear_display();

    // Send initialization sequence to SSD1306
    send_command(init_commands, sizeof(init_commands));
}

void ssd1306_deinit(void) {
    // Command to turn off the SSD1306 display
    const uint8_t display_off_command[] = {0xAE};

    send_command(display_off_command, sizeof(display_off_command));
}

void send_command(const uint8_t *commands, size_t length) {
    uint8_t buffer[length + 1];
    buffer[0] = 0x00; // Control byte for command

    for (size_t i = 0; i < length; i++) {
        buffer[i + 1] = commands[i];
    }

    return i2c_write_blocking(ssd1306_config.i2c_port, ssd1306_config.i2c_address, buffer, length + 1, false);
}

void send_data(const uint8_t *data, size_t length) {
    uint8_t buffer[length + 1];
    buffer[0] = 0x40; // Control byte for data
    
    for (size_t i = 0; i < length; i++) {
        buffer[i + 1] = data[i];
    }

    i2c_write_blocking(ssd1306_config.i2c_port, ssd1306_config.i2c_address, buffer, length + 1, false);
}

void set_page_column(uint8_t page, uint8_t column) {
    uint8_t commands[] = {
        0xB0 | page,                  // Set page address
        0x00 | (column & 0x0F) + 2,       // Set lower column address
        0x10 | ((column >> 4) & 0x0F) // Set higher column address
    };
    send_command(commands, sizeof(commands));
}

void clear_display(void) {
    uint8_t clear_data[ssd1306_config.width]; // Buffer to hold the clear data for one entire page
    memset(clear_data, 0x00, ssd1306_config.width); // Set all bytes in the buffer to 0

    for (uint8_t page = 0; page < 8; page++) {
        set_page_column(page, 0); // Set the current page and start from the first column
        send_data(clear_data, ssd1306_config.width); // Send the clear data to clear the entire page
    }
}

void write_char(char c, uint8_t page, uint8_t column) {
    int index = 0;
    if (c >= 'A' && c <= 'Z') {
        index = c - 'A';
    } else if (c >= '0' && c <= '9') {
        index = c - '0' + 26; // numbers start from 26th index
    } else if (c == ':') {
        index = 36; // ':' is at 36th index
    } else if (c == ';') {
        index = 37; // ';' is at 37th index
    } else if (c == '<') {
        index = 38; // '<' is at 38th index
    }
    else if (c == '.') {
        index = 39; // '.' is at 39th index
    } else if (c == ' ') {
        uint8_t space[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
        set_page_column(page, column);
        send_data(space, 5);
        return;
    } else {
        return; // unsupported character
    }

    set_page_column(page, column);
    send_data(font[index], 5);
}

void write_string(const char *str, uint8_t page, uint8_t column) {
    while (*str) {
        write_char(*str, page, column);

        column += 6; // 5 pixels per character + 1 pixel space
        if (column > 127) {
            column = 0;
            page++;
        }
        str++;
    }
}
