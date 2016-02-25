/*
* Motor B (right when looking from front)
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>

#include "RF24.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

char buffer[33];

// Motor 1
int dir1PinA = 3;
int dir2PinA = 4;
int speedPinA = 5; // Needs to be a PWM pin to be able to control motor speed

				   // Motor 2
int dir1PinB = 7;
int dir2PinB = 8;
int speedPinB = 6; // Needs to be a PWM pin to be able to control motor speed


void setup()
{
	pinMode(dir1PinA, OUTPUT);
	pinMode(dir2PinA, OUTPUT);
	pinMode(speedPinA, OUTPUT);
	pinMode(dir1PinB, OUTPUT);
	pinMode(dir2PinB, OUTPUT);
	pinMode(speedPinB, OUTPUT);

    buffer[0] = '\0';
    Serial.begin(115200);
    Serial.println(F("Receiver"));
    
    radio.begin();
    radio.setRetries(0, 15);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openReadingPipe(0, addresses[1]);
    radio.startListening();
}

void motorMove(int mindex, int mspeed)
{
	int speedPin = speedPinA;
	int dir1pin = dir1PinA;
	int dir2pin = dir2PinA;

	if (mindex == 1)
	{
		speedPin = speedPinB;
		dir1pin = dir1PinB;
		dir2pin = dir2PinB;
	}

	if (mspeed >= 0)
	{
		// forward
		digitalWrite(dir1pin, LOW);
		digitalWrite(dir2pin, HIGH);

		analogWrite(speedPin, mspeed);
	}
	else
	{
		digitalWrite(dir1pin, HIGH);
		digitalWrite(dir2pin, LOW);

		analogWrite(speedPin, -mspeed);
	}	
}

void goForward(int value, int leftRight)
{
	int motorBSpeed = 0;
	int motorASpeed = 0;

	if (value > 0)
	{
		// motor B forward
		motorBSpeed = value;
		// motor A forward
		motorASpeed = value;

		if (leftRight < 0)
		{
			motorBSpeed = value + leftRight;
		}
		else if (leftRight > 0)
		{
			motorASpeed = value - leftRight;
		}
	}
	else if (value < 0)
	{
		// motor B reverse
		motorBSpeed = value;
		// motor A reverse
		motorASpeed = value;

		if (leftRight < 0)
		{
			motorBSpeed = value - leftRight;
		}
		else if (leftRight > 0)
		{
			motorASpeed = value + leftRight;
		}
	}
	else if (leftRight != 0)
	{
		// condition below is just for clarification
		if (leftRight < 0)
		{
			// motor B backward
			motorBSpeed = leftRight;
			// motor A forward
			motorASpeed = -leftRight;
		}
		else if (leftRight > 0)
		{
			// motor B forward
			motorBSpeed = leftRight;
			// motor A backward
			motorASpeed = -leftRight;
		}
	}
	else
	{
		// stop both motors
	}

	motorMove(0, motorBSpeed);
	motorMove(1, motorASpeed);

	Serial.print("B: ");
	Serial.print(motorBSpeed);
	Serial.print(" A: ");
	Serial.println(motorASpeed);
}

void loop()
{
    if (radio.available())
    {
       
        radio.read(&buffer, 32);
		buffer[33] = '\0';
        // Spew it
        Serial.print(F("Got: "));
        Serial.println(buffer);

		char* p = strchr(buffer, ',');
		if (p != NULL)
		{
			
			char* rest = &buffer[*p + 1];
			*p = '\0';
			int forwardBack = atoi(buffer);
			Serial.print("fb:");
			Serial.println(forwardBack);
			int leftRight = atoi(p + 1);
			Serial.print("lr:");
			Serial.println(leftRight);
			goForward(forwardBack, leftRight);
		}
    }
    else
    {
        //Serial.println(F("NO DATA"));
    }
}
