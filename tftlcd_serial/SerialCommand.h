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
    SerialCommand();
    SerialCommand(GfxCommand inCmd);
    SerialCommand(const SerialCommand& serialCommand);
    bool isMatch(const char* str) const;
    GfxCommand getGfxCommand() const;
    String getGfxCommandName() const;
    String getToken() const;
    uint16_t getTokenLen() const;
    uint16_t getCommandIndex() const;
    String parseParam(const String& str) const;
 private:
     GfxCommand command;
};
#endif

