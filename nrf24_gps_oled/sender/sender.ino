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

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

#define GPS_RX 5
#define GPS_TX 6
#define GPS_BAUD 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus tinyGPS;


RF24 radio(9, 10);
static const byte sender[6] = "2Node";

void setup() 
{
    gpsSerial.begin(GPS_BAUD);
    display.begin(&Adafruit128x64, I2C_ADDRESS);
    display.set400kHz();
    display.setFont(Adafruit5x7);
    delay(1000);

    display.clear();
    //display.setTextColor(WHITE);
    //display.setTextSize(1);
    //display.setCursor(0, 0);
    display.println(F("Sender"));

    Serial.begin(115200);
    Serial.println(F("SENDER"));

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(sender);
    radio.stopListening();
}


void displayBuffer(char* buffer, int maxLex)
{    
    int y = 2 * 8;
    int len = strlen(buffer);
    Serial.println(len);
    display.clear(0, len * 6, y, y + 8);
    //display.setCursor(0, y);
    display.print(buffer);
    //
    Serial.print(F("Buffer: "));
    Serial.println(buffer);
}

void displaySendStatus(bool success)
{
    int x = 128 - 1 * 6;
    int y = 0;
    // clear the char
    display.clear(x, x + 6, y, y + 8);
    //display.setCursor(x, y);
    if (success)
    {
        display.print(F("^"));
    }
    else
    {
        display.print(F("x"));
    }
}

void displayInfo(TinyGPSPlus &tiny, char* buffer, int bufferLen)
{
    if (GetLatLngAlt(tiny, buffer, bufferLen))
    {
        Serial.println(buffer);
    }
}

bool GetLatLngAlt(TinyGPSPlus &tiny, char* buffer, int len)
{

    if (!tiny.location.isValid())
    {
        return false;
    }

    Serial.println(F("Got location"));
    char lat[16];
    dtostrf(tiny.location.lat(), 9, 6, lat);
    char lng[16];
    dtostrf(tiny.location.lng(), 10, 6, lng);
    char alt[16];
    dtostrf(tiny.altitude.meters(), 6, 2, alt);
    snprintf(
        buffer,
        len,
        "%s,%s,%s",
        lat, lng, alt);
    return true;
}

void loop()
{
    char buffer[32];
    buffer[0] = '\0';
    while (gpsSerial.available() > 0)
    {
        if (tinyGPS.encode(gpsSerial.read()))
        {
            if (GetLatLngAlt(tinyGPS, buffer, 128))
            {
                displayBuffer(buffer, 128);

                bool sendResult = radio.write(buffer, strlen(buffer));
                displaySendStatus(sendResult);
            }
        }
    }

    delay(250);
}
