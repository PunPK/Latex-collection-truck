#include "ultrasonic_sensor.h"

#include "robot_config.h"

namespace
{
    const unsigned long SERVO_FRAME_US = 20000UL;
    const uint16_t SERVO_MIN_PULSE_US = 1000;
    const uint16_t SERVO_MAX_PULSE_US = 2000;
    const unsigned long SERVO_STEP_INTERVAL_MS = 30UL;
    const unsigned long DISTANCE_SAMPLE_INTERVAL_MS = 70UL;
    const unsigned long ULTRASONIC_TIMEOUT_US = 8000UL;

    uint8_t servoAngle = 0;
    int8_t sweepDirection = 1;
    uint16_t servoPulseWidthUs = 1500;
    unsigned long lastSweepUpdateMs = 0;
    unsigned long lastDistanceSampleMs = 0;
    unsigned long lastServoFrameStartUs = 0;
    unsigned long servoPulseHighStartUs = 0;
    bool servoPulseHigh = false;
    int lastDistanceCm = 0;

    void setServoAngle(uint8_t angle)
    {
        servoAngle = constrain(angle, 0, 180);
        servoPulseWidthUs = map(servoAngle, 0, 180, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
    }

    void serviceServoPulse()
    {
        unsigned long nowUs = micros();

        if (servoPulseHigh)
        {
            if (nowUs - servoPulseHighStartUs >= servoPulseWidthUs)
            {
                digitalWrite(SERVO_PIN, LOW);
                servoPulseHigh = false;
            }
            return;
        }

        if (nowUs - lastServoFrameStartUs >= SERVO_FRAME_US)
        {
            digitalWrite(SERVO_PIN, HIGH);
            servoPulseHighStartUs = nowUs;
            lastServoFrameStartUs = nowUs;
            servoPulseHigh = true;
        }
    }
}

void ultrasonic_init()
{
    pinMode(SERVO_PIN, OUTPUT);
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(SERVO_PIN, LOW);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    setServoAngle(0);
    lastServoFrameStartUs = micros();
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
    serviceServoPulse();

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

    serviceServoPulse();
}
