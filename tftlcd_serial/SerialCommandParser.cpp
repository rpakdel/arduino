// 
// 
// 

#include "SerialCommandParser.h"


SerialCommand& SerialCommandParser::parseCommand(const String& str)
{
    for (int i = 1; i < commandsCount; ++i)
    {
        SerialCommand& cmd = commands[i];
        if (cmd.isMatch(&str.c_str()[0]))
        {
            return cmd;
        }
    }

    // return unknown
    return commands[0];
}

SerialCommandParser::SerialCommandParser(TFTLCDColorParser& inColorParser)
    :
    colorParser(inColorParser)
{
    commands[unknown] = SerialCommand(unknown);
    commands[print] = SerialCommand(print);
    commands[println] = SerialCommand(println);
    commands[setTextColor] = SerialCommand(setTextColor);
    commands[setTextSize] = SerialCommand(setTextSize);
    commands[setTextWrap] = SerialCommand(setTextWrap);
    commands[setCursor] = SerialCommand(setCursor);
    commands[drawPixel] = SerialCommand(drawPixel);
    commands[drawLine] = SerialCommand(drawLine);
    commands[drawRect] = SerialCommand(drawRect);
    commands[fillRect] = SerialCommand(fillRect);
    commands[fillScreen] = SerialCommand(fillScreen);
    commands[drawCircle] = SerialCommand(drawCircle);
    commands[fillCircle] = SerialCommand(fillCircle);
    commands[drawTriangle] = SerialCommand(drawTriangle);
    commands[fillTriangle] = SerialCommand(fillTriangle);
    commands[drawRoundRect] = SerialCommand(drawRoundRect);
    commands[fillRoundRect] = SerialCommand(fillRoundRect);
    commands[drawChar] = SerialCommand(drawChar);
    commands[setRotation] = SerialCommand(setRotation);
}

int SerialCommandParser::parseColor(const String& colorName)
{
    return colorParser.getColor(colorName);
}
