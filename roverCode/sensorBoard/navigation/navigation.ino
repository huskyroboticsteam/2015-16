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
#include <SoftwareSerial.h> //for gps
#define ANALOG_PIN A0

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
LSM303 compass;
bool gpsDone = false;
bool magDone = false;
bool potDone = false;
char result[32];

void setup()
{
  //gps
  Serial.begin(9600);
  Ethernet.begin(mac,ipArduino);
  Udp.begin(localPort);
  gpsSerial.begin(9600);

  //potentiometer
  pinMode(ANALOG_PIN, INPUT);
  
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
        //mark end of result
        result[strlen(result)] = '\0';
        
        Serial.println(result);
        Udp.beginPacket(ipComputer, localPort);
        Udp.write(result);
        Udp.endPacket();
      }
    }
  }
  resetData();
}

void resetData()
{
  gpsDone = false;
  magDone = false;
  potDone = false;

  // No idea why doesn't strlen(result) work now.
  for(int i=0 ; i<strlen(result) ; i++)
  {
    result[i] = (char)0; 
  }
}

//reads data from gps and puts it into buffer
void gpsLoop() {
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

//reads magnetometer data and puts it in buffer
void magnetometerLoop() {
  
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

//reads potentiometer data and puts it in buffer
void potentiometerLoop() {
    int potPos = analogRead(ANALOG_PIN);

    //get digits of potentiometer reading separately
    int thousand = (int)potPos/1000;
    int hundred = (int)potPos%1000/100;
    int ten = (int)potPos%100/10;
    int one = (int)potPos%10;
    
    //put potentiometer reading in buffer
    result[26] = ',';
    result[27] = (char)thousand+48;
    result[28] = (char)hundred+48;
    result[29] = (char)ten+48;
    result[30] = (char)one+48;
    potDone = true;
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
void addPeriod(String field, char* result, int startIndex,int splitIndex) { 
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
