/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include <printf.h>

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

// Neopixel is connected to
#define NEOPIXEL_PIN 6

// How many NeoPixels
#define NUMPIXELS 16

#define DEBUG_DATA_RATE 115200

SoftwareSerial serial2 = SoftwareSerial(5, 4);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

RF24 radio = RF24(9, 10);
static const int receiverAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };

void setup()
{
    serial2.begin(DEBUG_DATA_RATE);
    
    pixels.begin(); // This initializes the NeoPixel library.
    clearPixels();

    radio.begin();
	radio.setDataRate(RF24_2MBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openReadingPipe(1, receiverAdd[0]);    
    radio.startListening();

    serial2.println(F("----------Receiver ready----------"));
}

void clearPixels()
{
  for(char i = 0; i < 16; i++)
  {
    printColor(i, 0);
  }
}

void printColor(int index, long color)
{
    if (index >=0 && index < 16)
    {
      //serial2.println(index, DEC);
      //serial2.print("|");
      //serial2.println(pixelColor, HEX);      
      pixels.setPixelColor(index, color);

      if (index == 15)
      {
        pixels.show();
      }
    }    
}

void printColorFromBuffer(int* buffer, int len)
{
  int pixelIndex = buffer[1];
  long pixelColor = 0;
  
  for(int byteIndex = 2; byteIndex < len; byteIndex++)
  {
    long value = buffer[byteIndex];
    byte shift = (4 - byteIndex + 1) * 8;
    value = (value << shift);
    pixelColor += value;
  }

  //serial2.println(pixelIndex);
  //serial2.print(" ");
  //serial2.println(pixelColor, HEX);
  printColor(pixelIndex, pixelColor);
}

void displayBuffer(int* buffer, int len)
{    
    serial2.print(F("Buffer: "));    
	
	for (int i = 0; i < len; i++)
	{
		int b = buffer[i];
		if (b == '#')
		{
		  serial2.print(F("#"));
		}
		else
		{
			  serial2.print(buffer[i], HEX);
		}
	}
    serial2.println();
}

int buffer[32];   

void clearBuffer(int len)
{
  for (int i = 0; i < len; i++)
  {
    buffer[i] = 0;
  }
}

void loop()
{
    //clearBuffer(6);
        
    while (radio.available())
    {
        radio.read(&buffer, 32);

        //displayBuffer(buffer, 6);    
        printColorFromBuffer(buffer,  6);   
    }    
}
