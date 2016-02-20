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
    Serial.begin(9600);
}

void loop()
{
    
  //  Serial.println((unsigned char)packetBuffer[1]);
    //    Serial.println("hi");
   // receiveDX6iData();
    receiveWirelessData();
    Serial.print("POTENTIOMETER STATUS: ");
    Serial.println((unsigned char)packetBuffer[0]);
    Serial.print("EMERGENCY STOP STATUS: ");
    Serial.println((unsigned char)packetBuffer[1]);
    Serial.print("angle: ");
    Serial.println((unsigned char)packetBuffer[2]);
    Serial.print("speed: ");
    Serial.println((unsigned char)packetBuffer[3]);
    if ((unsigned char)packetBuffer[1] == 1)
    {
        readCurrentAngle();
        calculateMotorSpeeds();
        writeToMotors();
    }
    timeoutCheck();
}

