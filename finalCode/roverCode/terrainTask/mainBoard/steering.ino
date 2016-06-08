// All Functions for Driving the Motors //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define RIGHT_PET 110

#define LEFT_PET 143

    
void calculateMotorSpeeds()
{
    inputAngle = map(((unsigned char)packetBuffer[2]) & 0xFFFF, 0, 255, 45, -45);
    inputAngle *= 0.5; // Makes the linear turning slower.
    
    speed = map(((unsigned char)packetBuffer[3]) & 0xFFFF, 0, 255, 100, -100);
    speed /= 2.5;
    // Min is 284 (all the way right), max is 769 (all the way left), middle is 531 (center)

    // left is positive and right is negative for inputAngle
    if(currentAngle >= 750) { // state 1 - going left
        if(inputAngle > 8) { // doesn't want to keep turning left
            frontRightVal = speed;
            frontLeftVal = speed; 
            backRightVal = speed;
            backLeftVal = speed;
        }/* else if (inputAngle > -8) { // going straight
          frontRightVal = speed;
          frontLeftVal = speed;
          backRightVal = speed + 10;
          backLeftVal = speed - 10;
        } */else { // turning the other way (right)
        
            frontRightVal = (speed + inputAngle);
            frontLeftVal = (speed - inputAngle); 
            backRightVal = (speed - inputAngle);
            backLeftVal = (speed + inputAngle);
        }
    } else if (currentAngle <= 300) { // state 3 - going right
        if(inputAngle < -8) { // doesn't want to keep turning right
            frontRightVal = speed;
            frontLeftVal = speed; 
            backRightVal = speed;
            backLeftVal = speed;
        } /*else if (inputAngle < 8) { // going straight
          frontRightVal = speed;
          frontLeftVal = speed;
          backRightVal = speed - 10;
          backLeftVal = speed + 10;
        } */else { // turning the other way (left)
            frontRightVal = (speed + inputAngle);
            frontLeftVal = (speed - inputAngle); 
            backRightVal = (speed - inputAngle);
            backLeftVal = (speed + inputAngle);
        }            
    } else { // state 2
        frontRightVal = (speed + inputAngle);
        frontLeftVal = (speed - inputAngle); 
        backRightVal = (speed - inputAngle);
        backLeftVal = (speed + inputAngle);
    }
}

void emergencyCalculateSpeeds()
{
    inputAngle = map(((unsigned char)packetBuffer[2]) & 0xFFFF, 0, 255, 45, -45);
    inputAngle *= 0.5; // Makes linear turning slower. 
    
    speed = map(((unsigned char)packetBuffer[3]) & 0xFFFF, 0, 255, 100, -100);
    speed /= 2.5;

    frontRightVal = (speed + inputAngle);
    frontLeftVal = (speed - inputAngle); 
    backRightVal = (speed - inputAngle);
    backLeftVal = (speed + inputAngle);
}

void writeToMotors()
{
    if(networkStatus == true) {
        timeLastPacket = millis();
        frontRight.writeMicroseconds(map(frontRightVal, -100, 100, 1000, 2000));
        frontLeft.writeMicroseconds(map(frontLeftVal, -100, 100, 2000, 1000));
        backRight.writeMicroseconds(map(backRightVal, -100, 100, 2000, 1000));
        backLeft.writeMicroseconds(map(backLeftVal, -100, 100, 2000, 1000));
    }
}

void initializeSteeringSystem()
{
    backRight.attach(MOTOR_1);
    frontRight.attach(MOTOR_2);
    frontLeft.attach(MOTOR_3);
    backLeft.attach(MOTOR_4);
    frontRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    frontLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    backRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    backLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
}

void readCurrentAngle()
{
    currentAngle = analogRead(POTENTIOMETER);
}

