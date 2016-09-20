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
    radio.setChannel(108);
    radio.openWritingPipe(addresses[0]);
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
    count++;
    if (count >= 1024)
    {
        count = 0;
    }

    sprintf(buffer, "%d", count);
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
    delay(500);
}
