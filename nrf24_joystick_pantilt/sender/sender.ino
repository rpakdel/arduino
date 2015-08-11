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
    pinMode(3, INPUT);
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

int count = 0;
char buffer[128];

void loop()
{
    int x = analogRead(0);
    int y = analogRead(1);
    int s = digitalRead(3);

    sprintf(buffer, "%d,%d,%d", x, y,s);
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
    delay(100);
}
