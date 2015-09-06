//Allan Gilmore
//code for 3 ultrasonic sensors on arm
//range 2cm - 450cm


const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 6;
const int echoPin2 = 8;
const int trigPin3 = 10;
const int echoPin3 = 12;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration1, inches1, cm1;
  long duration2, inches2, cm2;
  long duration3, inches3, cm3;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin1, OUTPUT);
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);
  
  inches1 = microsecondsToInches(duration1);
  cm1 = microsecondsToCentimeters(duration1);
  
  delay(100);
  
  pinMode(trigPin2, OUTPUT);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);

  // convert the time into a distance
  inches2 = microsecondsToInches(duration2);
  cm2 = microsecondsToCentimeters(duration2);
  
  delay(100);
  
  
  pinMode(trigPin3, OUTPUT);
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  
  pinMode(echoPin3, INPUT);
  duration3 = pulseIn(echoPin3, HIGH);
  
  inches3 = microsecondsToInches(duration3);
  cm3 = microsecondsToCentimeters(duration3);
  
  if (cm2 <= cm1 && cm2 <= cm3) {
    // cm2 is the smallest
    Serial.print("Mid ");
    Serial.print(cm2);
    Serial.print("cm, ");
    Serial.println();
  } else if (cm1 < cm3) {
    // cm1 is the smallest
    Serial.print("left ");
    Serial.print(cm1);
    Serial.print("cm, ");
    Serial.println();
  } else {
    // cm3 is the smallest 
    Serial.print("Right ");
    Serial.print(cm3);
    Serial.print("cm, ");
    Serial.println();
  }  
  delay(1000);
}
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
