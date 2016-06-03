#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include "Adafruit_MAX31855.h"

EthernetUDP Udp;
byte MAC_ADDRESS[] = {0x90, 0xA4, 0xDA, 0x00, 0x3D, 0x8B};
//byte MAC_ADDRESS[] = {0x90, 0xA2, 0xDA, 0x00, 0x3D, 0x8B};
IPAddress IP(192, 168, 1, 53);
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
bool networkStatus = true;
bool hasIP = false;
unsigned long timeLastPacket; // to be set to millis() in main code

int drillgo = 128;
int therm_DO = 5;
int therm_CS = 4;
int therm_CLK = 3;
int maxW = 1024; //this value will need to change

Adafruit_MAX31855 thermocouple(therm_CLK, therm_CS, therm_DO);

Servo augar;
Servo drill;

void setup()
{
    initializeScience();
    initializeWirelessCommunication();
    Serial.begin(9600);
}

void loop()
{
    receiveWirelessData();
    calculateScienceSpeeds();
    temp();
    soil();
    writeToAugar();
    collect();
    timeoutCheck();
}

