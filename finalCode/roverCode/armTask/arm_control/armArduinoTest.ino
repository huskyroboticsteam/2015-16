#include "armControl.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <I2cDiscreteIoExpander.h>


I2cDiscreteIoExpander direction(6);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x7);

uint16_t dirState;

// play around with these values to find the best
// maybe use -100 to 100
#define MIN_SPEED = 0
#define MAX_SPEED = 100


void setup() {
    
}

void loop() {
  
  
}






void initArm() {

  //Set all IO Expander outputs low except standby which is set high (disables Motors)
  dirState = 0x02;
  direction.digitalWrite(dirState);

  pwm.begin();
  pwm.setPWMFreq(1000);
  
  /*#ifdef TWBR
    uint8_t twbrbackup = TWBR;
    TWBR = 12;
  #endif*/
  
  
}

void enableMotors(){
  dirState = 0x00;
  direction.digitalWrite(dirState);
}

void disableMotors(){
  dirState = 0x02;
  direction.digitalWrite(dirState);
}

void driveMotor (int mot , int dir , int rawSpeed){
 
  double driveSpeed = map(rawSpeed , MIN_SPEED , MAX_SPEED , 0 , 4096);


  switch(mot) {
    case 0:
      //b5 (15) , b6 (14) , pwmb56 (5)
      if(dir == 1) {
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
      if(dir == 1) {
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
      if(dir == 1) {
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
      if(dir == 1) {
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
      if(dir == 1) {
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
      if(dir == 1) {
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
      if(dir == 1) {
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


void stopMotor (int mot) {
  driveMotor (mot , 0 , 0);
}

