#include "PCA9685_Control.h"
const int BASE_STOP = 275; // ค่าที่ทำให้มอเตอร์ฐานหยุดนิ่งสนิท
PCA9685Control pwm = PCA9685Control(0x40);

void setup()
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);

  // สั่งให้ฐาน (Channel 0) หยุดนิ่งทันที
  Serial.println("เริ่มต้น: กำลังตั้งค่าแขนกล");
  pwm.setPWM(0, 0, BASE_STOP);
  pwm.setPWM(1, 0, 200);
  pwm.setPWM(2, 0, 200);
  pwm.setPWM(3, 0, 400);
  pwm.setPWM(4, 0, BASE_STOP);
  pwm.setPWM(5, 0, 300);

  Serial.println("เริ่มต้น: กำลังจัดตำแหน่งแขนกล");
  turnBase(5, 300, 1000);
  delay(500);

  Serial.println("เริ่มต้น: จบการตั้งค่าแขนกล");

  delay(1000);
}

//  servo 360
// รับค่าความเร็ว/ทิศทาง (speedPulse) และเวลาที่ให้หมุน (durationMs)
void turnBase(uint8_t channel, uint16_t speedPulse, int durationMs)
{
  pwm.setPWM(channel, 0, speedPulse);
  delay(durationMs);
  pwm.setPWM(channel, 0, BASE_STOP);
}

void loop()
{
  // Serial.println("ฐาน: หมุนไปทิศทางที่ 1 (ความเร็วปานกลาง) เป็นเวลา 1 วินาที");
  // // ค่า > 280 ยิ่งมากยิ่งหมุนเร็ว (เช่น 350)
  // turnBase(0, 300, 1000);
  // delay(2000); // หยุดพัก 2 วินาที
  // Serial.println("ฐาน: หมุนไปทิศทางที่ 2 (ความเร็วปานกลาง) เป็นเวลา 1 วินาที");
  // // ค่า < 270 ยิ่งน้อยยิ่งหมุนเร็ว (เช่น 200)
  // turnBase(0, 230, 1000);
  // delay(2000);

  // Serial.println("บน1: หมุนไปทิศทางที่ 1 (ความเร็วปานกลาง) เป็นเวลา 1 วินาที");
  // // ค่า > 280 ยิ่งมากยิ่งหมุนเร็ว (เช่น 350)
  // turnBase(1, 300, 500);
  // delay(2000); // หยุดพัก 2 วินาที

  // Serial.println("บน1: หมุนไปทิศทางที่ 2 (ความเร็วปานกลาง) เป็นเวลา 1 วินาที");
  // // ค่า < 270 ยิ่งน้อยยิ่งหมุนเร็ว (เช่น 200)
  // turnBase(1, 250, 500);
  // delay(2000);
}