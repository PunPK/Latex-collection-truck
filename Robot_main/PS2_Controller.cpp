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
            delayMicroseconds(20);

            if (digitalRead(PS2_DAT_PIN) == HIGH)
            {
                incomingByte |= (1 << bit);
            }

            digitalWrite(PS2_CLK_PIN, HIGH);
            delayMicroseconds(20);
        }

        return incomingByte;
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
}

void PS2_ReadData(uint8_t *ps2_data)
{
    digitalWrite(PS2_ATT_PIN, LOW);
    delayMicroseconds(20);

    ps2Transfer(0x01);
    ps2Transfer(0x42);
    ps2Transfer(0x00);

    for (uint8_t i = 0; i < 6; i++)
    {
        ps2_data[i] = ps2Transfer(0x00);
    }

    delayMicroseconds(20);
    // digitalWrite(PS2_ATT_PIN, HIGH);
}

PS2_Status PS2_GetStatus(uint8_t *ps2_data)
{
    if (!ps2PacketLooksValid(ps2_data))
    {
        return STOP;
    }

    uint8_t data_b1 = ps2_data[1];
    uint8_t analog_x = ps2_data[5];

    if (data_b1 == 0xFF && analog_x > 100 && analog_x < 155)
    {
        return STOP;
    }

    int x = 0;
    int y = 0;

    if (analog_x < 100)
    {
        x = -1;
    }
    else if (analog_x > 155)
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