#include <Wire.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include "../../T-002/shared/joydata.h"
#include "../../T-002/shared/gpsdata.h"

#define X_PIN 0
#define Y_PIN 1
#define BUTTON_PIN 2

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

JoyData lastJoyData = { 0, 0, 0, 0 };
unsigned long lastMillis = 0;

// time to wait to ping bot even if no joy changes
#define PING_TIME 3000 

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Joystick"));

    radio.begin();
    radio.setAutoAck(true);
    radio.enableAckPayload();
    radio.setPayloadSize(sizeof(GpsData));
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_2MBPS);

    radio.openWritingPipe(addresses[0]);
    radio.stopListening();
}

void send(JoyData& data)
{
    Serial.print(F("Joy: "));
    printJoyData(data, Serial);
    Serial.print(F("..."));
    
    if (radio.write(&data, joyDataSize))
    {
        Serial.println(F("ok"));
    }
    else
    {
        Serial.println(F("no ack"));
    }
}

bool read(GpsData& gpsData)
{
    if (!radio.isAckPayloadAvailable())
    {
        Serial.println(F("No GPS data"));
        return false;
    }

    radio.read(&gpsData, gpsDataSize);
    Serial.print(F("GPS: "));
    printlnGpsData(gpsData, Serial);
    return true;
}

byte sameJoyDataCount = 0;

void loop()
{
    // the joystick is rotated 90, so x and y are swapped
    short x = (510 - analogRead(Y_PIN)) / 2;
    short y = (506 - analogRead(X_PIN)) / 2;
    byte button = 255 - analogRead(BUTTON_PIN);

    JoyData newData = { 0, x, y, button };

    clampJoyDataXY(newData, -255, 255);
    zeroDeadZoneJoyData(newData, 20, 10);

    unsigned long m = millis();
    unsigned long d = m - lastMillis;
    bool sameJoyData = areJoyDataEqual(lastJoyData, newData);
    if (!sameJoyData)
    {
        sameJoyDataCount = 0;
    }
    else if (sameJoyDataCount < 3)
    {
        sameJoyDataCount++;
        sameJoyData = false;
    }

    //if (!sameJoyData || d >= PING_TIME)
    //{
        //Serial.println(F("Sending Joystick data"));
        lastMillis = m;
        lastJoyData = newData;
        send(newData);

        GpsData gpsData;
        read(gpsData);
    //}
}
