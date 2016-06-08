/*#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

EthernetUDP Udp;
byte MAC_ADDRESS[] = {0x8B, 0xA2, 0x00, 0xDA, 0x3D, 0x90};
IPAddress IP(192, 168, 1, 53);

int LED = 13;   //Whatever pin the LED is connected to

void setup() {
    pinMode(LED, OUTPUT);
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
    Serial.begin(9600);
}*/

