// All Functions for Wireless Communication //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

const int xLow = 1067;
const int xHigh = 1905;
const int yLow = 1066;
const int yHigh = 1905;

void initializeWirelessCommunication()
{
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
    timeLastPacket = millis();
}

void receiveWirelessData()
{
    networkStatus = parsePacketData();
//    Serial.println((char)packetBuffer[2]);
}

bool parsePacketData()
{
    int packetSize = Udp.parsePacket();
    if(packetSize == 6) {
        hasIP = true;
        Udp.read(packetBuffer, 96);
        return true;
    }
    return false;
}

void timeoutCheck()
{
    if(millis() - timeLastPacket >= TIMEOUT) {
        frontRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        frontLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        backRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        backLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    }
}

