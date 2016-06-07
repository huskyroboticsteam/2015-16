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
    if(packetSize == 13) {
        //Serial.println(13);
        hasIP = true;
        Udp.read(packetBuffer, 96);
        timeLastPacket = millis();
        return true;
    }
    return false;
}

bool timeoutCheck()
{
    if(millis() - timeLastPacket >= TIMEOUT) {
        frontRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        frontLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        backRight.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        backLeft.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        pwm.setPWM(SHOLDER_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY);
        pwm.setPWM(SHOLDER, 0, ARM_TALON_NEUTRAL_FREQUENCY);
        pwm.setPWM(ELBOW, 0, ARM_TALON_NEUTRAL_FREQUENCY - 10);
        pwm.setPWM(ELBOW_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY - 10);
        pwm.setPWM(WRIST, 0, ARM_TALON_NEUTRAL_FREQUENCY);
        pwm.setPWM(WRIST_ROT, 0, ARM_TALON_NEUTRAL_FREQUENCY);
        pwm.setPWM(HAND, 0, ARM_TALON_NEUTRAL_FREQUENCY);
        return true;
    }
    return false;
}

