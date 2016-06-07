#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>



void cameraSetup()
{
  // write netrual to cameras
  eye_of_sauron.attach(EYE_OF_SAURON); 
}

void moveCameras()
{
  char eye_of_sauron_char = packetBuffer[5];
  int eye_of_sauron_int= map(((int)eye_of_sauron_char), 0, 255, 0, 180);
  eye_of_sauron.write(eye_of_sauron_int);
}
