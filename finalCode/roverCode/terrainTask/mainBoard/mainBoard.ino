 #include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>

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

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x43);

Servo eye_of_sauron;

void setup()
{
  //Serial.begin(115200);
    initializePWM();
    initializeArm();
    initializeSteeringSystem();
    // initializeNavigation();
    cameraSetup();
    initializeWirelessCommunication();
    
}

void loop()
{
   receiveWirelessData();
   if (!timeoutCheck()) {
   //getNavigationData();
     if ((unsigned char)packetBuffer[1] == 1) { // is emergency stop enabled?
          if ((unsigned char)packetBuffer[0] == 1) { // is potentiometer enabled?
              readCurrentAngle();
              calculateMotorSpeeds();
          }
          else {
              emergencyCalculateSpeeds();
          }
          writeToMotors();
      }
      makeArmMove();
      moveCameras();
   }
}

void initializePWM() {
  pwm.begin();
  pwm.setPWMFreq(333);
  yield();
}

