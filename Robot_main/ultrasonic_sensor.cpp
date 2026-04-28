#include "ultrasonic_sensor.h"

#include "robot_config.h"
#include <Servo.h>

namespace
{
    const unsigned long SERVO_STEP_INTERVAL_MS = 30UL;
    const unsigned long DISTANCE_SAMPLE_INTERVAL_MS = 70UL;
    const unsigned long ULTRASONIC_TIMEOUT_US = 8000UL;

    uint8_t servoAngle = 0;
    int8_t sweepDirection = 1;
    uint16_t servoPulseWidthUs = 1500;
    unsigned long lastSweepUpdateMs = 0;
    unsigned long lastDistanceSampleMs = 0;
    int lastDistanceCm = 0;

    Servo ultrasonicServo;

    void setServoAngle(uint8_t angle)
    {
        servoAngle = constrain(angle, 0, 180);
        servoPulseWidthUs = map(servoAngle, 0, 180, 1000, 2000);
        // Use hardware-timed pulses via Servo library for smooth movement
        ultrasonicServo.writeMicroseconds(servoPulseWidthUs);
    }
}

void ultrasonic_init()
{
    // Attach servo to SERVO_PIN using Servo library (hardware-timed)
    ultrasonicServo.attach(SERVO_PIN);

    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    setServoAngle(0);
}

int ultrasonic_read_distance_cm()
{
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    unsigned long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT_US);
    if (duration == 0)
    {
        return 0;
    }

    return (int)(duration * 0.0343 / 2.0);
}

void ultrasonic_update()
{
    unsigned long nowMs = millis();
    if (nowMs - lastSweepUpdateMs >= SERVO_STEP_INTERVAL_MS)
    {
        lastSweepUpdateMs = nowMs;

        if (sweepDirection > 0 && servoAngle >= 180)
        {
            sweepDirection = -1;
        }
        else if (sweepDirection < 0 && servoAngle <= 0)
        {
            sweepDirection = 1;
        }

        setServoAngle((uint8_t)constrain((int)servoAngle + sweepDirection, 0, 180));
    }

    if (nowMs - lastDistanceSampleMs >= DISTANCE_SAMPLE_INTERVAL_MS)
    {
        lastDistanceSampleMs = nowMs;
        lastDistanceCm = ultrasonic_read_distance_cm();

        Serial.print(servoAngle);
        Serial.print(",");
        Serial.print(lastDistanceCm);
        Serial.println(".");
    }
}
