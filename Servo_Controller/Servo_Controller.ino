#include "PCA9685_Control.h"

PCA9685Control pwm = PCA9685Control(0x40);

void setup() {
  Serial.begin(9600);
  pwm.begin();         
  pwm.setPWMFreq(50);  

  // กำหนดท่าเริ่มต้นของแขนกล (สมมติให้ช่อง 0 และ 1 ไปที่จุดกึ่งกลาง)
  pwm.setPWM(0, 0, 300);
  pwm.setPWM(1, 0, 300);
  delay(1000); // รอให้เข้าที่
}

// สร้างฟังก์ชันขยับ Servo แบบนุ่มนวล โดยอ้างอิงจากตำแหน่งปัจจุบัน
void moveSmooth(uint8_t channel, uint16_t targetPulse, int speedDelay) {
  // 1. ถามคลาสว่า ตอนนี้ Servo ตัวนี้อยู่ที่ตำแหน่งไหน?
  uint16_t current = pwm.getPos(channel); 
  
  // 2. เปรียบเทียบและค่อยๆ สั่งให้ขยับไปหาเป้าหมาย
  if (current < targetPulse) {
    for (uint16_t p = current; p <= targetPulse; p++) {
      pwm.setPWM(channel, 0, p);
      delay(speedDelay);
    }
  } 
  else if (current > targetPulse) {
    for (uint16_t p = current; p >= targetPulse; p--) {
      pwm.setPWM(channel, 0, p);
      delay(speedDelay);
    }
  }
}

void loop() {
  Serial.println("ขยับข้อต่อที่ 0 ไปตำแหน่ง 400 ช้าๆ...");
  moveSmooth(0, 400, 500); // ช่อง 0, เป้าหมาย 400, delay 5ms ต่อเสต็ป
  delay(1000);

  Serial.println("ขยับข้อต่อที่ 1 ไปตำแหน่ง 200 เร็วขึ้น...");
  moveSmooth(1, 200, 200); // ช่อง 1, เป้าหมาย 200, delay 2ms ต่อเสต็ป
  delay(1000);
  
  Serial.println("ขยับข้อต่อ 0 กลับมาที่ 300...");
  moveSmooth(0, 300, 500);
  delay(1000);
}