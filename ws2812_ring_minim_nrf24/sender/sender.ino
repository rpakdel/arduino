
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include <SoftwareSerial.h>

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display = SSD1306AsciiWire();

RF24 radio = RF24(9, 10);
static const int senderAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };

static const int sender = 0;

bool oledAvailable = true;

#define FRAME_SIZE 6
#define PAYLOAD_SIZE 32

int buffer[PAYLOAD_SIZE];
int bufferIndex = 0;

#define SERIAL_DATA_RATE 115200

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
    display.set2X();
    display.clear();    
    display.setCursor(0, 0);
	display.print(F("BOOT"));    
    Serial.println(F("OLED ready"));
}


void setup() 
{
	clearBuffer(buffer, FRAME_SIZE);

    Serial.begin(SERIAL_DATA_RATE);
    Serial.println(F("Setup"));
    
    
    initDisplay();
    
    delay(1000);
    Serial.print(F("BOOT "));
    Serial.println(sender);

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
	radio.setDataRate(RF24_2MBPS);
    radio.openWritingPipe(senderAdd[sender]);
    radio.stopListening();
	delay(1000);
	display.setCursor(0, 0);
	display.print(F("Sender #"));
	display.print(sender);
}

void displayBuffer(int* buff, int len)
{
    if (oledAvailable)
    {        
		display.set2X();
		display.setCursor(0, 2);
		display.clearToEOL();

        for (int i = 0; i < len; ++i)
        {
			int b = buff[i];
			if (b == '#')
			{
				display.print(F("#"));
			}
			else
			{
				display.print(buff[i], HEX);
			}
        }
    }
    
    Serial.print(F("Buffer: "));
    Serial.println((char*)buff);
}

void displaySendStatus(bool success)
{    
    if (!oledAvailable)
    {
        return;
    }
    
	display.set2X();
    int x = 128 - 2 * 6;
    int y = 0;
    // clear the char
	display.setCursor(x, y);
    display.clear(x, x + 2 * 6, y, y + 2 * 8);
    
    if (success)
    {
        display.print(F("^"));
    }
    else
    {
        display.print(F("x"));
    }
}

void clearBuffer(int* buff, int len)
{
	for (int i = 0; i < len; i++)
	{
		buff[i] = 0;
	}
}

void sendBuffer(int* buff, int len)
{
	//displayBuffer(buff, len);
	bool sendResult = radio.write(buff, PAYLOAD_SIZE);
	//displaySendStatus(sendResult);
}

void nextByte(int b)
{
	if (b == '#')
	{
		if (bufferIndex == FRAME_SIZE)
		{
			sendBuffer(buffer, FRAME_SIZE);
			clearBuffer(buffer, FRAME_SIZE);
		}

		// a new frame is starting
		bufferIndex = 0;
	}
	else if (bufferIndex >= FRAME_SIZE)
	{
		bufferIndex = 0;
	}

	buffer[bufferIndex] = b;
	bufferIndex++;

	
}

unsigned int count;
void loop()
{
	while (Serial.available())
	{
		int b = Serial.read();
		nextByte(b);
	}
}
