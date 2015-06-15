// 
// 
// 

#include "SerialCommand.h"

SerialCommand::SerialCommand()
    :
    command(unknown)
{

}

SerialCommand::SerialCommand(GfxCommand inCmd)
    : 
    command(inCmd)
{
}

SerialCommand::SerialCommand(const SerialCommand& serialCommand)
    : 
    command(serialCommand.command)
{
}

String SerialCommand::getToken() const
{
    String token = getGfxCommandName() + ":";
    token.toUpperCase();
    return token;
}

bool SerialCommand::isMatch(const char* str) const
{
    String i(str);
    i.toUpperCase();
    String token = getToken();
    return i.startsWith(token);
}

GfxCommand SerialCommand::getGfxCommand() const
{
    return this->command;
}

String SerialCommand::getGfxCommandName() const
{
    char buffer[30];
    strcpy_P(buffer, (char*)pgm_read_word(&(GfxCommand_table[getCommandIndex()])));
    return String(buffer);
}

uint16_t SerialCommand::getTokenLen() const
{
    return getToken().length();
}

uint16_t SerialCommand::getCommandIndex()  const
{
    return this->command;
}

String SerialCommand::parseParam(const String& str) const
{
    return str.substring(getTokenLen());
}
