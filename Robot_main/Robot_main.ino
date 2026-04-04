#include "ps2_controller.h"


// Array เก็บข้อมูลจากจอย
uint8_t ps2_data[6];
PS2_Status status;

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
    status = PS2_GetStatus(ps2_data);

    Serial.print(" | ");
    switch (status) {

            case FORWARD: Serial.print("FORWARD"); break;
            case BACKWARD: Serial.print("BACKWARD"); break;
            case LEFT: Serial.print("LEFT"); break;
            case RIGHT: Serial.print("RIGHT"); break;

            case FORWARD_LEFT: Serial.print("FORWARD LEFT"); break;
            case FORWARD_RIGHT: Serial.print("FORWARD RIGHT"); break;
            case BACKWARD_LEFT: Serial.print("BACKWARD LEFT"); break;
            case BACKWARD_RIGHT: Serial.print("BACKWARD RIGHT"); break;

            default: Serial.print("STOP"); break;
        }

    Serial.println();

    delay(100);
}