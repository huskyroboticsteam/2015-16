// All Functions for Wireless Communication //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define ALE_PIN 4
#define ELEV_PIN 2

const int xLow = 1067;
const int xHigh = 1905;
const int yLow = 1066;
const int yHigh = 1905;

void initializeWirelessCommunication()
{
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
    timeLastPacket = millis();
    pinMode(ALE_PIN, INPUT);
    pinMode(ELEV_PIN, INPUT);
    Serial.begin(9600);
}


void receiveWirelessData()
{
    networkStatus = parsePacketData();
}


bool parsePacketData()
{
    int packetSize = Udp.parsePacket();
    if(packetSize == 2) {
                hasIP = true;
        Udp.read(packetBuffer, 96);

        inputAngle = map(((unsigned char)packetBuffer[0]) & 0xFFFF, 0, 255, 45, -45);
        bool negInput = false; // keeps track of negative inputs (to turn right)
        if(inputAngle < 0) { // range is uneven, needs to even out the negative side
            inputAngle -= 1;
            negInput = true;
        }
        inputAngle = (int) 32 * pow(2, (abs(inputAngle)-25)/5.0) - 0.78; // equation to change the speed exponentially
        if (negInput) { // to get back negative input, if needed
            inputAngle *= -1;
        }
        
        speed = map(((unsigned char)packetBuffer[1]) & 0xFFFF, 0, 255, 100, -100);
        bool negSpeed = false; // keeps track of negative inputs (to go backwards)
        if(speed < 0) { // range is uneven, needs to even out the negative side
            speed -= 1;
            negSpeed = true;
        }
        speed = (int) 10 * pow(2, (abs(speed)-25)/10.0) - 1.7; // equation to change the speed exponentially
        if (negSpeed) { // to get back negative input, if needed
            speed *= -1;
        }
        return true;
    }
    return false;
}

void receiveDX6iData()
{
    int valuex = pulseIn(ALE_PIN, HIGH);
    int valuey = pulseIn(ELEV_PIN, HIGH);
    inputAngle = map(valuex, xLow, xHigh, 45, -45);
    speed = map(valuey, yLow, yHigh, -100, 100);
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

