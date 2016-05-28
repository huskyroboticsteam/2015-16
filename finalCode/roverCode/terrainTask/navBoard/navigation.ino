#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "gps.h"

IPAddress ipComputer(192, 168, 1, 40);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); // RX, TX (TX not used)
bool got_pos = false;
const int sentenceSize = 451;
char sentence[sentenceSize];
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
        //mark end of result
        result[strlen(result)] = '\0';
        Udp.beginPacket(ipComputer, DESTINATION_PORT);
        Udp.write(result);
        Udp.endPacket();
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


void get_pos()
{
    // Get a valid position from the GPS
    gpsSerial.flush();
    delay(900);
    while (gpsSerial.available()) {
       valid_pos = gps_decode(gpsSerial.read());
    }
    gpsDone = true;
}
