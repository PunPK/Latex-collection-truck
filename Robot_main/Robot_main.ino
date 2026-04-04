#include "ps2_controller.h"

// Array เก็บข้อมูลจากจอย
uint8_t ps2_data[6];

void setup()
{
    Serial.begin(9600);
    SPI_Init();
}

void loop()
{
    PS2_ReadData(ps2_data);

    // Debug
    Serial.print("DATA: ");
    for (int i = 0; i < 6; i++)
    {
        Serial.print(ps2_data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    delay(100);
}