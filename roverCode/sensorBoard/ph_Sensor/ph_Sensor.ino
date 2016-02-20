#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>

#include <EthernetUdp.h>
// Enter a MAC address and IP address for your controller below.

// The IP address will be dependent on your local network:

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99 };

IPAddress ipArduino(192, 168, 1, 51);
IPAddress ipComputer(192, 168, 1, 5);

float Intercept = 13.720;
float Slope = -3.838;
unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP

EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac,ipArduino);
  Udp.begin(localPort);
  Wire.begin();
}

void loop() {

  //calculate pH from original analog reading
  float Count = analogRead(A0);
  float Voltage = Count / 1023 * 5.0;// convert from count to raw voltage
  float SensorReading = Intercept + Voltage * Slope; //converts voltage to sensor reading
  
  Udp.beginPacket(ipComputer, 8888);
  
  //get digits of pH
  int ten = (int)SensorReading%100/10;
  int one = (int)SensorReading%10;
  int tenth = (int)(SensorReading*10)%10;
  int hundredth = (int)(SensorReading*100)%10;

  //put pH in array and send over udp
  char arr[4];
  arr[0] = (char)ten+48;
  arr[1] = (char)one+48;
  arr[2] = (char)tenth+48;
  arr[3] = (char)hundredth+48;
  Udp.write(arr);
  Udp.endPacket();
  delay(100);
}