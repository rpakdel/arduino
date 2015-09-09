 /*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RF24.h"
#include <U8glib.h>
#include <TinyGPS++.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

RF24 radio(9, 10);
static const int senderAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };
static const int sender = 0;

void setup() 
{
    pinMode(13, OUTPUT);
    Serial.begin(115200);
    Serial.print(F("SENDER"));
    Serial.println(sender + 1);

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(senderAdd[sender]);
    radio.stopListening();
}


void displayBuffer(char* buffer)
{    
    int y = 2 * 8;
    int len = strlen(buffer);
    Serial.print(sender + 1);
    Serial.print(F(" Buffer: "));
    Serial.println(buffer);
}

int count = 0;
void prepareBuffer(char* buffer, int maxLen)
{
    count++;
    if (count > 1000)
    {
        count = 0;
    }
    sprintf(buffer, "%d", count);
}

void loop()
{
    char buffer[32];
    buffer[0] = '\0';
    prepareBuffer(buffer, 32);
    displayBuffer(buffer);

    digitalWrite(13, HIGH);
    bool sendResult = radio.write(buffer, strlen(buffer));
    digitalWrite(13, LOW);
    if (!sendResult)
    {
        Serial.println("X");
    }

    delay(150);
}
