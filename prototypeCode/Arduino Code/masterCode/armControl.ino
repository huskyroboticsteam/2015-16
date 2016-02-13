// Apr 18, 2015. Husky Robotics, made by Electrical Team
// Using Analog read from current sensor ACS758

// Attach the arm motors
void attachArmMotors()
{
    /*for (int i = 0; i < 3; i++) {
        armMotor[i].attach(TALON_ARM[i]);
    }*/
    armMotor[0].attach(TALON_ARM[0]);
    armMotor[1].attach(TALON_ARM[1]);
    armMotor[2].attach(TALON_ARM[2]);
    
}

// writes position from wireless to the arm motors
void writeToArmMotors()
{
    for(int i = 0; i < 3; i++) {
        armMotor[i].writeMicroseconds(dirToMicroseconds(arm[i]));
    }
}

// DEMO
void armRiseAndBack() {
      for(int i = 0; i < 3; i++) {
        armMotor[i].writeMicroseconds(SPEEDUP_FREQUENCY);
        //Serial.println(TALON_ARM[i]);
        delay(5000);
        armMotor[i].writeMicroseconds(SPEEDDOWN_FREQUENCY);
        delay(5000);
        armMotor[i].writeMicroseconds(NEUTRAL_FREQUENCY);
      }
}
