#include "motor.h"

void motor_init() {
    pinMode(MA1, OUTPUT);
    pinMode(MA2, OUTPUT);
    pinMode(MB1, OUTPUT);
    pinMode(MB2, OUTPUT);
    pinMode(PWM_A, OUTPUT);
    pinMode(PWM_B, OUTPUT);
}

void Forward(int time)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, HIGH);
  analogWrite(PWM_A, SPEED);

  digitalWrite(MB1, HIGH);
  digitalWrite(MB2, LOW);
  analogWrite(PWM_B, SPEED);

  delay(time);
}

void Backward(int time)
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, LOW);
  analogWrite(PWM_A, SPEED);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, HIGH);
  analogWrite(PWM_B, SPEED);

  delay(time);
}

void turnLeft(int time)
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, LOW);
  analogWrite(PWM_A, SPEED);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, LOW);
  analogWrite(PWM_B, 0);

  delay(time);
}

void turnRight(int time)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, LOW);
  analogWrite(PWM_A, 0);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, HIGH);
  analogWrite(PWM_B, SPEED);

  delay(time);
}

void Stop(int time)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, LOW);
  analogWrite(PWM_A, 0);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, LOW);
  analogWrite(PWM_B, 0);

  delay(time);
}