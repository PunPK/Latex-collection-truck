#include "PS2_Controller.h"

// สร้าง Array สำหรับเก็บค่าจอย 6 ไบต์
uint8_t ps2_data[6];

void setup() {
  // เปิด Serial Monitor ที่ Baud Rate 9600
  Serial.begin(9600);
  
  // เรียกฟังก์ชันตั้งค่าจอย PS2
  PS2_Init();
  
  Serial.println("PS2 Controller Ready! (Arduino Style)");
}

void loop() {
  // โยน Array เข้าไปเก็บค่าจากจอย
  PS2_ReadData(ps2_data);

  // จัดรูปแบบข้อความให้อ่านง่าย แล้วสั่งปริ้นท์
  // ใช้ sprintf เพื่อรวมค่าตัวแปรทั้งหมดเข้าไปในข้อความเดียว
  char printBuffer[80];
  sprintf(printBuffer, "Btn1: %02X | Btn2: %02X | RX: %3d | RY: %3d | LX: %3d | LY: %3d", 
          ps2_data[0], ps2_data[1], ps2_data[2], ps2_data[3], ps2_data[4], ps2_data[5]);
          
  Serial.println(printBuffer);

  // หน่วงเวลาเล็กน้อย
  delay(100);
}