int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor
int d0 = 11;
 
void setup () 
{
  pinMode (ledPin, OUTPUT);
  pinMode (d0, INPUT);
  Serial.begin (9600);
}
 
void loop () 
{
  sensorValue = analogRead (sensorPin);
  //digitalWrite (ledPin, HIGH);
  //delay (sensorValue);
  //digitalWrite (ledPin, LOW);
  //delay (sensorValue);
  int d0Value = digitalRead(d0);
  if (d0Value)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  Serial.println (sensorValue, DEC);
}
