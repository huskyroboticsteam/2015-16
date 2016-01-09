// All Functions for Driving the Motors //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

void calculateMotorSpeeds()
{

    
   /* int angleDifference = 0;
    readCurrentAngle();
    motorRatio = ((VERTICAL_WHEEL_SEPARATION / 2 * tan(currentAngle)) + (0.5)(LATERAL_WHEEL_SEPARATION)) / ((VERTICAL_WHEEL_SEPARATION / 2 * tan(currentAngle) - (0.5)(LATERAL_WHEEL_SEPARATION));
    angleDifference = TUNING_CONSTANT * (currentAngle - inputAngle);// difference in angle (input - actual)
    // adjust morot speeds with error difference * constant
    frontRightVal = motorRatio * speed;
    frontLeftVal = motorRatio * speed;
    backRightVal = motorRatio * speed;
    backLeftVal = motorRatio * speed;*/
}

void writeToMotors()
{
    if(networkStatus == true) {
        if(inputAngle > 8 || inputAngle < -8) {
            frontRightVal = (speed + inputAngle);
            frontLeftVal = (speed - inputAngle);
            backRightVal = (speed - inputAngle);
            backLeftVal = (speed + inputAngle);
        }
        else
        {
            frontRightVal = speed;
            frontLeftVal = speed;
            backRightVal = speed;
            backLeftVal = speed;
        }
        timeLastPacket = millis();
        frontRight.writeMicroseconds(map(frontRightVal, -100, 100, 2000, 1000));
        frontLeft.writeMicroseconds(map(frontLeftVal, -100, 100, 2000, 1000));
        backRight.writeMicroseconds(map(backRightVal, -100, 100, 2000, 1000));
        backLeft.writeMicroseconds(map(backLeftVal, -100, 100, 1000, 2000));
    }
}

void initializeSteeringSystem()
{
    frontRight.attach(FRONT_RIGHT_PIN);
    frontLeft.attach(FRONT_LEFT_PIN);
    backRight.attach(BACK_RIGHT_PIN);
    backLeft.attach(BACK_LEFT_PIN);
}

void readCurrentAngle()
{
    currentAngle = analogRead(FEEDBACK_POTENTIOMETER_PIN);
    currentAngle = (currentAngle, 0, 1024, -45, 45);
}

