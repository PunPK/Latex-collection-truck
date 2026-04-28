#include "motor.h"
#include "PS2_Controller.h"
#include "ultrasonic_sensor.h"
#include "servo_controller.h"

uint8_t ps2_data[6];
PS2_Status status = STOP;

const unsigned long PS2_POLL_INTERVAL_MS = 15;
const unsigned long DEBUG_PRINT_INTERVAL_MS = 120;

unsigned long last_ps2_poll_ms = 0;
unsigned long last_debug_print_ms = 0;

void apply_motor_from_status(PS2_Status current)
{
    switch (current)
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
}

void print_debug()
{
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
}

void setup()
{
    Serial.begin(9600);

    motor_init();
    PS2_Init();
    ultrasonic_init();
    PS2_EnableAnalog();
    servo_init();
}

void loop()
{
    ultrasonic_update();

    unsigned long now = millis();

    if (now - last_ps2_poll_ms >= PS2_POLL_INTERVAL_MS)
    {
        last_ps2_poll_ms = now;
        PS2_ReadData(ps2_data);
        status = PS2_GetStatus(ps2_data);
        apply_motor_from_status(status);
    }

    if (now - last_debug_print_ms >= DEBUG_PRINT_INTERVAL_MS)
    {
        last_debug_print_ms = now;
        print_debug();
    }
}