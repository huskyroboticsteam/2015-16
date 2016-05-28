#include "arm_control.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <I2cDiscreteIoExpander.h>

I2cDiscreteIoExpander direction(6);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x7);

uint16_t dirState;

// play around with these values to find the best
// maybe use -100 to 100
#define MIN_SPEED  0
#define MAX_SPEED  100

void initArm() {

  //Set all IO Expander outputs low except standby which is set high (disables Motors)
  dirState = 0x02;
  direction.digitalWrite(dirState);

  pwm.begin();
  pwm.setPWMFreq(1000);
}

void enableMotors() {
  dirState = 0x00;
  direction.digitalWrite(dirState);
}

void disableMotors() {
  dirState = 0x02;
  direction.digitalWrite(dirState);
}

void driveMotor(int mot, int dir, int rawSpeed)
{

  double driveSpeed = map(rawSpeed, MIN_SPEED, MAX_SPEED, 0, 4096);


  switch (mot) {
    case 0:
      //b5 (15) , b6 (14) , pwmb56 (5)
      if (dir == 1) {
        dirState |= _BV(13);
        dirState &= ~(_BV(12));
      } else if (dir == 0) {
        dirState &= ~(_BV(13));
        dirState |= (_BV(12));
      }
      pwm.setPin(5, driveSpeed, false);
      break;
    case 1:
      //a6 (12) , a5 (13) , pwma56 (4)
      if (dir == 1) {
        dirState |= _BV(10);
        dirState &= ~(_BV(11));
      } else if (dir == 0) {
        dirState &= ~(_BV(10));
        dirState |= (_BV(11));
      }
      pwm.setPin(4, driveSpeed, false);
      break;
    case 2:
      //b3 (11) , b4 (10) , pwmb34 (3)
      if (dir == 1) {
        dirState |= _BV(9);
        dirState &= ~(_BV(8));
      } else if (dir == 0) {
        dirState &= ~(_BV(9));
        dirState |= (_BV(8));
      }
      pwm.setPin(3, driveSpeed, false);
      break;
    case 3:
      //a3 (7) , a4 (6) , pwma34 (2)
      if (dir == 1) {
        dirState |= _BV(7);
        dirState &= ~(_BV(6));
      } else if (dir == 0) {
        dirState &= ~(_BV(7));
        dirState |= (_BV(6));
      }
      pwm.setPin(2, driveSpeed, false);
      break;
    case 4:
      //b1 (4) , b2 (5) , pwmb12 (1)
      if (dir == 1) {
        dirState |= _BV(4);
        dirState &= ~(_BV(5));
      } else if (dir == 0) {
        dirState &= ~(_BV(4));
        dirState |= (_BV(5));
      }
      pwm.setPin(1, driveSpeed, false);
      break;
    case 5:
      //a1 (2) , a2 (3) , pwma12 (0)
      if (dir == 1) {
        dirState |= _BV(2);
        dirState &= ~(_BV(3));
      } else if (dir == 0) {
        dirState &= ~(_BV(2));
        dirState |= (_BV(3));
      }
      pwm.setPin(0, driveSpeed, false);
      break;
    case 6:
      //a7 (17) , a8 (16) , pwma7 (6)
      if (dir == 1) {
        dirState |= _BV(15);
        dirState &= ~(_BV(14));
      } else if (dir == 0) {
        dirState &= ~(_BV(15));
        dirState |= (_BV(14));
      }
      pwm.setPin(6, driveSpeed, false);
      break;
    default:
      break;
      
    direction.digitalWrite(dirState);
  }
}

void stopMotor(int mot) {
  driveMotor(mot, 0, 0);
}

int absolute(int input) {
  if (input < 0) {
    return -input;
  }
  return input;
}

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
    driveMotor(SHOLDER_ROT, 1, absolute(diffArmPos[SHOLDER_ROT]));
  } else {
    driveMotor(SHOLDER_ROT, 0, absolute(diffArmPos[SHOLDER_ROT]));
  }
  // sholder
  if (diffArmPos[SHOLDER] < 0) {
    driveMotor(SHOLDER, 1, absolute(diffArmPos[SHOLDER]));
  } else {
    driveMotor(SHOLDER, 0, absolute(diffArmPos[SHOLDER]));
  }
  // elbow
  if (diffArmPos[ELBOW] < 0) {
    driveMotor(ELBOW, 1, absolute(diffArmPos[ELBOW]));
  } else {
    driveMotor(ELBOW, 0, absolute(diffArmPos[ELBOW]));
  }
  // elbow_rot
  if (diffArmPos[ELBOW_ROT] < 0) {
    driveMotor(ELBOW_ROT, 1, absolute(diffArmPos[ELBOW_ROT]));
  } else {
    driveMotor(ELBOW_ROT, 0, absolute(diffArmPos[ELBOW_ROT]));
  }
  // wrist
  if (diffArmPos[WRIST] < 0) {
    driveMotor(WRIST, 1, absolute(diffArmPos[WRIST]));
  } else {
    driveMotor(WRIST, 0, absolute(diffArmPos[WRIST]));
  }
  // wrist_rot
  if (diffArmPos[WRIST_ROT] < 0) {
    driveMotor(WRIST_ROT, 1, absolute(diffArmPos[WRIST_ROT]));
  } else {
    driveMotor(WRIST_ROT, 0, absolute(diffArmPos[WRIST_ROT]));
  }
  // hand
  if (diffArmPos[HAND] < 0) {
    driveMotor(HAND, 1, absolute(diffArmPos[HAND]));
  } else {
    driveMotor(HAND, 0, absolute(diffArmPos[HAND]));
  }
  
  // replace prev with next
  for (int j = 0; j < 7; j++) {
    prevArmPos[j] = nextArmPos[j];
  }
  
}
