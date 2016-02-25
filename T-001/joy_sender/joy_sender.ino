 /*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RF24.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void setup() 
{
    Serial.begin(115200);
    Serial.println(F("SENDER"));

    radio.begin();
    radio.setRetries(0, 15);
    radio.setPALevel(RF24_PA_HIGH);

    radio.openWritingPipe(addresses[1]);
    radio.stopListening();
}


void displayBufferOnLcd(char* buffer, int maxLex)
{
    Serial.print(F("Buffer: "));
    Serial.println(buffer);
}

char buffer[128];

void loop()
{
  int forwardBack = (510 - analogRead(0)) / 2;
  
  if (forwardBack < -255)
  {
    forwardBack = -255;
  }
  if (forwardBack > 255)
  {
    forwardBack = 255;
  }
  

  int leftRight = (506 - analogRead(1)) / 2;

  if (leftRight < -255)
  {
    leftRight = -255;
  }
  if (leftRight > 255)
  {
    leftRight = 255;
  }
    
    sprintf(buffer, "%d,%d", forwardBack, leftRight);
    Serial.print(F("Now sending "));
    Serial.print(buffer);
    Serial.print(F("..."));
    if (!radio.write(buffer, strlen(buffer)))
    {
        Serial.println(F("failed"));
    }
    else
    {
        Serial.println(F("ok"));
    }
}
