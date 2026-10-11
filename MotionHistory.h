#ifndef MOTION_HISTORY_H
#define MOTION_HISTORY_H

#include <Arduino.h>

#define HISTORY_SIZE 10

extern const float CPR;
extern const float OUTPUT_CPR;
extern const float GEAR_RATIO;

void printLastMovement(uint8_t motor);

void saveMovement(
    uint8_t motor,
    long pulses);

void printHistory(uint8_t motor);

#endif