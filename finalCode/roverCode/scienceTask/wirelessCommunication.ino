// All Functions for Wireless Communication //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define ALE_PIN 4
#define ELEV_PIN 2

/**/

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
    if(packetSize == 3) {
                hasIP = true;        
        Udp.read(packetBuffer, 96);      
        return true;
    }
    return false;
}

void timeoutCheck()
{
    if(millis() - timeLastPacket >= TIMEOUT) {
        drill.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
        augar.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    }
}

