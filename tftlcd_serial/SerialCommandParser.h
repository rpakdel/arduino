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
     SerialCommand& parseCommand(const String& str);
     int parseColor(const String& colorName);
private:
    SerialCommand commands[20];
    int commandsCount = 20;
    TFTLCDColorParser colorParser;
};


#endif

