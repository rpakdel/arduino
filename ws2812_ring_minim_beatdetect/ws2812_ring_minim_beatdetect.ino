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

//#define SERIAL2
#define dataRate 38400
#define debugRate 115200

void setup() 
{
  Serial.begin(dataRate);
  serial2.begin(debugRate);
  serial2.println("----------ready-----------");
  pixels.begin(); // This initializes the NeoPixel library.
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

void serial2print(int i)
{
#if defined SERIAL2
    serial2.print(i);
#endif
}

void serial2println(int i)
{
#if defined SERIAL2
    serial2.println(i);
#endif
}

void loop() 
{
    // assume we're reading first pixel value
    int pixelIndex = 0;
    int offSet = 0;
    while (pixelIndex < NUMPIXELS)
    {
        //serial2print(pixelIndex);
        //serial2print(":");

        int colorBufferIndex = 0;

        // read until COLORSIZE bytes are read
        bool gotNewLine = false;
        while (colorBufferIndex < COLORSIZE)
        {
            if (Serial.available())
            {
                byte b = Serial.read();

                if (b == SEMICOLON)
                {
                    gotNewLine = true;
                    break;
                }

                colorBuffer[colorBufferIndex] = b;
                colorBufferIndex++;
            }
        }

        if (gotNewLine)
        {
            //serial2println("NL");
            break;
        }

        colorBuffer[colorBufferIndex] = '\0';

        if (colorBufferIndex == COLORSIZE)
        {
            //serial2print(colorBuffer);
            //serial2print(" ");

            pixelColors[pixelIndex] = strtol(&colorBuffer[2], NULL, 16);
            pixels.setPixelColor(pixelIndex, pixelColors[pixelIndex]);
            //serial2print("|");
        }

        pixelIndex++;
    }

    // For a set of NeoPixels the first NeoPixel is 0, 
    // second is 1, all the way up to the count of pixels minus one.
    //for (int i = 0; i < NUMPIXELS; i++)
    //{
    //    pixels.setPixelColor(i, pixelColors[i]);
    //}
    pixels.show();
}

