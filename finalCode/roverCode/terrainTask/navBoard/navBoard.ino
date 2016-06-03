#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "gps.h"
#include <SoftwareSerial.h>

EthernetUDP Udp;
byte MAC_ADDRESS[] = {0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99};
IPAddress IP(192, 168, 1, 52);
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
bool networkStatus = true;
bool hasIP = false;
unsigned long timeLastPacket; // to be set to millis() in main code

void setup()
{
    initializeWirelessCommunication();
    initializeNavigation();
}

void loop()
{
   getNavigationData();
}

