#include <SoftwareSerial.h>

#define RECEIVER

#if defined SENDER
#define TX 3
#define RX 4
#define NAME "SENDER"
#define COM "COM11"
#else
#define TX 4
#define RX 3
#define COM "COM10"
#define NAME "RECEIVER"
#endif

SoftwareSerial mySerial(RX, TX); // RX, TX

#define SERIAL_DEBUG_RATE 115200
#define SERIAL_RATE 115200

void setup()
{
    Serial.begin(SERIAL_DEBUG_RATE);

    Serial.print(NAME);
    Serial.print(" ");
    Serial.println(COM);
    mySerial.begin(SERIAL_RATE);
}

void printColor(char pixelIndex, long pixelColor)
{
    
    Serial.print("=> ");
    Serial.print(pixelIndex, DEC);
    Serial.print(':');
    Serial.println(pixelColor);
    Serial.println();
}

byte byteCount = 0;
char pixelIndex = -1;
long pixelColor = 0;

void loop() // run over and over
{   
    while (mySerial.available())
    {
        byte b = mySerial.read();
        if (b == '#')
        {
            // read a whole frame, set the color and reset
            if (byteCount == 10)
            {
                printColor(pixelIndex, pixelColor);
            }
            // a new frame is starting
            byteCount = 0;
            pixelIndex = -1;
            pixelColor = 0;
        }
        else
        {
       
            // already read #, so this is the pixelIndex
            if (byteCount == 1)
            {
                Serial.print("PI ");
                pixelIndex = b;
                Serial.println(b);
            }
            else
            {
                Serial.print(byteCount - 2);
                Serial.print('|');
                Serial.print(b, HEX);
                Serial.print("=");

                if (byteCount >= 2) // lsb of color
                {
                    unsigned long shift = (4 - byteCount + 1) * 8;
                    Serial.print(shift);
                    Serial.print('<');
                    unsigned long value = (b << shift);
                    //pixelColor = pixelColor + (b << shift);
                    Serial.println(value, BIN);
                }
            }
        }
        byteCount++;
    }
}
