void soil() {
  float rawValue = analogRead(SOIL_SENSOR);
  Serial.println(rawValue);
  float humidity = (rawValue * 100/ maxW);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  //delay(1000);
}
