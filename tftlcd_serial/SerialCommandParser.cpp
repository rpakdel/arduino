// 
// 
// 

#include "SerialCommandParser.h"


SerialCommand& SerialCommandParser::parseCommand(const String& str)
{
    for (int i = 1; i < commandsCount; ++i)
    {
        SerialCommand* cmd = commands[i];
        if (cmd->isMatch(str))
        {
            return *cmd;
        }
    }

    // return unknown
    return *commands[0];
}

SerialCommandParser::SerialCommandParser(TFTLCDColorParser& inColorParser)
    :
    colorParser(inColorParser)
{
    commandsCount = 10;
    commands = new SerialCommand*[commandsCount];

    int index = 0;
    commands[index++] = new SerialCommand(commandsCount++, unknown, "unknown");
    commands[index++] = new SerialCommand(commandsCount++, fillScreen, "fillScreen");
    commands[index++] = new SerialCommand(commandsCount++, print, "print");
    commands[index++] = new SerialCommand(commandsCount++, println, "println");
    commands[index++] = new SerialCommand(commandsCount++, setTextColor, "setTextColor");
    commands[index++] = new SerialCommand(commandsCount++, setTextSize, "setTextSize");
    commands[index++] = new SerialCommand(commandsCount++, setTextWrap, "setTextWrap");
    commands[index++] = new SerialCommand(commandsCount++, setCursor, "setCursor");
    commands[index++] = new SerialCommand(commandsCount++, drawPixel, "drawPixel");
    commands[index++] = new SerialCommand(commandsCount++, drawLine, "drawLine");

    if (index != commandsCount)
    {
        Serial.println("WRONG COMMANDSCOUNT");
    }
}

int SerialCommandParser::parseColor(const String& colorName)
{
    return colorParser.getColor(colorName);
}
