/*#include <SPI.h>
#include "Adafruit_MAX31855.h"

int therm_DO = 5;
int therm_CS = 4;
int therm_CLK = 3;

Adafruit_MAX31855 thermocouple(therm_CLK, therm_CS, therm_DO);
void setup() {  
    Serial.begin(9600);
}*/
void temp() {
    F = thermocouple.readFarenheit();
    C = thermocouple.readCelsius();
    /*Serial.print("C: ");
    Serial.println(C);
    Serial.print("F: ");
    Serial.println(F);
    */
    //delay(1000);
}

void soil() {
  float rawValue;
  if ((int)packetBuffer[2] == 1) {
    rawValue = analogRead(SOIL_SENSOR1);
  } else if ((int)packetBuffer[2] == 2) {
    rawValue = analogRead(SOIL_SENSOR2);
  } else {
    humidity = 0;
  }
  humidity = ((maxW - rawValue) * 100/ (maxW - minW));
  /*
  Serial.print("Humidity: ");
  Serial.println(humidity);
  */
  //delay(1000);
}
