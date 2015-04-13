// SerialCommandParser.h

#ifndef _SERIALCOMMANDPARSER_h
#define _SERIALCOMMANDPARSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GfxCommand.h"
#include "SerialCommand.h"
#include "TFTLCDColorParser.h"

class SerialCommandParser
{
 public:
     SerialCommandParser(TFTLCDColorParser& inColorParser);
     SerialCommand* parseCommand(String str);
     int parseColor(String colorName);
private:
    SerialCommand** commands;
    int commandsCount = 0;
    TFTLCDColorParser colorParser;
};


#endif

