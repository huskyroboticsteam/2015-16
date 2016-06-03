#include <Servo.h>




void initializeArmTest() {
  sholder_rot.attach(SHOLDER_ROT);
  sholder.attach(SHOLDER);
  elbow.attach(ELBOW);
  elbow_rot.attach(ELBOW_ROT);
  wrist.attach(WRIST);
  wrist_rot.attach(WRIST_ROT);
  hand.attach(HAND);
  sholder_rot.writeMicroseconds(1500);
  sholder.writeMicroseconds(1500);
  elbow.writeMicroseconds(1500);
  elbow_rot.writeMicroseconds(1500);
  wrist.writeMicroseconds(1500);
  wrist_rot.writeMicroseconds(1500);
  hand.writeMicroseconds(1500);
}

void calculateArmTest() {
  sholder_rot.writeMicroseconds(1500);
  sholder.writeMicroseconds(1500);
  elbow.writeMicroseconds(1500);
  elbow_rot.writeMicroseconds(1500);
  wrist.writeMicroseconds(1500);
  wrist_rot.writeMicroseconds(1500);
  hand.writeMicroseconds(1500);
  delay(1000);
  hand.writeMicroseconds(1600);
  delay(1000);
  hand.writeMicroseconds(1400);
  delay(1000);
  
  
}
