// SerialCommand.h
#include "GfxCommand.h"

#ifndef _SERIALCOMMAND_h
#define _SERIALCOMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SerialCommand
{
 public:
    SerialCommand(int inCommandIndex, GfxCommand inCmd, const String& inCommandName);
    SerialCommand(const SerialCommand& serialCommand);
    bool isMatch(const String& str);
    GfxCommand getGfxCommand() const;
    String getGfxCommandName() const;
    int getTokenLen() const;
    int getCommandIndex() const;
    String parseParam(const String& str) const;
 private:
     int commandIndex;
     GfxCommand command;
     int tokenLen;
     String token;
     String commandName;
};
#endif

