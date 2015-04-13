// CommandExec.h

#ifndef _COMMANDEXEC_h
#define _COMMANDEXEC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_TFTLCD.h>
#include "GfxCommand.h"
#include "SerialCommandParser.h"

class CommandExec
{
 public:
     CommandExec(SerialCommandParser& inParser, Adafruit_TFTLCD* inTft);
     void exec(String str);
private:
    SerialCommandParser parser;
    Adafruit_TFTLCD* tft;
    
    void execCommand(SerialCommand* command, String param);
    void setTextSize(String sizeStr);
    void setTextColor(String colorName);
    void print(String str, bool newLine);
    void fillScreen(String colorName);
    void setCursor(String xy);
    void drawPixel(String xyColor);


    void printBadCommandParam(String cmdName, String param);
};

#endif

