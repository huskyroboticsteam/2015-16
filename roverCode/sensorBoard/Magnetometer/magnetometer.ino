#include <Wire.h>
#include <LSM303.h>
#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>
#include <String.h>

#include <EthernetUdp.h>
// Enter a MAC address and IP address for your controller below.

// The IP address will be dependent on your local network:

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99 };

IPAddress ip(192, 168, 1, 51);
IPAddress ipComp(192, 168, 1, 5);
//69, 91, 178, 233 192.168.1.51

unsigned int localPort = 8888;      // local port to listen on



// An EthernetUDP instance to let us send and receive packets over UDP

EthernetUDP Udp;
LSM303 compass;

void setup() {
  
  Serial.begin(9600);
  Ethernet.begin(mac,ip);

  Udp.begin(localPort);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  Serial.println("STARTED>");
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void loop() {
  compass.read();
  float heading = compass.heading();
  Udp.beginPacket(ipComp, 8888);
  char arr1[1] = {'a'};
  int hundred = (int)heading/100;
  int ten = (int)heading%100/10;
  int one = (int)heading%10;
  char arr[3];
  arr[0] = (char)hundred+48;
  arr[1] = (char)ten+48;
  arr[2] = (char)one+48;
  Udp.write(arr);
  Udp.endPacket();
  Serial.println(heading);
  delay(100);
}

