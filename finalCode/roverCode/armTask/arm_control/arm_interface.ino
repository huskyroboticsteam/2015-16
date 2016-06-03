#include "arm_control.h"
#include <Wire.h>


// play around with these values to find the best
// maybe use -100 to 100
#define MIN_SPEED  0
#define MAX_SPEED  100



void initArm() {
  //Serial.println("initArm");
  sholder_rot.attach(SHOLDER_ROT);
  sholder.attach(SHOLDER);
  elbow.attach(ELBOW); 
  elbow_rot.attach(ELBOW_ROT);
  wrist.attach(WRIST);
  wrist_rot.attach(WRIST_ROT);
  hand.attach(HAND);
  stopMotor(sholder_rot);
  stopMotor(sholder);
  stopMotor(elbow);
  stopMotor(elbow_rot);
  stopMotor(wrist);
  stopMotor(wrist_rot);
  stopMotor(hand);/*
  prevArmPos[0] = 0;
  prevArmPos[1] = 0;
  prevArmPos[2] = 0;
  prevArmPos[3] = 0;
  prevArmPos[4] = 0;
  prevArmPos[5] = 0;
  prevArmPos[6] = 0;
  */
}

void enableMotors(){
  
}

void disableMotors(){

}

void driveMotor (Servo mot , int dir , int rawSpeed){
  if (dir > 0) {
    Serial.println("a");
    mot.writeMicroseconds(1600);
  } else if (dir < 0) {
    Serial.println("b");
    mot.writeMicroseconds(1400);
  } else {
    Serial.println("c");
    mot.writeMicroseconds(1500);
  }
  Serial.println(rawSpeed);
}


void stopMotor(Servo mot) {
  driveMotor(mot , 0 , 0);
}

int absolute(int input) {
  if (input < 0) {
    return -input;
  }
  return input;
}

// calculates the arm pos and writes to the motors
void calculateArmPos(int8_t packetBuffer[]) {
  Serial.println("calculate");
  // parse the packet into the array
  
  // int16_t nextArmPos[7];
  // unpack the packet here 
  /*
  for (int i = 0; i < 7; i++) {
    /*
    nextArmPos[i] = packetBuffer[ARM_PACK_START + 2 * i];
    nextArmPos[i] *= 256;
    nextArmPos[i] += packetBuffer[ARM_PACK_START + 2 * i + 1];
    
  }
*/
  // if the potentiometers gets done prevArmPos will be chnaged here
  
  // calculate the difference and write to motors
  /*
  int16_t diffArmPos[7];
  for (int k = 0; k < 7; k++) {
    diffArmPos[k] = prevArmPos[k] - nextArmPos[k];
  }
  */
  
  // write to motors
  // sholder_rot
  if (packetBuffer[0] < 0) {
    driveMotor(sholder_rot, 1, absolute(packetBuffer[0]));
  } else {
    driveMotor(sholder_rot, -1, absolute(packetBuffer[0]));
  }
  
  // sholder
  if (packetBuffer[1] < 0) {
    driveMotor(sholder, 1, absolute(packetBuffer[1]));
  } else {
    driveMotor(sholder, -1, absolute(packetBuffer[1]));
  }
  // elbow
  if (packetBuffer[2] < 0) {
    driveMotor(elbow, 1, absolute(packetBuffer[2]));
  } else {
    driveMotor(elbow, -1, absolute(packetBuffer[2]));
  }
  
  // elbow_rot
  if (packetBuffer[3] < 0) {
    driveMotor(elbow_rot, 1, absolute(packetBuffer[3]));
  } else {
    driveMotor(elbow_rot, -1, absolute(packetBuffer[3]));
  }

  // wrist
  if (packetBuffer[4] < 0) {
    driveMotor(wrist, 1, absolute(packetBuffer[4]));
  } else {
    driveMotor(wrist, -1, absolute(packetBuffer[4]));
  }
  // wrist_rot
  if (packetBuffer[5] < 0) {
    driveMotor(wrist_rot, 1, absolute(packetBuffer[5]));
  } else {
    driveMotor(wrist_rot, -1, absolute(packetBuffer[5]));
  }
  // hand
  if (packetBuffer[6] < 0) {
    driveMotor(hand, 1, absolute(packetBuffer[6]));
  } else {
    driveMotor(hand, -1, absolute(packetBuffer[6]));
  }
  /*
  
  // replace prev with next
  for (int j = 0; j < 7; j++) {
    prevArmPos[j] = nextArmPos[j];
  }
  */
}

int abosulte(int in) {
   if (in > 0) {
     return in;
   } else {
     return -in;
   }
}
