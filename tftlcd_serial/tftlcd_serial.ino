#include "SerialCommandParser.h"
#include "TFTLCDColorParser.h"
#include "CommandExec.h"
#include "SerialCommand.h"
#include "GfxCommand.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined(__AVR_ATmega8__)

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#endif

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
TFTLCDColorParser colorParser;
SerialCommandParser parser(colorParser);
CommandExec commandExec(parser, &tft);

void setup(void) {
    Serial.begin(921600);
    uint16_t identifier = 0x9341;
    tft.begin(identifier);
    tft.fillScreen(BLUE);
    tft.setTextSize(5);
    tft.setTextColor(YELLOW);
    tft.println("READY");
    
}

void loop(void) 
{
    while (Serial.available() > 0)
    {
        String str = Serial.readStringUntil('\n');
        commandExec.exec(str);
    }
}
