 /*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>


#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

#define GPS_RX 4
#define GPS_TX 3
#define GPS_BAUD 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus tinyGPS;


RF24 radio(9, 10);
static const int senderAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };
static const int sender = 1;


bool oledAvailable = true;
bool gpsAvailable = true;

void initDisplay()
{
    if (!oledAvailable)
    {
        return;
    }

    Serial.println(F("Init OLED"));
    Wire.begin();
    display.begin(&Adafruit128x64, I2C_ADDRESS);
    display.set400kHz();
    display.setFont(Adafruit5x7);
    display.set2X();
    display.clear();    
    display.setCursor(0, 0);
    display.print(F("Sender "));
    display.print(sender);
    Serial.println(F("OLED ready"));
}

void initGps()
{
    if (!gpsAvailable)
    {
        return;
    }

    gpsSerial.begin(GPS_BAUD);
}

void setup() 
{
    Serial.begin(9600);
    Serial.println(F("Setup"));
    
    
    initDisplay();
    initGps();    
    
    delay(1000);
    Serial.print(F("Sender "));
    Serial.println(sender);

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(senderAdd[sender]);
    radio.stopListening();
}



void displayBuffer(char* buffer)
{
    if (oledAvailable)
    {
        int len = strlen(buffer);
        display.setCursor(0, 0);

        display.print(F("Sender "));
        display.println(sender);

        for (int i = 0; i < len; ++i)
        {
            if (buffer[i] == ',')
            {
                display.println();
                display.clearToEOL();
            }
            else
            {
                display.print(buffer[i]);
            }
            
        }
    }
    
    Serial.print(F("Buffer: "));
    Serial.println(buffer);
}

void displaySendStatus(bool success)
{
    return;
    if (!oledAvailable)
    {
        return;
    }
    
    int x = 128 - 2 * 6;
    int y = 0;
    // clear the char
    display.clear(x, x + 2 * 6, y, y + 2 * 8);
    
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
    if (!gpsAvailable || !tiny.location.isValid())
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

    while (gpsAvailable && gpsSerial.available() > 0)
    {
        int g = gpsSerial.read();

        if (tinyGPS.encode(g))
        {
            if (GetLatLngAlt(tinyGPS, buffer, 128))
            {
                displayBuffer(buffer);

                bool sendResult = radio.write(buffer, strlen(buffer));
                displaySendStatus(sendResult);
            }
        }
    }    

    delay(250);
}
