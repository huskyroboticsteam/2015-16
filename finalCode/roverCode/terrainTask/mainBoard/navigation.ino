/*
  A note to be considered at this moment of development.
  - This does not work completely when data has not been recieved
    from either(i.e anything breaks = everything broken).

  Connection details : 
  - Magnetometer : 
    - vin -> 5V / 3.3V
    - gnd -> Ground
    - scl -> A5
    - sda -> A4

  - Potentiometer :
    - Middle wire -> A0
    - Side wires -> Ground and 5V (interchangeable)

  - GPS : 
    - TX0 -> Port 4
    - Others are self explainatory

  - Buffer Format:
    - Order: latitude, longitude, magnetometer, potentiometer
      (in that order) separated by commas
    - degrees, minutes, and seconds of latitude and longitude
      separated by periods
*/
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <LSM303.h>

IPAddress ipComputer(192, 168, 1, 40);

const int sentenceSize = 82;
char sentence[sentenceSize];
LSM303 compass;
bool gpsDone = false;
bool magDone = false;
bool potDone = false;
char result[32];

void initializeNavigation()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();
    /*
    Calibration values; the default values of +/-32767 for each axis
    lead to an assumed magnetometer bias of 0. Use the Calibrate example
    program to determine appropriate values for your particular unit.
    */
    compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
    compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void getNavigationData()
{
    getMagData();
    if(magDone) {
      getPotentiometerData();
      if(potDone) {
        //mark end of result
        result[strlen(result)] = '\0';
        Udp.beginPacket(ipComputer, DESTINATION_PORT);
        Udp.write(result);
        Udp.endPacket();
        resetData();
       }
    }
}

void resetData()
{
  magDone = false;
  potDone = false;
  // No idea why doesn't strlen(result) work now.
  for(int i = 0 ; i < strlen(result); i++)
  {
    result[i] = '\0'; 
  }
}

//reads data from gps and puts it into buffer

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
    result[strlen(result)] = (char)hundred+48;
    result[strlen(result)] = (char)ten+48;
    result[strlen(result)] = (char)one+48;

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
    result[strlen(result)] = ',';
    result[strlen(result)] = (char)thousand + 48;
    result[strlen(result)] = (char)hundred + 48;
    result[strlen(result)] = (char)ten + 48;
    result[strlen(result)] = (char)one + 48;
    potDone = true;
}


//adds period between degrees and minutes
void addPeriod(String field, char* result, int startIndex,int splitIndex)
{ 
    result[startIndex+splitIndex] = '.';
    
    //put digits before period into the result
    for(int i=0;i<splitIndex;i++){
        result[i + startIndex] = field[i];
    }

    //put digits after period into result
    for(int i=splitIndex;i<field.length();i++) {
        result[i + startIndex + 1] = field[i];
    }
}

