
#ifndef _ARM_CONTROL_
#define _ARM_CONTROL_
#define UDP_PORT 8888
#define DESTINATION_PORT 8887
#define TIMEOUT 250
#define TALON_NEUTRAL_FREQUENCY 1500
#define BAUD_RATE 9600


#define SHOLDER_ROT 2
#define SHOLDER 3
#define ELBOW 5
#define ELBOW_ROT 6
#define WRIST 7
#define WRIST_ROT 8
#define HAND 9 
/*
void initArm();

void enableMotors();

void disableMotors();

// drives the motor in the direction indicated at the speed indicated
void driveMotor(Servo mot, int dir, int rawSpeed);

void stopMotor(Servo mot);

// calculates and writes arm positions
void calculateArmPos();
*/

#endif  // _ARM_CONTROL_
