#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define OH_SHIT 5
#define EYE_OF_SAURON 6


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x43);

void cameraSetup()
{
   //Serial.begin(9600);
   pwm.begin();
   pwm.setPWMFreq(60);
   yield();
   
}

void moveCameras()
{
  char oh_shit_char = packetBuffer[OH_SHIT - 1];
  char eye_of_sauron_char = packetBuffer[EYE_OF_SAURON - 1];
  int oh_shit = map(((int)oh_shit_char), 0, 255, 0, 800);
  int eye_of_sauron = map(((int)eye_of_sauron_char), 0, 255, 0, 800);
  //Serial.print("oh_shit: "); Serial.println(oh_shit);
  //Serial.print("eye_of_sauron: "); Serial.println(eye_of_sauron);
  pwm.setPWM(OH_SHIT, 0, oh_shit);
  pwm.setPWM(EYE_OF_SAURON, 0, eye_of_sauron);
}

