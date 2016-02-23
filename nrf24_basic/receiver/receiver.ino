/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>

#include "RF24.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

char buffer[33];

void setup()
{
    buffer[0] = '\0';
    Serial.begin(115200);
    Serial.println(F("Receiver"));

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("RECEIVER"));
    display.display();

    radio.begin();

    radio.openReadingPipe(0, addresses[1]);

    radio.startListening();
}

void loop()
{
    
    display.clearDisplay();
    if (radio.available())
    {
        display.setCursor(0, 0);
        display.println(F("RECEIVER*"));

        radio.read(&buffer, 32);

        display.setCursor(0, 16);
        display.print(buffer);
        display.display();
        // Spew it
        Serial.print(F("Got: "));
        Serial.println(buffer);
    }
    else
    {
        display.setCursor(0, 0);
        display.println(F("RECEIVER"));
        display.setCursor(0, 16);
        display.print(buffer);
        //Serial.println(F("NO DATA"));
    }
    display.display();
}
