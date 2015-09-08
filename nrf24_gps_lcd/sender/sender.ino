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
#include <SoftwareSerial.h>
#include <TinyGPSPlus/TinyGPS++.h>
#include "RF24.h"

#define LCD_I2C_ADDR    0x27
#define LCD_BACKLIGHT_PIN     3
#define LCD_En_pin  2
#define LCD_Rw_pin  1
#define LCD_Rs_pin  0
#define LCD_D4_pin  4
#define LCD_D5_pin  5
#define LCD_D6_pin  6
#define LCD_D7_pin  7

LiquidCrystal_I2C	lcd(LCD_I2C_ADDR, LCD_En_pin, LCD_Rw_pin, LCD_Rs_pin, LCD_D4_pin, LCD_D5_pin, LCD_D6_pin, LCD_D7_pin);

#define GPS_RX 3
#define GPS_TX 2
#define GPS_BAUD 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus tinyGPS;

RF24 radio(7, 8);
byte addresses[][6] = { "1Node", "2Node" };

void setup() 
{
    gpsSerial.begin(GPS_BAUD);

    lcd.begin(16, 2);
    lcd.setBacklightPin(LCD_BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home();
    lcd.clear();
    lcd.println(F("SENDER"));

    Serial.begin(115200);
    Serial.println(F("SENDER"));

    radio.begin();

    radio.openWritingPipe(addresses[1]);
    radio.stopListening();
}

double DegToRad(double deg)
{
    return deg * 0.01745278;
}


void printXYZ(TinyGPSPlus &tiny)
{
    int earthRadius = 6367; // km
    double lat = DegToRad(tiny.location.lat());
    double lon = DegToRad(tiny.location.lng());
    double x = earthRadius * cos(lat) * cos(lon);
    double y = earthRadius * cos(lat) * sin(lon);
    double z = earthRadius * sin(lat);
    Serial.print(F(" XYZ("));
    Serial.print(x, 2);
    Serial.print(F(", "));
    Serial.print(y, 2);
    Serial.print(F(", "));
    Serial.print(z, 2);
    Serial.print(F(")"));
}

bool GetLatLngAlt(TinyGPSPlus &tiny, char* buffer, int len)
{
    if (!tiny.location.isValid())
    {
        return false;
    }

    char lat[32];
    dtostrf(tiny.location.lat(), 9, 6, lat);
    char lng[32];
    dtostrf(tiny.location.lng(), 10, 6, lng);
    char alt[32];
    dtostrf(tiny.altitude.meters(), 6, 2, alt);
    snprintf(
        buffer,
        len,
        "%s,%s,%s",
        lat, lng, alt);
    return true;
}

void displayInfo(TinyGPSPlus &tiny, char* buffer, int bufferLen)
{
    if (GetLatLngAlt(tiny, buffer, bufferLen))
    {
        Serial.println(buffer);
    }
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
    char buffer[128];
    buffer[0] = '\0';
    while (gpsSerial.available() > 0)
    {
        if (tinyGPS.encode(gpsSerial.read()))
        {
            if (GetLatLngAlt(tinyGPS, buffer, 128))
            {
                lcd.clear();
                lcd.home();
                displayBufferOnLcd(buffer, 128);

                if (!radio.write(buffer, strlen(buffer)))
                {
                    lcd.print(F("fail"));
                }
                else
                {
                    lcd.print(F(" ok"));
                }
            }
        }
    }
    
    
    delay(250);
}
