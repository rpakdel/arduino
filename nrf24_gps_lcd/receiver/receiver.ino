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

RF24 radio(9, 10);
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

void setup()
{
    lcd.begin(16, 2);
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home();
    Serial.begin(115200);
    Serial.println(F("Receiver"));
    lcd.print("Receiver");
    delay(1000);
    radio.begin();

    radio.openReadingPipe(0, addresses[1]);

    // Start the radio listening for data
    radio.startListening();
}

void displayBufferOnLcd(char* buffer, int maxLex)
{
    Serial.print(F("Buffer: "));
    Serial.println(buffer);
    int len = strlen(buffer);
    lcd.setCursor(0, 0);
    char line[16 + 1];
    strncpy(line, buffer, 16);
    line[16] = '\0';
    lcd.print(line);
    Serial.print(F("Line 1: "));
    Serial.println(line);
    if (len > 16 && len < 32)
    {
        strncpy(line, &buffer[16], len - 16);
        line[len - 16] = '\0';
    }
    else
    {
        strncpy(line, &buffer[17], 16);
        line[16] = '\0';
    }
    Serial.print(F("Line 2: "));
    Serial.println(line);
    lcd.setCursor(0, 1);
    lcd.print(line);
}

void loop()
{
    char buffer[32];
    buffer[0] = '\0';    
        
    if (radio.available())
    {
      digitalWrite(13, HIGH);
        radio.read(&buffer, 32);    

        displayBufferOnLcd(buffer, 32);        
        digitalWrite(13, LOW);
    }

    delay(10);
}
