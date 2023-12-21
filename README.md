# SSD1306 Driver for Raspberry Pi Pico

This SSD1306 library is designed for interfacing with an SSD1306 OLED display using the Raspberry Pi Pico. It simplifies the process of initializing the display, rendering text, and managing display properties. The library is written in C and is tailored for use with the Raspberry Pi Pico SDK, making it suitable for a wide range of embedded projects that require display capabilities.

## Features

* Display Initialization: Configure and initialize the SSD1306 display with custom settings.
* Text Rendering: Display text using a built-in 5x7 font.
* Display Control: Clear the screen, adjust contrast, and set display orientation.
* Separation of low-level hardware interaction (HAL) and high-level display functions.
* Supports different I2C addresses and screen sizes.
* Utilizes I2C protocol for communication, optimizing for speed and resource usage.

## Installation

### Add as a Git Submodule:

```sh
cd external/
git submodule add https://github.com/oguzbilgic/pico-ssd1603
git submodule update --init --recursive
```

### Integrate with CMake:

Add the SSD1306 library to your CMakeLists.txt:

```cmake
add_subdirectory(external/ssd1306)
target_link_libraries(your_executable PRIVATE ssd1306)
```

## Usage

To use the library, include the header file and initialize the display with your desired configuration:

```c
#include "ssd1306.h"

int main() {
    ssd1306_config_t config = {
        .i2c_port = i2c0,
        .i2c_address = 0x3C,
        .width = 128,
        .height = 64
    };

    ssd1306_init(&config);
    ssd1306_write_string("Hello, World!", 0, 0);
    // Additional display operations...
}
```