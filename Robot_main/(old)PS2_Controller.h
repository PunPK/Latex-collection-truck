#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <Arduino.h>

#include "robot_config.h"

typedef enum
{
    STOP = 0,

    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,

    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT

} PS2_Status;

void PS2_Init();
void PS2_ReadData(uint8_t *ps2_data);
void PS2_EnableAnalog();
PS2_Status PS2_GetStatus(uint8_t *ps2_data);

#endif