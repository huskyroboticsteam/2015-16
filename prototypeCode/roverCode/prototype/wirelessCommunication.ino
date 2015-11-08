// All Functions for Wireless Communication //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

// GLOBAL VARIABLES //
EthernetUDP Udp;
byte MAC_ADDRESS[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress IP(192, 168, 1, 4);

void initializeWirelessCommunication()
{
    // Serial.begin(BAUD_RATE);
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
}

void getPacket()
{
    
}

void parsePacketData()
{
    
}

