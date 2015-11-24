/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SSD1306Ascii.h>
#ifdef OLED_SPI
#else
#include <SSD1306AsciiWire.h>

#endif

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include <printf.h>

RF24 radio(9, 10);
static const int receiverAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

bool oledAvailable = true;

void initDisplay()
{
    if (!oledAvailable)
    {
        return;
    }

    Serial.println(F("Init OLED"));
    Wire.begin();
    display.begin(&Adafruit128x64, I2C_ADDRESS);
    display.set400kHz();
    display.setFont(Adafruit5x7);
    display.clear();
    display.setCursor(0, 0);
    display.println(F("Receiver"));
    Serial.println(F("OLED ready"));
}


void setup()
{
    Serial.begin(9600);
    Serial.println(F("Setup"));

    initDisplay();

    delay(1000);

    Serial.println(F("Receiver"));

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.openReadingPipe(1, receiverAdd[0]);
    radio.openReadingPipe(2, receiverAdd[1]);
    radio.startListening();
}

void displayBuffer(char* buffer, uint8_t pipeNum)
{
    if (oledAvailable)
    {
        int len = strlen(buffer);
        display.setCursor(0, 0);

        display.clearToEOL();
        display.print(F("Receiver "));
        display.println(pipeNum);
        display.println();

        if (pipeNum == 2)
        {
            display.setCursor(0, 5);
        }

        for (int i = 0; i < len; ++i)
        {
            if (buffer[i] == ',')
            {
                display.println();
                display.clearToEOL();
            }
            else
            {
                display.print(buffer[i]);
            }

        }
    }

    Serial.print(F("Buffer: "));
    Serial.println(buffer);
}

int numNoRadio = 0;
void loop()
{
    char buffer[32];
    buffer[0] = '\0';

    uint8_t pipeNum = 0;
    if (radio.available(&pipeNum))
    {
        numNoRadio = 0;

        digitalWrite(13, HIGH);
        radio.read(&buffer, 32);

        displayBuffer(buffer, pipeNum);
        digitalWrite(13, LOW);
    }
    else
    {
        numNoRadio++;
        if (numNoRadio > 300)
        {
            numNoRadio = 0;
            display.clear();
            display.print("Receiver, no data");
        }
    }

    delay(10);
}
