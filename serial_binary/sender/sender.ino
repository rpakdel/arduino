#include <SoftwareSerial.h>

#define SENDER

#if defined SENDER
#define TX 3
#define RX 4
#define NAME "SENDER"
#define COM "COM11"
#else
#define TX 4
#define RX 3
#define COM "COM10"
#define NAME "RECEIVER"
#endif

SoftwareSerial mySerial(RX, TX); // RX, TX

#define SERIAL_DEBUG_RATE 115200
#define SERIAL_RATE 115200

void setup()
{
    Serial.begin(SERIAL_DEBUG_RATE);

    Serial.print(NAME);
    Serial.print(" ");
    Serial.println(COM);
    mySerial.begin(SERIAL_RATE);
}
void loop() // run over and over
{
  while(Serial.available())
  {    
    byte b = Serial.read();    
    mySerial.write(b);
  }        
}

