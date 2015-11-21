#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
int frontRightVal;
int frontLeftVal;
int backRightVal;
int backLeftVal;

// GLOBAL VARIABLES //


void setup()
{
    initializeWirelessCommunication();
}

void loop()
{
    getPacket();
    parsePacketData();
    calculateMotorSpeeds();
    writeToMotors();
}

