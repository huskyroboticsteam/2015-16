#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
void calculateScienceSpeeds() {
    drillgo = map(((unsigned char)packetBuffer[0]) & 0xFFFF, 0, 255, 100, -100);
    writeTurntable((int)packetBuffer[2]);
} 
void collect() {
    if ((unsigned char)packetBuffer[1] == 1) {
        drill.writeMicroseconds(1900);
    } else if ((unsigned char)packetBuffer[1] == 2) {
        drill.writeMicroseconds(1100);
    } else {
        drill.writeMicroseconds(1500);
    }
}
void writeToAugar() {
    augar.writeMicroseconds(map(drillgo, -100, 100, 2000, 1000));    
}
void initializeScience() {
    augar.attach(MOTOR_5);
    drill.attach(MOTOR_6);
    turntable.attach(TURNTABLE);
    writeTurntable(0);
    
    drill.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    augar.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
}

void writeTurntable(int pos) {
  if (pos == 0) {
    turntable.write(80);
  } else if( pos == 1) {
    turntable.write(0);
  } else if (pos == 2) {
    turntable.write(165);
  }
}

void initializeLEDMorse() {
  pinMode(HUM_RED, OUTPUT);
  pinMode(HUM_GREEN, OUTPUT);
  pinMode(HUM_BLUE, OUTPUT);
  pinMode(TEMP_RED, OUTPUT);
  pinMode(TEMP_GREEN, OUTPUT);
  pinMode(TEMP_BLUE, OUTPUT);
  digitalWrite(HUM_RED, LOW);
  digitalWrite(HUM_GREEN, LOW);
  digitalWrite(HUM_BLUE, LOW);
  digitalWrite(TEMP_RED, LOW);
  digitalWrite(TEMP_GREEN, LOW);
  digitalWrite(TEMP_BLUE, LOW);
}

