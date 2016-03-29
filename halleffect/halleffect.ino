int inputPin = 12;

void setup() 
{
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(inputPin, HIGH); // pull resistor
}

int pinState = LOW;
void loop() 
{
  int v = digitalRead(inputPin);
  if (pinState != v)
  {
    Serial.println(v);
    pinState = v;
    digitalWrite(13, !v);
  } 
}
