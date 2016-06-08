#include <Adafruit_PWMServoDriver.h>

void initializeArm() {
  pwm.setPWM(SHOLDER_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY);
  pwm.setPWM(SHOLDER, 0, ARM_TALON_NEUTRAL_FREQUENCY);
  pwm.setPWM(ELBOW, 0, ARM_TALON_NEUTRAL_FREQUENCY - 10);
  pwm.setPWM(ELBOW_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY - 10);
  pwm.setPWM(WRIST, 0, ARM_TALON_NEUTRAL_FREQUENCY);
  pwm.setPWM(WRIST_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY);
  pwm.setPWM(HAND, 0, ARM_TALON_NEUTRAL_FREQUENCY);
}

void makeArmMove() {
  /*
  for (int i = 6; i < 13; i++) {
    Serial.print(i); Serial.print(": "); Serial.println(packetBuffer[i] + 0);
  }
  */
  pwm.setPWM(SHOLDER_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[6]) - 1) * ARM_SPEED * 1.25);
  pwm.setPWM(SHOLDER, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[7]) - 1) * ARM_SPEED * .75);
  pwm.setPWM(ELBOW, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[8]) - 1) * ARM_SPEED);
  pwm.setPWM(ELBOW_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[9]) - 1) * ARM_SPEED); 
  pwm.setPWM(WRIST, 0, ARM_TALON_NEUTRAL_FREQUENCY - (((int)packetBuffer[10]) - 1) * ARM_SPEED);
  pwm.setPWM(WRIST_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[11]) - 1) * ARM_SPEED);
  pwm.setPWM(HAND, 0, ARM_TALON_NEUTRAL_FREQUENCY + (((int)packetBuffer[12]) - 1) * ARM_SPEED * 2);
       
}

