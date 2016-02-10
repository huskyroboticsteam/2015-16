#include <SoftwareSerial.h> //for gps

//for udp over ethernet
//#include <Wire.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x89, 0x99 };

IPAddress ipArduino(192, 168, 1, 51);
IPAddress ipComputer(192, 168, 1, 5);
unsigned int localPort = 8888;

SoftwareSerial gpsSerial(4, 5); // RX, TX (TX not used)
const int sentenceSize = 80;
char sentence[sentenceSize];

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac,ipArduino);
  Udp.begin(localPort);
  //Wire.begin();
  gpsSerial.begin(9600);
}

void loop()
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

void displayGPS()
{
  char field[11];
  char result[23];
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

    //mark end of result
    result[22] = '\0';
    Serial.println(result);
    Udp.beginPacket(ipComputer, localPort);
    Udp.write(result);
    Udp.endPacket();
    
  }
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
