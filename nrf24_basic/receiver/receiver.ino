/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <LCD.h>
#include <I2CIO.h>
#include <FastIO.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "RF24.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Receiver"));

    radio.begin();

    radio.openReadingPipe(0, addresses[1]);

    radio.startListening();
}

char buffer[33];

void loop()
{
    buffer[0] = '\0';
    if (radio.available())
    {
        radio.read(&buffer, 32);

        // Spew it
        Serial.print(F("Got: "));
        Serial.println(buffer);
    }
    else
    {
        //Serial.println(F("NO DATA"));
    }

    //Serial.println(F("Waiting"));
    // Try again 1s later
    delay(10);
}