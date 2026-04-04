import processing.serial.*;
Serial myPort;
String data = "";
int angle = 0;
int distance = 0;
int prevAngle = 0;

// Trail effect
int[] trailAngles = new int[30];
int[] trailDistances = new int[30];
int trailIndex = 0;

color bgColor, radarGreen, sweepGreen, objectRed, textGreen, dimGreen;

void setup() {
  size(900, 600);
  
  bgColor    = color(0, 10, 0);       // เกือบดำ แต่เขียวนิดหน่อย
  radarGreen = color(0, 180, 0);      // เขียวสว่าง
  sweepGreen = color(0, 255, 80);     // เขียวสว่างมาก
  objectRed  = color(255, 50, 50);    // แดงสด
  textGreen  = color(0, 200, 80);     // เขียวข้อความ
  dimGreen   = color(0, 80, 0);       // เขียวจาง (วงรอบนอก)
  
  println(Serial.list());
  myPort = new Serial(this, "COM8", 9600);
  myPort.bufferUntil('.');
}

void draw() {
  // พื้นหลังโปร่งแสง (สร้าง motion blur effect)
  fill(bgColor, 30);
  noStroke();
  rect(0, 0, width, height);
  
  drawGrid();
  drawRadar();
  drawSweepFan();   // fan กวาดแสง
  drawSweepLine();
  drawTrail();      // ร่องรอยการกวาด
  drawObject();
  drawHUD();
}

void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('.');
  if (data != null) {
    data = trim(data);
    int commaIndex = data.indexOf(",");
    if (commaIndex > 0) {
      try {
        prevAngle = angle;
        angle = int(data.substring(0, commaIndex));
        distance = int(data.substring(commaIndex + 1));
        
        // บันทึก trail
        trailAngles[trailIndex] = angle;
        trailDistances[trailIndex] = distance;
        trailIndex = (trailIndex + 1) % trailAngles.length;
      } catch(Exception e) {}
    }
  }
}

// กริดพื้นหลัง
void drawGrid() {
  pushMatrix();
  translate(width/2, height - 20);
  
  stroke(0, 40, 0);
  strokeWeight(0.3);
  
  // เส้นแนวนอน (arc แบบ faint)
  noFill();
  for (int r = 50; r <= 300; r += 50) {
    arc(0, 0, r*2, r*2, PI, TWO_PI);
  }
  // เส้นรัศมี (ทุก 30 องศา)
  for (int a = 0; a <= 180; a += 30) {
    float x = 300 * cos(radians(a));
    float y = -300 * sin(radians(a));
    line(0, 0, x, y);
  }
  popMatrix();
}

// วงเรดาร์หลัก
void drawRadar() {
  pushMatrix();
  translate(width/2, height - 20);
  
  noFill();
  strokeWeight(1);
  
  int[] radii = {100, 200, 300};
  float[] distances_cm = {20, 40, 60};
  
  for (int i = 0; i < radii.length; i++) {
    // วงหลัก
    stroke(radarGreen, 180);
    arc(0, 0, radii[i]*2, radii[i]*2, PI, TWO_PI);
    
    // Label ระยะ (วางที่จุด 90 องศา = ตรงกลาง)
    fill(textGreen, 200);
    noStroke();
    textSize(11);
    textAlign(CENTER);
    float labelX = -radii[i] * cos(radians(90));
    float labelY = -radii[i] * sin(radians(90));
    text(int(distances_cm[i]) + "cm", labelX - 28, labelY + 4);
    noFill();
    stroke(radarGreen, 180);
  }
  
  // เส้นขอบล่าง
  strokeWeight(2);
  stroke(radarGreen);
  line(-310, 0, 310, 0);
  
  popMatrix();
}

// fan กวาดแสง (สว่างบริเวณหัวเส้น sweep)
void drawSweepFan() {
  pushMatrix();
  translate(width/2, height - 20);
  
  float sweepRad = radians(angle);
  int fanWidth = 15; // องศากว้างของ fan
  
  for (int i = 0; i < fanWidth; i++) {
    float alpha = map(i, 0, fanWidth, 80, 0);
    float a = sweepRad + radians(i);
    stroke(sweepGreen, alpha);
    strokeWeight(1);
    line(0, 0, 300 * cos(a), -300 * sin(a));
  }
  popMatrix();
}

// เส้น sweep หลัก
void drawSweepLine() {
  pushMatrix();
  translate(width/2, height - 20);
  
  stroke(sweepGreen);
  strokeWeight(2.5);
  line(0, 0, 300 * cos(radians(angle)), -300 * sin(radians(angle)));
  
  // จุดกลางเรดาร์
  fill(sweepGreen);
  noStroke();
  ellipse(0, 0, 8, 8);
  
  popMatrix();
}

// ร่องรอยที่ผ่านมา (trail)
void drawTrail() {
  pushMatrix();
  translate(width/2, height - 20);
  
  float scale = 5.0;
  
  for (int i = 0; i < trailAngles.length; i++) {
    int idx = (trailIndex - 1 - i + trailAngles.length) % trailAngles.length;
    if (trailDistances[idx] > 0 && trailDistances[idx] < 60) {
      float alpha = map(i, 0, trailAngles.length, 200, 0);
      float size  = map(i, 0, trailAngles.length, 14, 4);
      
      float x = trailDistances[idx] * scale * cos(radians(trailAngles[idx]));
      float y = -trailDistances[idx] * scale * sin(radians(trailAngles[idx]));
      
      noStroke();
      fill(objectRed, alpha);
      ellipse(x, y, size, size);
    }
  }
  popMatrix();
}

// จุด object หลัก
void drawObject() {
  if (distance <= 0 || distance > 60) return;
  
  pushMatrix();
  translate(width/2, height - 20);
  
  float scale = 5.0;
  float x = distance * scale * cos(radians(angle));
  float y = -distance * scale * sin(radians(angle));
  
  // วงกลมกระพริบ (pulsing ring)
  float pulse = sin(frameCount * 0.2) * 4;
  noFill();
  stroke(objectRed, 100);
  strokeWeight(1);
  ellipse(x, y, 28 + pulse, 28 + pulse);
  
  // จุดหลัก
  noStroke();
  fill(objectRed, 255);
  ellipse(x, y, 12, 12);
  
  // จุดแกนกลางสว่าง
  fill(255, 200, 200);
  ellipse(x, y, 5, 5);
  
  popMatrix();
}

// HUD ข้อมูล
void drawHUD() {
  // กรอบ HUD ซ้ายบน
  fill(0, 150, 0, 20);
  stroke(radarGreen, 80);
  strokeWeight(0.5);
  rect(10, 10, 200, 100, 4);
  
  fill(textGreen);
  noStroke();
  textSize(12);
  textAlign(LEFT);
  
  text("[ RADAR SYSTEM ]", 20, 30);
  
  textSize(11);
  fill(textGreen, 180);
  text("ANGLE   : " + nf(angle, 3) + " deg", 20, 50);
  text("DIST    : " + (distance > 0 && distance <= 60 ? nf(distance, 2) + " cm" : "---"), 20, 68);
  text("STATUS  : " + (distance > 0 && distance <= 60 ? "DETECTED" : "SCANNING"), 20, 86);
  
  // indicator สีแดง/เขียวขวาบน
  fill(distance > 0 && distance <= 60 ? objectRed : dimGreen);
  ellipse(width - 20, 20, 10, 10);
  
  // label ชื่อโปรเจกต์ล่างขวา
  fill(dimGreen);
  textSize(10);
  textAlign(RIGHT);
  text("HC-SR04 RADAR v2.0", width - 10, height - 10);
}
