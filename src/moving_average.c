#include "moving_average.h"

void init_moving_average(moving_average *avg) {
    for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
        avg->readings[i] = 0.0f;
    }

    avg->index = 0;
    avg->initialized = false;
}

float get_moving_average(moving_average *avg) {
    float sum = 0.0f;
    for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
        sum += avg->readings[i];
    }

    return sum / MOVING_AVERAGE_SIZE;
}

float update_moving_average(moving_average *avg, float new_reading) {
    if (!avg->initialized) {
        for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
            avg->readings[i] = new_reading;
        }
        avg->initialized = true;
    }

    avg->readings[avg->index] = new_reading;
    avg->index = (avg->index + 1) % MOVING_AVERAGE_SIZE;

    return get_moving_average(avg);
}
