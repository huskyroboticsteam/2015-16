// All Functions for Wireless Communication //
#include "Arduino.h"
#include "arm_control.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

const int xLow = 1067;
const int xHigh = 1905;
const int yLow = 1066;
const int yHigh = 1905;

void initializeWirelessCommunication()
{
   // Serial.println("initalize wire");
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
    timeLastPacket = millis();
}

void receiveWirelessData()
{
    networkStatus = parsePacketData();
    Serial.println(networkStatus);
}

bool parsePacketData()
{
  Serial.println("parsing");
  //char packetBufferChar[14]; 
  int packetSize = Udp.parsePacket();
  Serial.println(packetSize);
  if(packetSize == 7) {
      hasIP = true;
      Udp.read(packetBuffer, 96);
      /*
      for (int i = 0; i < 7; i++) {
        packetBuffer[i] = packetBufferChar[2 * i];
        packetBuffer[i] *= 256;
        packetBuffer[i] += packetBufferChar[2 * i + 1];
      }
      */
      return true;
  }
  return false;
  
}

void timeoutCheck()
{
    if(millis() - timeLastPacket >= TIMEOUT) {
      Serial.println("timeout");
      stopMotor(sholder_rot);
      stopMotor(sholder);
      stopMotor(elbow);
      stopMotor(elbow_rot);
      stopMotor(wrist);
      stopMotor(wrist_rot);
      stopMotor(hand);
    }
}

