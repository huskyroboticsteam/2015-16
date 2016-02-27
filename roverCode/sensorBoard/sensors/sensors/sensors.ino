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

float intercept = 13.720;
float slope = -3.838;
unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP

EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac,ipArduino);
  Udp.begin(localPort);
  Wire.begin();
  Serial.begin(9600);
}

void loop() {

  //calculate pH from original analog reading
  float count = analogRead(A0);
  float voltage = count / 1023 * 5.0;// convert from count to raw voltage
  float pH = intercept + voltage * slope; //converts voltage to sensor reading
  Udp.beginPacket(ipComputer, localPort);
  
  //get digits of pH
  int phTen = (int)pH%100/10;
  int phOne = (int)pH%10;
  int phTenth = (int)(pH*10)%10;
  int phHundredth = (int)(pH*100)%10;

  //get digits of humidity
  float humidity = analogRead(A1);
  int humidityHundred = (int)humidity/100;
  int humidityTen = (int)humidity%100/10;
  int humidityOne = (int)humidity%10;

  //put pH in array and send over udp
  char sensorData[9];
  sensorData[0] = (char)phTen+48;
  sensorData[1] = (char)phOne+48;
  sensorData[2] = (char)phTenth+48;
  sensorData[3] = (char)phHundredth+48;
  sensorData[4] = ',';
  sensorData[5] = (char)humidityHundred;
  sensorData[6] = (char)humidityTen;
  sensorData[7] = (char)humidityOne;
  sensorData[8] = '\0';
  Serial.println(sensorData);
  Udp.write(sensorData);
  Udp.endPacket();
  delay(100);
}
