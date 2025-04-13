#ifndef REPL_H
#define REPL_H

#include <stdbool.h>

// Type definition for the callback function associated with a command
typedef void (*CommandCallback)(void);

// Structure representing a command and its callback
typedef struct {
    const char* command;
    CommandCallback callback;
} Command;

// Function to initialize the REPL
void repl_init(void);

// Function to register a new command with the REPL
bool repl_register_command(const char* command, CommandCallback callback);

// The non-blocking REPL function to be called in the main loop
void repl_execute_non_blocking(void);

#endif // REPL_H