// 
// 
// 

#include "SerialCommand.h"

SerialCommand::SerialCommand(int inCommandIndex, GfxCommand inCmd, const String& inCommandName)
    : 
    commandIndex(inCommandIndex),
    command(inCmd), 
    commandName(inCommandName)
{
    this->token = inCommandName + ":";
    this->token.toUpperCase();
    this->tokenLen = this->token.length();
}

SerialCommand::SerialCommand(const SerialCommand& serialCommand)
    : 
    commandIndex(serialCommand.commandIndex),
    command(serialCommand.command), 
    commandName(serialCommand.commandName),
    token(serialCommand.token),
    tokenLen(serialCommand.tokenLen)
{

}

bool SerialCommand::isMatch(const String& str)
{
    String i(str);
    i.toUpperCase();
    return i.startsWith(this->token);
}

GfxCommand SerialCommand::getGfxCommand() const
{
    return this->command;
}

String SerialCommand::getGfxCommandName() const
{
    return this->commandName;
}

int SerialCommand::getTokenLen() const
{
    return this->tokenLen;
}

int SerialCommand::getCommandIndex()  const
{
    return this->commandIndex;
}

String SerialCommand::parseParam(const String& str) const
{
    return str.substring(this->tokenLen);
}
