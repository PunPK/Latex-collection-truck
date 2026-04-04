#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <Arduino.h>
#include <SPI.h>

// กำหนดขา CS (Chip Select) หรือ ATT ของ PS2
// ใน Arduino IDE เราอ้างอิงเป็นหมายเลขขาบนบอร์ดได้เลย
#define PS2_CS_PIN 10 

// ขา SPI อื่นๆ Arduino จะจัดการให้อัตโนมัติ:
// MOSI = 11, MISO = 12, SCK = 13

void PS2_Init();
void PS2_ReadData(uint8_t *ps2_data);

#endif