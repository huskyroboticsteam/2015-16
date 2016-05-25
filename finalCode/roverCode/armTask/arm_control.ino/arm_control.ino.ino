
#define ARM_PACK_START 100
#define SHOLDER_ROT 0
#define SHOLDER 1
#define ELBOW 2
#define ELBOWROT 3
#define WRIST 4
#define WRISTROT 5
#define HAND 6

#define SPEED_SCALAR 
uint16_t prevArmPos[7];

// calculates the arm pos and writes to the motors
void calculateArmPos() {
  // parse the packet into the array
  uint16_t nextArmPos[7];
  for (int i = 0; i < 7; i++) {
    nextArmPos[i] = packetBuffer[ARM_PACK_START + 2 * i];
    nextArmPos[i] *= 256;
    nextArmPos[i] += packetBuffer[ARM_PACK_START + 2 * i + 1];
  }

  // if the potentiometers gets done prevArmPos will be chnaged here
  
  // calculate the difference and write to motors
  uint16_t diffArmPos[7];
  for (int k = 0; k < 7; k++) {
    diffArmPos[k] = prevArmPos[k] - nextArmPos[k];
  }
  // write to motors
  if (diffArmPos[SHOLDER_ROT
  driveMotor(SHOLDER_ROT
  
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
