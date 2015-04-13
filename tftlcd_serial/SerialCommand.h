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
    SerialCommand(int inCommandIndex, GfxCommand inCmd, String inCommandName);
    SerialCommand(const SerialCommand& serialCommand);
    bool isMatch(String str);
    GfxCommand getGfxCommand();
    String getGfxCommandName();
    int getTokenLen();
    int getCommandIndex();
    String parseParam(String str);
 private:
     int commandIndex;
     GfxCommand command;
     int tokenLen;
     String token;
     String commandName;
};
#endif

