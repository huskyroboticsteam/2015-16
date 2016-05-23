#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <LSM303.h>
#include "gps.h"

IPAddress ipComputer(192, 168, 1, 40);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); // RX, TX (TX not used)
bool got_pos = false;
const int sentenceSize = 451;
char sentence[sentenceSize];
LSM303 compass;
bool gpsDone = false;
bool magDone = false;
bool potDone = false;
bool valid_pos = false;

void initializeNavigation()
{
    gpsSerial.begin(9600);
    gps_setup();
}

void getNavigationData()
{
    get_pos();
    if(valid_pos) {
        result[strlen(result)] = ',';
        //getPotentiometerData();
        //mark end of result
        result[strlen(result)] = '\0';
        Udp.beginPacket(ipComputer, DESTINATION_PORT);
        Udp.write(result);
        Udp.endPacket();
        Serial.println(result);
        resetData();
    }
}

void resetData()
{
  gpsDone = false;
  magDone = false;
  potDone = false;

  // No idea why doesn't strlen(result) work now.
  for(int i = 0 ; i < 31; i++) {
    result[i] = '\0'; 
  }
  valid_pos = 0;
}

//reads magnetometer data and puts it in buffer
void getMagData()
{ 
    //get heading from compass
    compass.read();  
    float heading = compass.heading();

    //get digits of heading separately
    int hundred = (int)heading/100;
    int ten = (int)heading%100/10;
    int one = (int)heading%10;

    //put heading in char array and send over udp
    result[23] = (char)hundred+48;
    result[24] = (char)ten+48;
    result[25] = (char)one+48;

    //Serial.println(result);
    magDone = true;
}

//reads potentiometer data and puts it in buffer
void getPotentiometerData()
{
    int potPos = analogRead(POTENTIOMETER);
    //Serial.println(potPos);

    //get digits of potentiometer reading separately
    int thousand = (int)potPos / 1000;
    int hundred = (int)potPos % 1000 / 100;
    int ten = (int)potPos % 100 / 10;
    int one = (int)potPos % 10;
    
    //put potentiometer reading in buffer
    result[26] = ',';
    result[27] = (char)thousand + 48;
    result[28] = (char)hundred + 48;
    result[29] = (char)ten + 48;
    result[30] = (char)one + 48;
    potDone = true;
}

void get_pos()
{
    // Get a valid position from the GPS
    uint32_t timeout = millis();
    gpsSerial.flush();
    while (gpsSerial.available()) {
       valid_pos = gps_decode(gpsSerial.read());
    }
}
