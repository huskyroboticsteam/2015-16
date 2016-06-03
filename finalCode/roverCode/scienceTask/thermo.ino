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
    float F = thermocouple.readFarenheit();
    float C = thermocouple.readCelsius();
    Serial.print("C: ");
    Serial.println(C);
    Serial.print("F: ");
    Serial.println(F);
    delay(1000);
}

