#include <Wire.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h> 

#include <ArduinoJson.h>

#include "gpsdata.h"
#include "joydata.h"

SoftwareSerial SoftSerial(2, 3); // RX, TX
#define DEBUG_SERIAL SoftSerial
#define DEBUG_BAUD 115200

#define ESP_SERIAL Serial
#define ESP_BAUD 115200


RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void send(JoyData& data)
{
    //DEBUG_SERIAL.print(F("Now sending "));
    //DEBUG_SERIAL.print(data.X);
    //DEBUG_SERIAL.print(F(", "));
    //DEBUG_SERIAL.print(data.Y);
    //DEBUG_SERIAL.print(F("..."));

    if (radio.write(&data, sizeof(data)))
    {
        //DEBUG_SERIAL.println(F("ok"));
    }
    else
    {
        //DEBUG_SERIAL.println(F("no ack"));
    }
}

bool read(GpsData& gpsData)
{
    if (!radio.isAckPayloadAvailable())
    {
        DEBUG_SERIAL.println(F("No GPS data"));
        return false;
    }

    radio.read(&gpsData, sizeof(gpsData));
    DEBUG_SERIAL.print(F("GPS: "));
    DEBUG_SERIAL.print(gpsData.lat);
    DEBUG_SERIAL.print(F(", "));
    DEBUG_SERIAL.println(gpsData.lon);
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
    DEBUG_SERIAL.begin(DEBUG_BAUD);
    DEBUG_SERIAL.println(F("Begin setup"));

    ESP_SERIAL.begin(ESP_BAUD);

    setupRadio();

    DEBUG_SERIAL.println(F("Ready"));
}

int readCount = 0;
#define ESP_BUFFER_SIZE 255
char serialBuffer[ESP_BUFFER_SIZE];
void loop(void)
{
    while (ESP_SERIAL.available())
    {
        int c = ESP_SERIAL.read();
        if (c == '\n' || readCount == (ESP_BUFFER_SIZE - 1))
        {
            serialBuffer[readCount] = 0;
            //DEBUG_SERIAL.println(serialBuffer);
            readCount = 0;

            JoyData joyData;
            if (jsonDeserializeJoyData(&serialBuffer[1], joyData))
            {
                //printJoyData(joyData, DEBUG_SERIAL);
                DEBUG_SERIAL.println();
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

