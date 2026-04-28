#include "PS2_Controller.h"

namespace
{
    bool ps2PacketLooksValid(const uint8_t *ps2_data)
    {
        if (ps2_data[0] == 0xFF && ps2_data[1] == 0xFF && ps2_data[2] == 0xFF &&
            ps2_data[3] == 0xFF && ps2_data[4] == 0xFF && ps2_data[5] == 0xFF)
        {
            return false;
        }

        return ps2_data[1] != 0xFF || ps2_data[2] != 0xFF || ps2_data[3] != 0xFF;
    }

    uint8_t ps2Transfer(uint8_t outgoingByte)
    {
        uint8_t incomingByte = 0;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            digitalWrite(PS2_CMD_PIN, (outgoingByte & 0x01) ? HIGH : LOW);
            outgoingByte >>= 1;

            digitalWrite(PS2_CLK_PIN, LOW);
            delayMicroseconds(8);

            if (digitalRead(PS2_DAT_PIN) == HIGH)
            {
                incomingByte |= (1 << bit);
            }

            digitalWrite(PS2_CLK_PIN, HIGH);
            delayMicroseconds(8);
        }

        return incomingByte;
    }

    // --- Analog filtering + calibration for joystick X (ps2_data[5]) ---
    const uint8_t FILTER_SIZE = 5;
    const uint8_t DEADZONE_THRESHOLD = 12; // adjust as needed

    uint8_t analogBuffer[FILTER_SIZE] = {128, 128, 128, 128, 128};
    uint8_t analogIndex = 0;
    uint8_t analogCount = 0; // how many valid samples in buffer (<= FILTER_SIZE)
    uint8_t analogCenter = 128;
    bool analogCalibrated = false;

    void addAnalogSample(uint8_t v)
    {
        analogBuffer[analogIndex] = v;
        analogIndex = (analogIndex + 1) % FILTER_SIZE;
        if (analogCount < FILTER_SIZE) analogCount++;
    }

    uint8_t getMedianAnalog()
    {
        uint8_t tmp[FILTER_SIZE];
        uint8_t n = analogCount;
        if (n == 0) return analogCenter;
        for (uint8_t i = 0; i < n; i++) tmp[i] = analogBuffer[i];

        // simple sort (bubble/insertion ok for small N)
        for (uint8_t i = 0; i < n - 1; i++)
        {
            for (uint8_t j = i + 1; j < n; j++)
            {
                if (tmp[j] < tmp[i])
                {
                    uint8_t t = tmp[i];
                    tmp[i] = tmp[j];
                    tmp[j] = t;
                }
            }
        }

        return tmp[n/2];
    }
}

void PS2_Init()
{
    pinMode(PS2_CMD_PIN, OUTPUT);
    pinMode(PS2_CLK_PIN, OUTPUT);
    pinMode(PS2_ATT_PIN, OUTPUT);
    pinMode(PS2_DAT_PIN, INPUT_PULLUP);

    digitalWrite(PS2_CMD_PIN, HIGH);
    digitalWrite(PS2_CLK_PIN, HIGH);
    digitalWrite(PS2_ATT_PIN, HIGH);

    // Quick calibration: read a few samples to establish analog center
    for (uint8_t i = 0; i < FILTER_SIZE; i++)
    {
        uint8_t tmp[6] = {0};
        PS2_ReadData(tmp);
        addAnalogSample(tmp[5]);
        delay(20);
    }
    analogCenter = getMedianAnalog();
    analogCalibrated = true;
}

void PS2_ReadData(uint8_t *ps2_data)
{
    digitalWrite(PS2_ATT_PIN, LOW);
    delayMicroseconds(10);

    ps2Transfer(0x01);
    ps2Transfer(0x42);
    ps2Transfer(0x00);

    for (uint8_t i = 0; i < 6; i++)
    {
        ps2_data[i] = ps2Transfer(0x00);
    }

    // add recent analog X sample into filter buffer
    addAnalogSample(ps2_data[5]);

    delayMicroseconds(10);
    digitalWrite(PS2_ATT_PIN, HIGH);
}

