#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>

void initializeScience() {
    augar.attach(motor_5);
    drill.attach(motor_6);
}
void calculateScienceSpeeds() {
    drillgo = map(((unsigned char)packetBuffer[4]) & 0xFFFF, 0, 255, 100, -100);
}

void writeToAugar() {
    augar.writeMicroseconds(map(drillgo, -100, 100, 2000, 1000));
    
}

void collect() {
    if ((unsigned char)packetBuffer[5] == 1) 
    {
        drill.writeMicroseconds(1800);
    }
    else if ((unsigned char)packetBuffer[6] == 1)
    {
        drill.writeMicroseconds(1200);
    } else {
        drill.writeMicroseconds(1500);
    }
}
