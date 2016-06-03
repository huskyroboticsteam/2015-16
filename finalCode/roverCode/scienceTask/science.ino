#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
void calculateScienceSpeeds() {
    drillgo = map(((unsigned char)packetBuffer[0]) & 0xFFFF, 0, 255, 100, -100);
} 
void collect() {
    if ((unsigned char)packetBuffer[1] == 1) {
        drill.writeMicroseconds(1900);
    } else if ((unsigned char)packetBuffer[2] == 1) {
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
    drill.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
    augar.writeMicroseconds(TALON_NEUTRAL_FREQUENCY);
}

