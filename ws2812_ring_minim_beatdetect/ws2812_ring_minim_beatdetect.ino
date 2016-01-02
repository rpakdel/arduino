// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

SoftwareSerial serial2(5, 4);

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

long pixelColors[NUMPIXELS]; // 16 pixels

#define NEWLINE 10
#define CARRIAGERETURN 13
#define COMMA 44
#define COLORSIZE 8
#define SEMICOLON 59

char colorBuffer[COLORSIZE + 1];

#define SERIAL2
#define dataRate 115200
#define debugRate 115200

void setup() 
{
  Serial.begin(dataRate);
  serial2.begin(debugRate);
  serial2.println("----------ready-----------");
  pixels.begin(); // This initializes the NeoPixel library.
  clearPixels();
}

void clearPixels()
{
  for(char i = 0; i < 16; i++)
  {
    printColor(i, 0);
  }
}
  

void serial2print(const char* msg)
{
#if defined SERIAL2
  serial2.print(msg);
#endif
}

void serial2println(const char* msg)
{
#if defined SERIAL2
  serial2.println(msg);
#endif
}

void serial2print(long i)
{
#if defined SERIAL2
    serial2.print(i);
#endif
}

void serial2println(long i)
{
#if defined SERIAL2
    serial2.println(i);
#endif
}

void printColor(char index, long color)
{
    if (index >=0 && index < 16)
    {
      //serial2.println(index, DEC);
      //serial2.print("|");
      //serial2.println(pixelColor, HEX);
      pixelColors[index] = color;
      pixels.setPixelColor(index, color);

      if (index == 15)
      {
        pixels.show();
      }
    }
    //delay(1);
}

byte byteIndex = 0;
char pixelIndex = -1;
long pixelColor = 0;

void nextByte(byte b)
{
  if (b == '#')
  {
     //Serial.print(byteIndex);
     //Serial.println("|#");
     // read a whole frame, set the color and reset
     if (byteIndex == 6)
     {
        printColor(pixelIndex, pixelColor);
     }
     
     // a new frame is starting
     byteIndex = 0;
     pixelIndex = -1;
     pixelColor = 0;
   }
   else
   {
     // already read #, so this is the pixelIndex
     if (byteIndex == 1)
     {
       //Serial.print(byteIndex);
       //Serial.print("|");
       pixelIndex = b;
       //Serial.println(b);
     }
     else
     {
       //Serial.print(byteIndex);
       //Serial.print('|');
       //Serial.print(b, BIN);
       //Serial.print(":");

       if (byteIndex >= 2) // lsb of color
       {
         long value = b;
         byte shift = (4 - byteIndex + 1) * 8;
         value = (value << shift);
         pixelColor += value;
         //Serial.print(" shift ");
         //Serial.print(shift);
         //Serial.print(' ');                    
         //Serial.println(value, BIN);
       }
     }
   }
   byteIndex++;
}

void loop() 
{
    while (Serial.available())
    {
        byte b = Serial.read();
        nextByte(b);
    }
}
