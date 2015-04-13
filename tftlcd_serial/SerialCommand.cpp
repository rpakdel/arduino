// 
// 
// 

#include "SerialCommand.h"

SerialCommand::SerialCommand(int inCommandIndex, GfxCommand inCmd, String inCommandName)
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

bool SerialCommand::isMatch(String str)
{
    String i(str);
    i.toUpperCase();
    return i.startsWith(this->token);
}

GfxCommand SerialCommand::getGfxCommand()
{
    return this->command;
}

String SerialCommand::getGfxCommandName()
{
    return this->commandName;
}

int SerialCommand::getTokenLen()
{
    return this->tokenLen;
}

int SerialCommand::getCommandIndex()
{
    return this->commandIndex;
}

String SerialCommand::parseParam(String str)
{
    return str.substring(this->tokenLen);
}
