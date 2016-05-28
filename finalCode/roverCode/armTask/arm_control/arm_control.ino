 #include "arm_control.h"
#include <Adafruit_PWMServoDriver.h>
#include <I2cDiscreteIoExpander.h>
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

void initArm();
void enableMotors();
void calculateArmPos();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("1");
  delay(100);
  //Serial.println("2");
  initArm();
  //Serial.println("3");
  delay(100);
  //Serial.println("4");
  enableMotors();
  //Serial.println("5");
  delay(100);
  //Serial.println("6");
  
  
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
  // calculateArmPos();
  /*
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
  */
  
  driveMotor(7, 1, 10);
  driveMotor(1, 1, 10);
  driveMotor(2, 1, 10);
  driveMotor(3, 1, 10);
  driveMotor(4, 1, 10);
  driveMotor(5, 1, 10);
  driveMotor(6, 1, 10);
        
  delay(1000);
  stopMotor(1);
  stopMotor(2);
  stopMotor(3);
  stopMotor(4);
  stopMotor(5);
  stopMotor(6);
  stopMotor(7);

  delay(1000);
  driveMotor(7, 0, 20);
  driveMotor(1, 0, 20);
  driveMotor(2, 0, 20);
  driveMotor(3, 0, 20);
  driveMotor(4, 0, 20);
  driveMotor(5, 0, 20);
  driveMotor(6, 0, 20);
        
  delay(1000);
  stopMotor(1);
  stopMotor(2);
  stopMotor(3);
  stopMotor(4);
  stopMotor(5);
  stopMotor(6);
  stopMotor(7);
        
  delay(1000);
  
  /*
  driveMotor(2, 0, 20);
  delay(1000);
  */
  
}
