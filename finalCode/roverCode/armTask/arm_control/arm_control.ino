#include "Arduino.h"
#include <Servo.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <Wire.h>

EthernetUDP Udp;
byte MAC_ADDRESS[] = {0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99};
IPAddress IP(192, 168, 1, 7);
bool networkStatus = true;
bool hasIP = false;
unsigned long timeLastPacket;


Servo sholder_rot; 
Servo sholder; 
Servo elbow; 
Servo elbow_rot; 
Servo wrist; 
Servo wrist_rot; 
Servo hand; 

char packetBuffer[7];

void setup() {
  Serial.begin(115200);
  initArm();
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
   initializeWirelessCommunication();
  
  // initializeArmTest();
}

void loop() {
  // put your main code here, to run repeatedly:
  receiveWirelessData();
  // calculateArmPos();

  // calculateArmTest();
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  
  int8_t data[11];
  int i = 0;
  while (0 < Wire.available()) { // loop through all but the last
    int8_t c = Wire.read(); // receive byte as a character
    c -= 127;
    data[i] = c;
  }
  int8_t data_Arm[7];
  for (int j = 4; j < 11; j++) {
    data_Arm[j - 4] = data[j];
  }
  calculateArmPos(data_Arm);
/*
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);
  }
  char x = Wire.read();
  Serial.println(x);
*/
}
