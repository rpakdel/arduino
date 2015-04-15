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
     void exec(const String& str);
private:
    SerialCommandParser parser;
    Adafruit_TFTLCD* tft;
    
    void execCommand(const SerialCommand& command, const String& param);
    void setTextSize(const String& sizeStr);
    void setTextColor(const String& colorName);
    void print(const String& str, bool newLine);
    void fillScreen(const String& colorName);
    void setCursor(const String& xy);
    void drawPixel(const String& xyColor);
    void drawLine(const String& x0y0x1y1Color);

    bool splitParameters(const String& command, const String& param, String split[], int numParams);
    void printBadCommandParam(const String& cmdName, const String& param);

};

#endif

