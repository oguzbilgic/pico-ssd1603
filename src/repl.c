#include "repl.h"
#include "pico/stdlib.h"
#include "tusb.h" // Include for TinyUSB functionality

#define MAX_COMMANDS 10
#define MAX_COMMAND_LENGTH 256

static Command commands[MAX_COMMANDS];
static int command_count = 0;
static char command_buffer[MAX_COMMAND_LENGTH];
static int command_buffer_index = 0;
static bool was_connected = false;

void repl_init(void) {
    stdio_init_all(); // Initialize stdio which includes USB serial
    command_buffer_index = 0;
}

bool repl_register_command(const char* command, CommandCallback callback) {
    if (command_count < MAX_COMMANDS) {
        commands[command_count].command = command;
        commands[command_count].callback = callback;
        command_count++;
        return true;
    }
    return false;
}

void repl_execute_non_blocking(void) {
    // Check for USB CDC connection and print welcome message if newly connected
    if (tud_cdc_connected()) {
        if (!was_connected) {
            // New connection established
            printf("Welcome to the Pico REPL!\n");
            printf("> "); // Prompt for the first command
            was_connected = true;
        }

        if (tud_cdc_available()) {
            char ch = tud_cdc_read_char();

            // Echo the character back (optional)
            putchar(ch);

            if (ch == '\n' || ch == '\r' || command_buffer_index >= MAX_COMMAND_LENGTH - 1) {
                command_buffer[command_buffer_index] = '\0'; // Null-terminate the string
                putchar('\n'); // Print newline for clarity

                // Process the command
                for (int i = 0; i < command_count; i++) {
                    if (strcmp(command_buffer, commands[i].command) == 0) {
                        commands[i].callback();
                        break;
                    }
                }

                // Reset the buffer for the next command
                printf("> "); // Prompt for the next command
                command_buffer_index = 0;
            } else {
                // Store the character in the buffer
                command_buffer[command_buffer_index++] = ch;
            }
        }
    } else {
        was_connected = false;
        command_buffer_index = 0; // Reset the buffer index if disconnected
    }
}
