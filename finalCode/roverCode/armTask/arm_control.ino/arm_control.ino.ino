
#define ARM_PACK_START 100

uint16_t prevArmPos[7];

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

}
