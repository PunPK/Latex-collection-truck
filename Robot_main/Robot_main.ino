#include "motor.h"
#include "PS2X_lib.h"
#include "PS2_Controller.h"
#include "ultrasonic_sensor.h"
#include "servo_controller.h"

uint8_t ps2_data[6];
PS2_Status status_left = STOP;
PS2_Status status_right = STOP;
PS2_Status gripper_status = Release;

// PS2X library instance
PS2X ps2x;

// deadzone for analog stick
const uint8_t PS2_DEADZONE = 12;

const unsigned long PS2_POLL_INTERVAL_MS = 15;
const unsigned long DEBUG_PRINT_INTERVAL_MS = 250;

unsigned long last_ps2_poll_ms = 0;
unsigned long last_debug_print_ms = 0;

PS2_Status get_status_from_sticks(int x, int y, PS2_Status center_value)
{
    // map to PS2_Status
    if (x == 0 && y == 1) return BACKWARD;
    else if (x == 0 && y == -1) return FORWARD;
    else if (x == -1 && y == 0) return LEFT;
    else if (x == 1 && y == 0) return RIGHT;
    else if (x == -1 && y == 1) return BACKWARD_LEFT;
    else if (x == 1 && y == 1) return BACKWARD_RIGHT;
    else if (x == -1 && y == -1) return FORWARD_LEFT;
    else if (x == 1 && y == -1) return FORWARD_RIGHT;
    else return center_value;
}


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

void apply_arm_from_status(PS2_Status current)
{
    switch (current)
    {
    case FORWARD:
        arm_forward();
        break;
    case BACKWARD:
        arm_backward();
        break;
    case LEFT:
        arm_turn_left();
        break;
    case RIGHT:
        arm_turn_right();
        break;
    case FORWARD_LEFT:
        arm_turn_left();
        break;
    case FORWARD_RIGHT:
        arm_turn_right();
        break;
    case BACKWARD_LEFT:
        arm_turn_left();
        break;
    case BACKWARD_RIGHT:
        arm_turn_right();
        break;
    case Release:
        gripper_release();
        break;
    case Clamp:
        gripper_clamp();
        break;
    default:
        arm_stop();
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
    Serial.print(" LEFT :");
    switch (status_left)
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

    Serial.print("  | STATUS: ");
    Serial.print("  |  RIGHT :");
    switch (status_right)
    {
    case FORWARD: Serial.print("FORWARD"); break;
    case BACKWARD: Serial.print("BACKWARD"); break;
    case LEFT: Serial.print("LEFT"); break;
    case RIGHT: Serial.print("RIGHT"); break;
    case FORWARD_LEFT: Serial.print("FORWARD_LEFT"); break;
    case FORWARD_RIGHT: Serial.print("FORWARD_RIGHT"); break;
    case BACKWARD_LEFT: Serial.print("BACKWARD_LEFT"); break;
    case BACKWARD_RIGHT: Serial.print("BACKWARD_RIGHT"); break;
    default: Serial.print("CENTER"); break;
    }

    Serial.println();
}

void setup()
{
    Serial.begin(115200);

    motor_init();
    ps2x.config_gamepad(PS2_CLK_PIN, PS2_CMD_PIN, PS2_ATT_PIN, PS2_DAT_PIN, true, true);
    ultrasonic_init();
    servo_init();
}

void loop()
{
    ultrasonic_update();

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

        int x_right = 0;
        int y_right = 0;

        uint8_t analog_x_right = ps2x.Analog(PSS_RX);
        uint8_t analog_y_right = ps2x.Analog(PSS_RY);
        if (((uint8_t)abs((int)analog_x_right - 128) <= PS2_DEADZONE))
        {
            x_right = 0;
        }
        else if ((analog_x_right < 128))        
        {
            x_right = -1;
        }
        else
        {
            x_right = 1;
        }

        if (((uint8_t)abs((int)analog_y_right - 128) <= PS2_DEADZONE) )
        {
            y_right = 0;
        }
        else if ((analog_y_right > 128))
        {
            y_right = 1;
        }
        else
        {
            y_right = -1;
        }
        
        if (ps2x.Analog(PSS_RX) == 255 && ps2x.Analog(PSS_RY) == 255 && ps2x.Analog(PSS_LX) == 255 && ps2x.Analog(PSS_LY) == 255)
        {
            x = 0;
            y = 0;
            x_right = 0;
            y_right = 0;
        }

        status_left = get_status_from_sticks(x, y, STOP);
        apply_motor_from_status(status_left);

        status_right = get_status_from_sticks(x_right, y_right, STOP);
        apply_arm_from_status(status_right);

        if (ps2x.Button(PSB_SQUARE))
        {
            gripper_status = Clamp;
        }
        else if (ps2x.Button(PSB_CIRCLE))
        {
            gripper_status = Release;
        }
        apply_arm_from_status(gripper_status);
        
    }

    if (now - last_debug_print_ms >= DEBUG_PRINT_INTERVAL_MS)
    {
        last_debug_print_ms = now;
        print_debug();
    }
    
}