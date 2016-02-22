/*
  A few notes to be considered at this moment of development. 
  - The reset function is broken without inputting the actual size.
  - The potentiometer function needs to be re-written with flexibility 
    in mind.
  - The complete data packet(with potentiometer) over UDP has not
    been tested.
  - This does not work completely when data has not been recieved
    from either(i.e anything breaks = everything broken).
  - Oh! Comments are missing too.

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
*/
//for gps
#include <SoftwareSerial.h>

//for udp over ethernet
#include <Wire.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <LSM303.h>

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99 };

IPAddress ipArduino(192, 168, 1, 51);
IPAddress ipComputer(192, 168, 1, 5);
unsigned int localPort = 8888;

SoftwareSerial gpsSerial(4, 5); // RX, TX (TX not used)
const int sentenceSize = 80;
char sentence[sentenceSize];
LSM303 compass; //creates an object of LSM303
// to check if the data has been recieved from the sensors
bool gpsDone = false;
bool magDone = false;
bool potDone = false;
char result[40];  //intially it was 27
int sensorValue = 0;
int sensorPin = A0;

void setup()
{
  //gps
  Serial.begin(9600);
  Ethernet.begin(mac,ipArduino);
  Udp.begin(localPort);
  gpsSerial.begin(9600);

  //magnetometer
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

void loop()
{
  gpsLoop();
  result[22] = ',';
  if(gpsDone)
  {
    magnetometerLoop();
    if(magDone) 
    {
      potentiometerLoop();
      if(potDone)
      {
        //result[strlen(result)] = '\0';
        Serial.println(result);
        Udp.beginPacket(ipComputer, localPort);
        Udp.write(result);
        Udp.endPacket();
      }
    }
  }
  //mark end of result
  resetData();
}

void gpsLoop()
{
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
     sentence[i] = '\0';
     i = 0;
     displayGPS();
    }
  }
}

void magnetometerLoop()
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
  magDone = true;
}

// works but need to write a better code
void potentiometerLoop()
{
  char buf[5];
  sensorValue = analogRead(sensorPin);
  sprintf(buf, "%04i", sensorValue);
  result[strlen(result)] = ',';
  buf[4] = '\0';
  int start = strlen(result);
  for(int i=0; i<strlen(buf) ; i++)
  {
    result[start+i] = buf[i];
  }
  result[strlen(result)] = '\0';
  potDone = true;
}

void resetData()
{
  gpsDone = false;
  magDone = false;
  potDone = false;

  // No idea why doesn't strlen(result) work now.
  for(int i=0 ; i<40 ; i++)
  {
    result[i] = (char)0; 
  }
}

void displayGPS()
{
  char field[11];
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    
    //latitude
    getField(field, 3);  // number
    addPeriod(field, (char*)result, 0, 2);

    //add comma in between latitude and longitude
    result[strlen(field)+1] = ',';
    int firstPartLength = strlen(field)+1;

    //longitude
    getField(field, 5);  // number
    addPeriod(field, (char*)result, firstPartLength + 1, 3);
    gpsDone = true;
  }
  //Serial.println(result);
}

//adds period between degrees and minutes
void addPeriod(String field, char* result, int startIndex,int splitIndex)
{ 
    result[startIndex+splitIndex] = '.';
    
    //put digits before period into the result
    for(int i=0;i<splitIndex;i++)
    {
        result[i + startIndex] = field[i];
    }

    //put digits after period into result
    for(int i=splitIndex;i<field.length();i++)
    {
        result[i + startIndex + 1] = field[i];
    }
}

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 
