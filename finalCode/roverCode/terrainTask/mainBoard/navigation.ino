#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <LSM303.h>

IPAddress ipComputer(192, 168, 1, 40);
const int sentenceSize = 451;
char sentence[sentenceSize];
LSM303 compass;
bool magDone = false;
bool potDone = false;
char result[32];

void initializeNavigation()
{
    pinMode(A6, INPUT);

    Wire.begin();
    compass.init();
    compass.enableDefault();

    compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
    compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void getNavigationData()
{
    getMagData();
    if (magDone) {
        getPotentiometerData();
    }
    //mark end of result
    result[strlen(result)] = '\0';
    Udp.beginPacket(ipComputer, DESTINATION_PORT);
    Udp.write(result);
    Udp.endPacket();
    resetData();
}

void resetData()
{
  magDone = false;
  potDone = false;

  // No idea why doesn't strlen(result) work now.
  for(int i = 0 ; i < 31; i++) {
    result[i] = '\0'; 
  }
}

//reads magnetometer data and puts it in buffer
void getMagData()
{ 
    //get heading from compass
    compass.read();  
    float heading = compass.heading();

    Serial.println(heading);
    //get digits of heading separately
    int hundred = (int)heading/100;
    int ten = (int)heading%100/10;
    int one = (int)heading%10;

    //put heading in char array and send over udp
    result[strlen(result)] = (char)hundred+48;
    result[strlen(result)] = (char)ten+48;
    result[strlen(result)] = (char)one+48;

    magDone = true;
}

//reads potentiometer data and puts it in buffer
void getPotentiometerData()
{
    
    int potPos = analogRead(POTENTIOMETER);

    //get digits of potentiometer reading separately
    int thousand = (int)potPos / 1000;
    int hundred = (int)potPos % 1000 / 100;
    int ten = (int)potPos % 100 / 10;
    int one = (int)potPos % 10;
    
    //put potentiometer reading in buffer
    result[strlen(result)] = ',';
    result[strlen(result)] = (char)thousand + 48;
    result[strlen(result)] = (char)hundred + 48;
    result[strlen(result)] = (char)ten + 48;
    result[strlen(result)] = (char)one + 48;
    potDone = true;
}
