#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <stdbool.h>

#define MOVING_AVERAGE_SIZE 20

typedef struct {
    float readings[MOVING_AVERAGE_SIZE];
    int index;
    bool initialized;
} moving_average;

void init_moving_average(moving_average *avg);
float get_moving_average(moving_average *avg);
float update_moving_average(moving_average *avg, float new_reading);

#endif // MOVING_AVERAGE_H