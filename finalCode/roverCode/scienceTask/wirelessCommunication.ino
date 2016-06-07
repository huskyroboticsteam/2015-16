// All Functions for Wireless Communication //
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define ALE_PIN 4
#define ELEV_PIN 2


void initializeWirelessCommunication()
{
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
    timeLastPacket = millis();
    pinMode(ALE_PIN, INPUT);
    pinMode(ELEV_PIN, INPUT);
}


void receiveWirelessData()
{
    networkStatus = parsePacketData();
}


bool parsePacketData()
{
    int packetSize = Udp.parsePacket();
    if(packetSize == 4) {
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
        turntable.write(0);
    }
}

void sendData(int mode) {
  if (mode == 1) {
    C *= 10;
    int c = (int)C;
    for (int i = 100; i <= c; c += 100) {
      digitalWrite(TEMP_RED, HIGH);
      delay(300);
      digitalWrite(TEMP_RED, LOW);
      delay(300);
    }
    delay(300);
    c %= 100;
    for (int i = 10; i <= c; c += 10) {
      digitalWrite(TEMP_GREEN, HIGH);
      delay(300);
      digitalWrite(TEMP_GREEN, LOW);
      delay(300);
    }
    delay(300);
    c %= 10;
    for (int i = 1; i <= c; c++) {
      digitalWrite(TEMP_BLUE, HIGH);
      delay(300);
      digitalWrite(TEMP_BLUE, LOW);
      delay(300);
    }
    delay(1000);
    int hum = (int)humidity;
    for (int i = 100; i <= hum; hum += 100) {
      digitalWrite(HUM_RED, HIGH);
      delay(300);
      digitalWrite(HUM_RED, LOW);
      delay(300);
    }
    c %= 100;
    delay(300);
    for (int i = 10; i <= hum; hum += 10) {
      digitalWrite(HUM_GREEN, HIGH);
      delay(300);
      digitalWrite(HUM_GREEN, LOW);
      delay(300);
    }
    delay(300);
    c %= 10;
    for (int i = 1; i <= hum; hum++) {
      digitalWrite(HUM_GREEN, HIGH);
      delay(300);
      digitalWrite(HUM_GREEN, LOW);
      delay(300);
    }
  }
  
}

