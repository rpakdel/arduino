void setup() 
{
    pinMode(5, INPUT);
    Serial.begin(9600); 
}

void loop() 
{
    int a = analogRead(0);
    int d = digitalRead(5);

    Serial.print("a: ");
    Serial.print(a);
    Serial.print(" d: ");
    Serial.println(d);
}