PS2_Status PS2_GetStatus(uint8_t *ps2_data)
{
    if (!ps2PacketLooksValid(ps2_data))
    {
        return STOP;
    }

    // Use filtered analog X (ps2_data[5]) with deadzone and digital buttons for Y
    uint8_t data_b1 = ps2_data[1];
    uint8_t analog_x = getMedianAnalog();

    int x = 0;
    int y = 0;

    if ((uint8_t)abs((int)analog_x - (int)analogCenter) <= DEADZONE_THRESHOLD)
    {
        x = 0;
    }
    else if (analog_x < analogCenter)
    {
        x = -1;
    }
    else
    {
        x = 1;
    }

    if (data_b1 == 0xFE || data_b1 == 0xFC || data_b1 == 0xE6)
    {
        y = 1;
    }
    else if (data_b1 == 0xFB || data_b1 == 0xF9 || data_b1 == 0xE3)
    {
        y = -1;
    }

    if (x == 0 && y == 1)
        return FORWARD;
    if (x == 0 && y == -1)
        return BACKWARD;
    if (x == -1 && y == 0)
        return LEFT;
    if (x == 1 && y == 0)
        return RIGHT;
    if (x == -1 && y == 1)
        return FORWARD_LEFT;
    if (x == 1 && y == 1)
        return FORWARD_RIGHT;
    if (x == -1 && y == -1)
        return BACKWARD_LEFT;
    if (x == 1 && y == -1)
        return BACKWARD_RIGHT;

    return STOP;
}

// PS2_Status PS2_GetStatus(uint8_t *ps2_data)
// {
//     if (!ps2PacketLooksValid(ps2_data))
//     {
//         return STOP;
//     }

//     uint8_t data_b1 = ps2_data[1];
//     uint8_t analog_x = ps2_data[5];

//     if (data_b1 == 0xFF && analog_x > 100 && analog_x < 155)
//     {
//         return STOP;
//     }

//     int x = 0;
//     int y = 0;

//     if (analog_x < 100)
//     {
//         x = -1;
//     }
//     else if (analog_x > 155)
//     {
//         x = 1;
//     }

//     if (data_b1 == 0xFE || data_b1 == 0xFC || data_b1 == 0xE6)
//     {
//         y = 1;
//     }
//     else if (data_b1 == 0xFB || data_b1 == 0xF9 || data_b1 == 0xE3)
//     {
//         y = -1;
//     }

//     if (x == 0 && y == 1)
//         return FORWARD;
//     if (x == 0 && y == -1)
//         return BACKWARD;
//     if (x == -1 && y == 0)
//         return LEFT;
//     if (x == 1 && y == 0)
//         return RIGHT;
//     if (x == -1 && y == 1)
//         return FORWARD_LEFT;
//     if (x == 1 && y == 1)
//         return FORWARD_RIGHT;
//     if (x == -1 && y == -1)
//         return BACKWARD_LEFT;
//     if (x == 1 && y == -1)
//         return BACKWARD_RIGHT;

//     return STOP;
// }

void PS2_EnableAnalog()
{
    digitalWrite(PS2_ATT_PIN, LOW);
    delayMicroseconds(20);

    ps2Transfer(0x01);
    ps2Transfer(0x43); // enter config mode
    ps2Transfer(0x00);
    ps2Transfer(0x01);
    ps2Transfer(0x00);

    digitalWrite(PS2_ATT_PIN, HIGH);
    delay(10);

    digitalWrite(PS2_ATT_PIN, LOW);
    delayMicroseconds(20);

    ps2Transfer(0x01);
    ps2Transfer(0x44); // set analog mode
    ps2Transfer(0x00);
    ps2Transfer(0x01); // analog
    ps2Transfer(0x03);
    ps2Transfer(0x00);
    ps2Transfer(0x00);
    ps2Transfer(0x00);
    ps2Transfer(0x00);

    digitalWrite(PS2_ATT_PIN, HIGH);
    delay(10);

    digitalWrite(PS2_ATT_PIN, LOW);
    delayMicroseconds(20);

    ps2Transfer(0x01);
    ps2Transfer(0x43); // exit config
    ps2Transfer(0x00);
    ps2Transfer(0x00);
    ps2Transfer(0x5A);
    ps2Transfer(0x5A);
    ps2Transfer(0x5A);
    ps2Transfer(0x5A);
    ps2Transfer(0x5A);

    digitalWrite(PS2_ATT_PIN, HIGH);
}