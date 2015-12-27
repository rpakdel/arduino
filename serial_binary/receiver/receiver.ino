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
    //Serial.print("=> ");
    Serial.print(pixelIndex, DEC);
    Serial.print(':');
    Serial.println(pixelColor);
}

byte byteIndex = 0;
char pixelIndex = -1;
long pixelColor = 0;

void loop() // run over and over
{   
    while (mySerial.available())
    {
        byte b = mySerial.read();
        if (b == '#')
        {
            //Serial.print(byteIndex);
            //Serial.println("|#");
            // read a whole frame, set the color and reset
            if (byteIndex == 0 || byteIndex == 6)
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
}
