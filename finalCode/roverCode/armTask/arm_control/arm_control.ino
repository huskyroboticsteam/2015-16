#include "arm_control.h"
#include <math.h>

#define ARM_PACK_START 4
#define SHOLDER_ROT 0
#define SHOLDER 1
#define ELBOW 2
#define ELBOW_ROT 3
#define WRIST 4
#define WRIST_ROT 5
#define HAND 6

#define SPEED_SCALAR 
int16_t prevArmPos[7];
char packetBuffer[18];
char packetVal = 0;

// calculates the arm pos and writes to the motors
void calculateArmPos() {
  // parse the packet into the array
  
  int16_t nextArmPos[7];
  for (int i = 0; i < 7; i++) {
    nextArmPos[i] = packetBuffer[ARM_PACK_START + 2 * i];
    nextArmPos[i] *= 256;
    nextArmPos[i] += packetBuffer[ARM_PACK_START + 2 * i + 1];
  }

  // if the potentiometers gets done prevArmPos will be chnaged here
  
  // calculate the difference and write to motors
  
  int16_t diffArmPos[7];
  for (int k = 0; k < 7; k++) {
    diffArmPos[k] = prevArmPos[k] - nextArmPos[k];
  }
  
  
  // write to motors
  // sholder_rot
  if (diffArmPos[SHOLDER_ROT] < 0) {
    driveMotor(SHOLDER_ROT, 1, std::abs(diffArmPos[SHOLDER_ROT]));
  } else {
    driveMotor(SHOLDER_ROT, 0, std::abs(diffArmPos[SHOLDER_ROT]));
  }
  // sholder
  if (diffArmPos[SHOLDER] < 0) {
    driveMotor(SHOLDER, 1, std::abs(diffArmPos[SHOLDER]));
  } else {
    driveMotor(SHOLDER, 0, std::abs(diffArmPos[SHOLDER]));
  }
  // elbow
  if (diffArmPos[ELBOW] < 0) {
    driveMotor(ELBOW, 1, std::abs(diffArmPos[ELBOW]));
  } else {
    driveMotor(ELBOW, 0, std::abs(diffArmPos[ELBOW]));
  }
  // elbow_rot
  if (diffArmPos[ELBOW_ROT] < 0) {
    driveMotor(ELBOW_ROT, 1, std::abs(diffArmPos[ELBOW_ROT]));
  } else {
    driveMotor(ELBOW_ROT, 0, std::abs(diffArmPos[ELBOW_ROT]));
  }
  // wrist
  if (diffArmPos[WRIST] < 0) {
    driveMotor(WRIST, 1, std::abs(diffArmPos[WRIST]));
  } else {
    driveMotor(WRIST, 0, std::abs(diffArmPos[WRIST]));
  }
  // wrist_rot
  if (diffArmPos[WRIST_ROT] < 0) {
    driveMotor(WRIST_ROT, 1, std::abs(diffArmPos[WRIST_ROT]));
  } else {
    driveMotor(WRIST_ROT, 0, std::abs(diffArmPos[WRIST_ROT]));
  }
  // hand
  if (diffArmPos[HAND] < 0) {
    driveMotor(HAND, 1, std::abs(diffArmPos[HAND]));
  } else {
    driveMotor(HAND, 0, std::abs(diffArmPos[HAND]));
  }
  
  // replace prev with next
  for (int j = 0; j < 7; j++) {
    prevArmPos[j] = nextArmPos[j];
  }
  
}

void setup() {
  // put your setup code here, to run once:
  initArm();
  enableMotors();
  prevArmPos[0] = 0;
  prevArmPos[1] = 0;
  prevArmPos[2] = 0;
  prevArmPos[3] = 0;
  prevArmPos[4] = 0;
  prevArmPos[5] = 0;
  prevArmPos[6] = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  calculateArmPos();
  packetBuffer[4] = packetVal;
  packetBuffer[5] = packetVal;
  packetBuffer[6] = packetVal;
  packetBuffer[7] = packetVal;
  packetBuffer[8] = packetVal;
  packetBuffer[9] = packetVal;
  packetBuffer[10] = packetVal;
  packetBuffer[11] = packetVal;
  packetBuffer[12] = packetVal;
  packetBuffer[13] = packetVal;
  packetBuffer[14] = packetVal;
  packetBuffer[15] = packetVal;
  packetBuffer[16] = packetVal;
  packetBuffer[17] = packetVal;
  packetBuffer[18] = packetVal;
  if (packetVal > 50) {
    packetVal = -50;
  }
  packetVal++;
