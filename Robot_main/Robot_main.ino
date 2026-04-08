#include "motor.h"
#include "PS2_Controller.h"
#include "ultrasonic_sensor.h"

uint8_t ps2_data[6];
PS2_Status status = STOP;

void setup()
{
    Serial.begin(9600);

    motor_init();
    PS2_Init();
    ultrasonic_init();
    PS2_EnableAnalog(); 
}

void loop()
{
    PS2_ReadData(ps2_data);
    status = PS2_GetStatus(ps2_data);

    switch (status)
    {
    case FORWARD:
        motor_forward();
        break;
    case BACKWARD:
        motor_backward();
        break;
    case LEFT:
        motor_turn_left();
        break;
    case RIGHT:
        motor_turn_right();
        break;
    case FORWARD_LEFT:
        motor_forward_left();
        break;
    case FORWARD_RIGHT:
        motor_forward_right();
        break;
    case BACKWARD_LEFT:
        motor_backward_left();
        break;
    case BACKWARD_RIGHT:
        motor_backward_right();
        break;
    default:
        motor_stop();
        break;
    }

    Serial.print("PS2: ");
    for (uint8_t i = 0; i < 6; i++)
    {
        Serial.print(ps2_data[i], HEX);
        Serial.print(' ');
    }
    Serial.print("| STATUS: ");

    switch (status)
    {
    case FORWARD:
        Serial.print("FORWARD");
        break;
    case BACKWARD:
        Serial.print("BACKWARD");
        break;
    case LEFT:
        Serial.print("LEFT");
        break;
    case RIGHT:
        Serial.print("RIGHT");
        break;
    case FORWARD_LEFT:
        Serial.print("FORWARD_LEFT");
        break;
    case FORWARD_RIGHT:
        Serial.print("FORWARD_RIGHT");
        break;
    case BACKWARD_LEFT:
        Serial.print("BACKWARD_LEFT");
        break;
    case BACKWARD_RIGHT:
        Serial.print("BACKWARD_RIGHT");
        break;
    default:
        Serial.print("STOP");
        break;
    }

    Serial.println();

    ultrasonic_update();
}