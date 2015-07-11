/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <LCD.h>
#include <I2CIO.h>
#include <FastIO.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8);
byte addresses[][6] = { "1Node", "2Node" };

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C	lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

char buffer[32];

void setup()
{
    lcd.begin(16, 2);
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home();
    Serial.begin(115200);
    Serial.println(F("Receiver"));

    radio.begin();

    radio.openReadingPipe(0, addresses[1]);

    // Start the radio listening for data
    radio.startListening();
}

void loop()
{
    buffer[0] = '\0';
    lcd.clear();
    lcd.home(); 
    if (radio.available())
    {
        radio.read(&buffer, 32);    

        // Spew it
        Serial.print(F("Got: "));
        Serial.println(buffer);
        lcd.print(buffer);
    }
    else
    {
        Serial.println(F("NO DATA"));
        lcd.print(F("NO DATA"));
    }

    // Try again 1s later
    delay(1000);
}