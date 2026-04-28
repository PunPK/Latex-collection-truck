#include "motor.h"
#include "PS2X_lib.h"
#include "PS2_Controller.h"
#include "ultrasonic_sensor.h"
#include "servo_controller.h"

uint8_t ps2_data[6];
PS2_Status status = STOP;

// PS2X library instance
PS2X ps2x;

// deadzone for analog stick
const uint8_t PS2_DEADZONE = 12;

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
    // Print raw analog values and D-pad/button states from PS2X
    Serial.print("ANALOG RX:");
    Serial.print(ps2x.Analog(PSS_RX));
    Serial.print("  LX:");
    Serial.print(ps2x.Analog(PSS_LX));
    Serial.print("  RY:");
    Serial.print(ps2x.Analog(PSS_RY));
    Serial.print("  LY:");
    Serial.print(ps2x.Analog(PSS_LY));

    Serial.print("  | D-PAD U:" ); Serial.print(ps2x.Button(PSB_PAD_UP) ? 1 : 0);
    Serial.print(" D:" ); Serial.print(ps2x.Button(PSB_PAD_DOWN) ? 1 : 0);
    Serial.print(" L:" ); Serial.print(ps2x.Button(PSB_PAD_LEFT) ? 1 : 0);
    Serial.print(" R:" ); Serial.print(ps2x.Button(PSB_PAD_RIGHT) ? 1 : 0);

    // Right-side face buttons: Triangle, Square, Circle, Cross
    Serial.print("  | TRI:" ); Serial.print(ps2x.Button(PSB_TRIANGLE) ? 1 : 0);
    Serial.print(" SQ:" ); Serial.print(ps2x.Button(PSB_SQUARE) ? 1 : 0);
    Serial.print(" CIR:" ); Serial.print(ps2x.Button(PSB_CIRCLE) ? 1 : 0);
    Serial.print(" X:" ); Serial.print(ps2x.Button(PSB_CROSS) ? 1 : 0);

    Serial.print("  | STATUS: ");
    switch (status)
    {
    case FORWARD: Serial.print("FORWARD"); break;
    case BACKWARD: Serial.print("BACKWARD"); break;
    case LEFT: Serial.print("LEFT"); break;
    case RIGHT: Serial.print("RIGHT"); break;
    case FORWARD_LEFT: Serial.print("FORWARD_LEFT"); break;
    case FORWARD_RIGHT: Serial.print("FORWARD_RIGHT"); break;
    case BACKWARD_LEFT: Serial.print("BACKWARD_LEFT"); break;
    case BACKWARD_RIGHT: Serial.print("BACKWARD_RIGHT"); break;
    default: Serial.print("STOP"); break;
    }

    Serial.println();
}

void setup()
{
    Serial.begin(9600);

    motor_init();
    // configure PS2X with pins from robot_config.h
    // order: clock, command, attention, data
    ps2x.config_gamepad(PS2_CLK_PIN, PS2_CMD_PIN, PS2_ATT_PIN, PS2_DAT_PIN, true, true);
    ultrasonic_init();
    // PS2X handles enabling analog/pressures in config above
    servo_init();
}

void loop()
{
    // ultrasonic_update();

    unsigned long now = millis();

    if (now - last_ps2_poll_ms >= PS2_POLL_INTERVAL_MS)
    {
        last_ps2_poll_ms = now;
        ps2x.read_gamepad();

        int x = 0;
        int y = 0;

        uint8_t analog_x = ps2x.Analog(PSS_LX);
        uint8_t analog_y = ps2x.Analog(PSS_LY);
        if (((uint8_t)abs((int)analog_x - 128) <= PS2_DEADZONE) && !ps2x.Button(PSB_PAD_LEFT) && !ps2x.Button(PSB_PAD_RIGHT))
        {
            x = 0;
        }
        else if ((analog_x < 128) || ps2x.Button(PSB_PAD_LEFT))        
        {
            x = -1;
        }
        else
        {
            x = 1;
        }

        if (((uint8_t)abs((int)analog_y - 128) <= PS2_DEADZONE) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_DOWN))
        {
            y = 0;
        }
        else if ((analog_y > 128) || ps2x.Button(PSB_PAD_DOWN))
        {
            y = 1;
        }
        else
        {
            y = -1;
        }

        // map to PS2_Status
        if (x == 0 && y == 1) status = BACKWARD;
        else if (x == 0 && y == -1) status = FORWARD;
        else if (x == -1 && y == 0) status = LEFT;
        else if (x == 1 && y == 0) status = RIGHT;
        else if (x == -1 && y == 1) status =  BACKWARD_LEFT;
        else if (x == 1 && y == 1) status = BACKWARD_RIGHT;
        else if (x == -1 && y == -1) status = FORWARD_LEFT;
        else if (x == 1 && y == -1) status = FORWARD_RIGHT;
        else status = STOP;

        apply_motor_from_status(status);
    }

    if (now - last_debug_print_ms >= DEBUG_PRINT_INTERVAL_MS)
    {
        last_debug_print_ms = now;
        print_debug();
    }
    
}