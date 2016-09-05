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

#include "joydata.h"
#include "gpsdata.h"

// set these so that x and y are according to you
// setup. Swap if needed.
#define X_PIN 1
#define Y_PIN 0

// find these by printing the values read on
// analog pin and printing them while moving the
// joystick to each 4 direction
// Note: these are for when joystick is attached to 3.3v

#define JOY_LEFT_VALUE 660
#define JOY_RIGHT_VALUE 0
#define JOY_UP_VALUE 0
#define JOY_DOWN_VALUE 660

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

JoyData joyData;

// time to wait to ping bot even if no joy changes
#define PING_TIME 3000 
unsigned long lastMillis = 0;

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUD 115200

void setup() 
{
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD);
    DEBUG_SERIAL.println(F("SEND_RCV"));

    joyData.bid = 0;
    joyData.X = 0;
    joyData.Y = 0;

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


void displayBuffer(char* buffer)
{
    DEBUG_SERIAL.print(F("Buffer: "));
    DEBUG_SERIAL.println(buffer);
}

void send(JoyData& data)
{
	DEBUG_SERIAL.print(F("Now sending "));
    printJoyData(data, DEBUG_SERIAL);
    DEBUG_SERIAL.print("...");

	if (radio.write(&data, sizeof(data)))
	{
		DEBUG_SERIAL.println(F("got ack"));
	}
	else
	{
		DEBUG_SERIAL.println(F("no ack"));
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
    printlnGpsData(gpsData, DEBUG_SERIAL);
	return true;
}



void loop()
{
    int ax = analogRead(X_PIN);
    int ay = analogRead(Y_PIN);

    //DEBUG_SERIAL.print(ax);
    //DEBUG_SERIAL.print(" ");
    //DEBUG_SERIAL.println(ay);

    int x = map(ax, JOY_LEFT_VALUE, JOY_RIGHT_VALUE, -255, 255);
    int y = map(ay, JOY_DOWN_VALUE, JOY_UP_VALUE, -255, 255);

    JoyData newData{ 0, x, y };
    //printlnJoyData(newData, DEBUG_SERIAL);

    clampJoyData(newData, -255, 255);
    zeroDeadZoneJoyData(newData, 20);

    unsigned long m = millis();
    unsigned long d = m - lastMillis;
    if (joyData.Y != newData.Y || joyData.X != newData.X || d >= PING_TIME)
    {
        lastMillis = m;
        joyData.X = newData.X;
        joyData.Y = newData.Y;
        send(joyData);

        GpsData gpsData;
        read(gpsData);
    }
}
