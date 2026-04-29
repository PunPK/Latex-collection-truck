#include "motor.h"
#include "robot_config.h"

namespace
{
  const uint8_t MOTOR_FORWARD_LEVEL = HIGH;
  const uint8_t MOTOR_BACKWARD_LEVEL = LOW;

  void setMotorOutputs(uint8_t leftDir, uint8_t leftSpeed, uint8_t rightDir, uint8_t rightSpeed)
  {
    digitalWrite(LEFT_MOTOR_DIR_PIN, leftDir);
    digitalWrite(RIGHT_MOTOR_DIR_PIN, rightDir);
    analogWrite(LEFT_MOTOR_PWM_PIN, leftSpeed);
    analogWrite(RIGHT_MOTOR_PWM_PIN, rightSpeed);
  }
}

void motor_init()
{
  pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);

  digitalWrite(LEFT_MOTOR_DIR_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_DIR_PIN, LOW);
  analogWrite(LEFT_MOTOR_PWM_PIN, 0);
  analogWrite(RIGHT_MOTOR_PWM_PIN, 0);
}

void motor_stop()
{
  setMotorOutputs(MOTOR_FORWARD_LEVEL, 0, MOTOR_FORWARD_LEVEL, 0);
}

void motor_forward()
{
  setMotorOutputs(MOTOR_FORWARD_LEVEL, MOTOR_FULL_SPEED, MOTOR_FORWARD_LEVEL, MOTOR_FULL_SPEED);
}

void motor_backward()
{
  setMotorOutputs(MOTOR_BACKWARD_LEVEL, MOTOR_FULL_SPEED, MOTOR_BACKWARD_LEVEL, MOTOR_FULL_SPEED);
}

void motor_turn_left()
{
  setMotorOutputs(MOTOR_BACKWARD_LEVEL, 0, MOTOR_FORWARD_LEVEL, MOTOR_TURN_SPEED);
}

void motor_turn_right()
{
  setMotorOutputs(MOTOR_FORWARD_LEVEL, MOTOR_TURN_SPEED, MOTOR_BACKWARD_LEVEL, 0);
}

void motor_forward_left()
{
  setMotorOutputs(MOTOR_FORWARD_LEVEL, MOTOR_CURVE_SPEED, MOTOR_FORWARD_LEVEL, MOTOR_FULL_SPEED);
}

void motor_forward_right()
{
  setMotorOutputs(MOTOR_FORWARD_LEVEL, MOTOR_FULL_SPEED, MOTOR_FORWARD_LEVEL, MOTOR_CURVE_SPEED);
}

void motor_backward_left()
{
  setMotorOutputs(MOTOR_BACKWARD_LEVEL, MOTOR_CURVE_SPEED, MOTOR_BACKWARD_LEVEL, MOTOR_FULL_SPEED);
}

void motor_backward_right()
{
  setMotorOutputs(MOTOR_BACKWARD_LEVEL, MOTOR_FULL_SPEED, MOTOR_BACKWARD_LEVEL, MOTOR_CURVE_SPEED);
}