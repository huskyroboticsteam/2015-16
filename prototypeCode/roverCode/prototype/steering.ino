// All Functions for Driving the Motors //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

void calculateMotorSpeeds()
{
    inputAngle = map(((unsigned char)packetBuffer[2]) & 0xFFFF, 0, 255, 45, -45);
    bool negInput = false; // keeps track of negative inputs (to turn right)
    if(inputAngle < 0) { // range is uneven, needs to even out the negative side
        inputAngle -= 1;
        negInput = true;
    }
    inputAngle = (int) 35 * pow(2, (abs(inputAngle)-25)/15.0) - 11.024; // equation to change the speed exponentially left/right
    if (negInput) { // to get back negative input, if needed
        inputAngle *= -1;
    }
    
    speed = map(((unsigned char)packetBuffer[3]) & 0xFFFF, 0, 255, 100, -100);
    bool negSpeed = false; // keeps track of negative inputs (to go backwards)
    if(speed < 0) { // range is uneven, needs to even out the negative side
        speed -= 1;
        negSpeed = true;
    }
    speed = (int) 10 * pow(2, (abs(speed)-25)/10.0) - 1.7; // equation to change the speed exponentially forward/back
    if (negSpeed) { // to get back negative input, if needed
        speed *= -1;
    }

    // left is positibe and right is negative
    if(currentAngle <= 140) { // state 1 - going left
        if(inputAngle > 8) { // doesn't want to keep turning left
            frontRightVal = speed;
            frontLeftVal = speed; 
            backRightVal = speed;
            backLeftVal = speed;
        } else { // turning the other way (right)
            frontRightVal = (speed + inputAngle);
            frontLeftVal = (speed - inputAngle); 
            backRightVal = (speed - inputAngle);
            backLeftVal = (speed + inputAngle);
        }
    } else if (currentAngle >= 610) { // state 3 - going right
        if(inputAngle < -8) { // doesn't want to keep turning right
            frontRightVal = speed;
            frontLeftVal = speed; 
            backRightVal = speed;
            backLeftVal = speed;
        } else { // turning the other way (left)
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
    bool negInput = false; // keeps track of negative inputs (to turn right)
    if(inputAngle < 0) { // range is uneven, needs to even out the negative side
        inputAngle -= 1;
        negInput = true;
    }
    inputAngle = (int) 35 * pow(2, (abs(inputAngle)-25)/15.0) - 11.024; // equation to change the speed exponentially
    if (negInput) { // to get back negative input, if needed
        inputAngle *= -1;
    }
    
    speed = map(((unsigned char)packetBuffer[3]) & 0xFFFF, 0, 255, 100, -100);
    bool negSpeed = false; // keeps track of negative inputs (to go backwards)
    if(speed < 0) { // range is uneven, needs to even out the negative side
        speed -= 1;
        negSpeed = true;
    }
    speed = (int) 10 * pow(2, (abs(speed)-25)/10.0) - 1.7; // equation to change the speed exponentially
    if (negSpeed) { // to get back negative input, if needed
        speed *= -1;
    }

    frontRightVal = (speed + inputAngle);
    frontLeftVal = (speed - inputAngle); 
    backRightVal = (speed - inputAngle);
    backLeftVal = (speed + inputAngle);
}

void writeToMotors()
{
    if(networkStatus == true) {
        timeLastPacket = millis(); 
        frontRight.writeMicroseconds(map(frontRightVal, -100, 100, 2000, 1000));
        frontLeft.writeMicroseconds(map(frontLeftVal, -100, 100, 2000, 1000));
        backRight.writeMicroseconds(map(backRightVal, -100, 100, 2000, 1000));
        backLeft.writeMicroseconds(map(backLeftVal, -100, 100, 1000, 2000));
    }
}

void initializeSteeringSystem()
{
    backRight.attach(MOTOR_1);
    frontLeft.attach(MOTOR_2);
    backLeft.attach(MOTOR_3);
    frontRight.attach(MOTOR_4);
}

void readCurrentAngle()
{
    currentAngle = analogRead(POTENTIOMETER);
}
