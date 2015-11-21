// All Functions for Wireless Communication //
#include <SPI.h> 
#include "Arduino.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

// GLOBAL VARIABLES //
EthernetUDP Udp;
byte MAC_ADDRESS[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress IP(192, 168, 1, 4);
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
int x = 0;
int y = 0;

void initializeWirelessCommunication()
{
    // Serial.begin(BAUD_RATE);
    Ethernet.begin(MAC_ADDRESS, IP);
    Udp.begin(UDP_PORT);
}

void getPacket()
{
    int packetSize = Udp.parsePacket();
    //Serial.println(x);
    //Serial.println(y);
    if(packetSize)
    {
        /*int actual = 0;
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = Udp.remoteIP();
        for (int i =0; i < 4; i++)
        {
          actual = (remote[i], HEX);
          Serial.print(actual);
          if (i < 3)
          {
              Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(Udp.remotePort());*/
    
        // read the packet into packetBufffer
        Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
        //Serial.println("Contents:");
        //Serial.println(packetBuffer);
    }
}

void parsePacketData()
{
    boolean isFirstHalf = true;
    for (int i = 0; i < UDP_TX_PACKET_MAX_SIZE; i++)
    {
        if (packetBuffer[i] != '\0')
        {
            char current = packetBuffer[i];
            if (current == ',')
            {
                isFirstHalf = false;
            }
            else if (isFirstHalf)
            {
                x = x*10 + (packetBuffer[i] - 48);
            }
            else
            {
                y = y*10 + (packetBuffer[i] - 48);
            }
        }
    }
}

