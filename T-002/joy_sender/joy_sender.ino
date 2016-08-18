 /*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RF24.h"

#include "joydata.h"
#include "gpsdata.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

void setup() 
{
    Serial.begin(115200);
    Serial.println(F("SENDER"));

    radio.begin();
	radio.setAutoAck(true);                    
	radio.enableAckPayload();
	radio.setPayloadSize(sizeof(GpsData));
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_LOW);
	radio.setDataRate(RF24_2MBPS);

    radio.openWritingPipe(addresses[0]);
	//radio.openReadingPipe(0, addresses[1]);
    radio.stopListening();
	//radio.printDetails();
}


void displayBufferOnLcd(char* buffer, int maxLex)
{
    Serial.print(F("Buffer: "));
    Serial.println(buffer);
}

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

void loop()
{
	int forwardBack = (510 - analogRead(0)) / 2;

	if (forwardBack < -255)
	{
		forwardBack = -255;
	}
	if (forwardBack > 255)
	{
		forwardBack = 255;
	}

	int leftRight = (506 - analogRead(1)) / 2;

	if (leftRight < -255)
	{
		leftRight = -255;
	}
	if (leftRight > 255)
	{
		leftRight = 255;
	}

	JoyData joyData;
	joyData.X = forwardBack;
	joyData.Y = leftRight;
	send(joyData);

	GpsData gpsData;
	read(gpsData);
}
