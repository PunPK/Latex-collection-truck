#include "PS2_Controller.h"

void PS2_Init() {
  // ตั้งค่าขา CS เป็น Output และดึงให้เป็น HIGH ไว้ก่อน (ยังไม่คุย)
  pinMode(PS2_CS_PIN, OUTPUT);
  digitalWrite(PS2_CS_PIN, HIGH);
  
  // เปิดใช้งาน Hardware SPI ของ Arduino
  SPI.begin();
}

void PS2_ReadData(uint8_t *ps2_data) {
  // ตั้งค่าโปรโตคอลของ PS2: ความเร็ว 250kHz, ส่ง LSB ก่อน, โหมด 3
  SPI.beginTransaction(SPISettings(250000, LSBFIRST, SPI_MODE3));

  // 1. ดึงขา CS เป็น LOW เพื่อเริ่มคุย
  digitalWrite(PS2_CS_PIN, LOW);
  delayMicroseconds(10);

  // 2. ส่งคำสั่งพื้นฐาน
  SPI.transfer(0x01); // Start Byte
  SPI.transfer(0x42); // Request Data
  SPI.transfer(0x00); // Dummy

  // 3. อ่านค่าข้อมูล 6 ไบต์เก็บลง Array
  ps2_data[0] = SPI.transfer(0x00); // ปุ่มกลุ่มแรก (ทิศทาง)
  ps2_data[1] = SPI.transfer(0x00); // ปุ่มกลุ่มสอง (แอคชัน)
  ps2_data[2] = SPI.transfer(0x00); // อนาล็อกขวา X
  ps2_data[3] = SPI.transfer(0x00); // อนาล็อกขวา Y
  ps2_data[4] = SPI.transfer(0x00); // อนาล็อกซ้าย X
  ps2_data[5] = SPI.transfer(0x00); // อนาล็อกซ้าย Y

  // 4. ดึงขา CS กลับเป็น HIGH เพื่อจบการสนทนา
  delayMicroseconds(10);
  digitalWrite(PS2_CS_PIN, HIGH);

  // ปิดการตั้งค่า SPI ชุดนี้
  SPI.endTransaction();
}