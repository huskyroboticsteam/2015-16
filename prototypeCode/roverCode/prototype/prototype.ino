#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>

EthernetUDP Udp;
byte MAC_ADDRESS[] = {0x90, 0xA2, 0xDA, 0x00, 0x3D, 0x8B};
IPAddress IP(192, 168, 1, 51);
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
bool networkStatus = true;
bool hasIP = false;
bool useDX6i = true;
unsigned long timeLastPacket; // to be set to millis() in main code

int inputAngle  = 10;
int speed  = 128;
int motorRatio = 1;
int currentAngle = 0;

Servo frontRight;
Servo frontLeft;
Servo backRight;
Servo backLeft;
int frontRightVal = 0;
int frontLeftVal = 0;
int backRightVal = 0;
int backLeftVal = 0;

void setup()
{
    initializeWirelessCommunication();
    initializeSteeringSystem();
}

void loop()
{
    receiveDX6iData();
//  receiveWirelessData();
//  calculateMotorSpeeds();
    writeToMotors();
    timeoutCheck();
}

