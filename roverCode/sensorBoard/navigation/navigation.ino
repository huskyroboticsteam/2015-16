#include <SoftwareSerial.h> //for gps

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
char result[27];

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
  if(gpsDone) {
    magnetometerLoop();
    result[26] = '\0';
    Serial.println(result);
    Udp.beginPacket(ipComputer, localPort);
    Udp.write(result);
    Udp.endPacket();
  }
  //mark end of result
  gpsDone = false;
  for(int i=0; i<strlen(result); i++) {
    result[i] = (char)0;
  }
}

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
