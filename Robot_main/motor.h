#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// PIN CONFIG ต้องแก้ใหม่
#define MA1 2
#define MA2 3
#define MB1 4
#define MB2 5

#define PWM_A 6
#define PWM_B 9

#define SPEED 200

void motor_init();

void Forward(int time);
void Backward(int time);
void turnLeft(int time);
void turnRight(int time);
void Stop(int time);

#endif