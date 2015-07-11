#include <ESP8266.h>
#include <doxygen.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "Esp8266Wifi.h"
#include "Gps.h"

#define LED 13



#define DEBUG_BAUD 9600

Gps gps;
Esp8266Wifi wifi(gps);

// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(DEBUG_BAUD);
    if (!wifi.Setup())
    {
        Serial.println("Could not setup wireless TCP server");
    }

    gps.Setup();
}

void loop()
{
    wifi.Loop();
    gps.Loop();
}
