#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

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

