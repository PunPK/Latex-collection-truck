#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include <Arduino.h>

// Motor pins
constexpr uint8_t LEFT_MOTOR_PWM_PIN = 10;
constexpr uint8_t LEFT_MOTOR_DIR_PIN = 12;
constexpr uint8_t RIGHT_MOTOR_PWM_PIN = 11;
constexpr uint8_t RIGHT_MOTOR_DIR_PIN = 13;

// Extra peripherals
constexpr uint8_t BUZZER_PIN = 4;
constexpr uint8_t SERVO_PIN = 9;
constexpr uint8_t ULTRASONIC_TRIG_PIN = 7;
constexpr uint8_t ULTRASONIC_ECHO_PIN = 8;

// PS2 software SPI pins
constexpr uint8_t PS2_DAT_PIN = 2;
constexpr uint8_t PS2_CMD_PIN = 3;
constexpr uint8_t PS2_ATT_PIN = 5;
constexpr uint8_t PS2_CLK_PIN = 6;

// Motion tuning
constexpr uint8_t MOTOR_FULL_SPEED = 200;
constexpr uint8_t MOTOR_TURN_SPEED = 170;
constexpr uint8_t MOTOR_CURVE_SPEED = 140;

#endif