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

#define X_PIN 0
#define Y_PIN 1

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

JoyData joyData;
unsigned long lastMillis;

// time to wait to ping bot even if no joy changes
#define PING_TIME 3000 

void setup() 
{
    Serial.begin(115200);
    Serial.println(F("SENDER"));

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
	//radio.openReadingPipe(0, addresses[1]);
    radio.stopListening();
	//radio.printDetails();
  lastMillis = millis();
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
  // the joystick is rotated 90, so x and y are swapped
  JoyData newData;
	newData.X = (510 - analogRead(Y_PIN)) / 2;
  newData.Y = (506 - analogRead(X_PIN)) / 2;

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
