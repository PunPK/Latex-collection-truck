#include <Servo.h>

Servo myServo;

int trigPin = 7;
int echoPin = 8;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  // หมุน 0 → 180
  for(int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(30);

    distance = getDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    
  }

  // หมุน 180 → 0
  for(int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(30);

    distance = getDistance();

    Serial.print(angle);
Serial.print(",");
Serial.print(distance);
Serial.println(".");
delay(10);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}