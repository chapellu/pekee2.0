void setup()
{
  Serial.begin(9600);
}
void loop()
{
  while (1)  {
    String message = Serial.readString();  // on soustrait le caractère 0, qui vaut 48 en ASCII
    Serial.println(message);
    delay(1000);
  }
  delay(10);
}
