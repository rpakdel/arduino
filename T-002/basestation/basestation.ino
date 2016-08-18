#include <Wire.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h> 

#include <ArduinoJson.h>

#include "gpsdata.h"
#include "joydata.h"

SoftwareSerial EspSerial(2, 3); // RX, TX
#define ESP_BAUD 115200
#define DEBUG_BAUD 115200

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void send(JoyData& data)
{
    Serial.print(F("Now sending "));
    Serial.print(data.X);
    Serial.print(F(", "));
    Serial.print(data.Y);
    Serial.print(F("..."));

    if (radio.write(&data, sizeof(data)))
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

    radio.read(&gpsData, sizeof(gpsData));
    Serial.print(F("GPS: "));
    Serial.print(gpsData.lat);
    Serial.print(F(", "));
    Serial.println(gpsData.lon);
    return true;
}

void setupRadio()
{
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


void setup(void)
{
    Serial.begin(DEBUG_BAUD);
    Serial.println(F("Begin setup"));

    EspSerial.begin(ESP_BAUD);

    setupRadio();

    Serial.println(F("Ready"));
}

int readCount = 0;
#define ESP_BUFFER_SIZE 255
char serialBuffer[ESP_BUFFER_SIZE];
void loop(void)
{
    while (EspSerial.available())
    {
        int c = EspSerial.read();
        if (c == '\n' || readCount == (ESP_BUFFER_SIZE - 1))
        {
            serialBuffer[readCount] = 0;
            //Serial.println(serialBuffer);
            readCount = 0;

            JoyData joyData;
            if (jsonDeserializeJoyData(&serialBuffer[1], joyData))
            {
                send(joyData);
            }
        }
        else
        {
            serialBuffer[readCount] = c;
            readCount++;
        }
    }
}

