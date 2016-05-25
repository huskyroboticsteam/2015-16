
#define ARM_PACK_START 4
#define SHOLDER_ROT 0
#define SHOLDER 1
#define ELBOW 2
#define ELBOWROT 3
#define WRIST 4
#define WRISTROT 5
#define HAND 6

#define SPEED_SCALAR 
int16_t prevArmPos[7];

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
    driveMotor(SHOLDER_ROT, 1, Math.abs(diffArmPos[SHOLDER_ROT]);
  } else {
    driveMotor(SHOLDER_ROT, 0, Math.abs(diffArmPos[SHOLDER_ROT]);
  }
  // sholder
  if (diffArmPos[SHOLDER] < 0) {
    driveMotor(SHOLDER, 1, Math.abs(diffArmPos[SHOLDER]);
  } else {
    driveMotor(SHOLDER, 0, Math.abs(diffArmPos[SHOLDER]);
  }
  // elbow
  if (diffArmPos[SHOLDER_ROT] < 0) {
    driveMotor(ELBOW, 1, Math.abs(diffArmPos[ELBOW]);
  } else {
    driveMotor(ELBOW, 0, Math.abs(diffArmPos[ELBOW]);
  }
  // elbow_rot
  if (diffArmPos[ELBOW_ROT] < 0) {
    driveMotor(ELBOW_ROT, 1, Math.abs(diffArmPos[ELBOW_ROT]);
  } else {
    driveMotor(ELBOW_ROT, 0, Math.abs(diffArmPos[ELBOW_ROT]);
  }
  // wrist
  if (diffArmPos[WRIST] < 0) {
    driveMotor(WRIST, 1, Math.abs(diffArmPos[WRIST]);
  } else {
    driveMotor(WRIST, 0, Math.abs(diffArmPos[WRIST]);
  }
  // wrist_rot
  if (diffArmPos[WRIST_ROT] < 0) {
    driveMotor(WRIST_ROT, 1, Math.abs(diffArmPos[WRIST_ROT]);
  } else {
    driveMotor(WRIST_ROT, 0, Math.abs(diffArmPos[WRIST_ROT]);
  }
  // hand
  if (diffArmPos[HAND] < 0) {
    driveMotor(HAND, 1, Math.abs(diffArmPos[HAND]);
  } else {
    driveMotor(HAND, 0, Math.abs(diffArmPos[HAND]);
  }
  
  // replace prev with next
  for (int j = 0; j < 7; j++) {
    prevArmPos[j] = nextArmPos[j];
  }
  
}

void setup() {
  // put your setup code here, to run once:
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
}
