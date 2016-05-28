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
int valid_pos = 0;

void initializeNavigation()
{
    gpsSerial.begin(9600);
    gps_setup();
}

void getNavigationData()
{
    get_pos();
    if(valid_pos && strlen(result) > 0) {
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
  for(int i = 0 ; i < 31; i++) {
    result[i] = '\0'; 
  }
  valid_pos = 0;
  gpsSerial.flush();
}

void get_pos()
{
    uint32_t timeout = millis();
    // Get a valid position from the GPS
    do {
        if (gpsSerial.available()) {
            valid_pos = gps_decode(gpsSerial.read());
        }
    } while ( (millis() - timeout < VALID_POS_TIMEOUT) && ! valid_pos) ;
}
