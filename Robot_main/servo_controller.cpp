#include "servo_controller.h"

PCA9685Control pwm = PCA9685Control(0x40);

const int BASE_STOP = 275; // ค่าที่ทำให้มอเตอร์ฐานหยุดนิ่งสนิท

void servo_init()
{
  pwm.begin();
  pwm.setPWMFreq(50);

  Serial.println("เริ่มต้น: กำลังตั้งค่าแขนกล");
  pwm.setPWM(0, 0, BASE_STOP);
  pwm.setPWM(1, 0, 470);
  pwm.setPWM(2, 0, 200);
  pwm.setPWM(3, 0, 400);
  pwm.setPWM(4, 0, BASE_STOP);
  pwm.setPWM(5, 0, 450);

  Serial.println("เริ่มต้น: กำลังจัดตำแหน่งแขนกล");
  // turnBase(5, 300, 1000);
  // delay(500);

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

void gripper_clamp()
{
  pwm.setPWM(5, 0, 200);
}
void gripper_release()
{
  pwm.setPWM(5, 0, 450);
}

