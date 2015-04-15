// 
// 
// 

#include "CommandExec.h"

CommandExec::CommandExec(SerialCommandParser& inParser, Adafruit_TFTLCD* inTft)
    : 
    parser(inParser), 
    tft(inTft)
{
}

void CommandExec::exec(const String& str)
{
    SerialCommand& command = parser.parseCommand(str);

    if (command.getGfxCommand() == unknown)
    {
        Serial.print("UNKNOWN COMMAND '");
        Serial.print(str);
        Serial.println("'");
    }

    String param = command.parseParam(str);
    execCommand(command, param);
}

void CommandExec::fillScreen(const String& colorName)
{
    int color = parser.parseColor(colorName);
    tft->fillScreen(color);
}

void CommandExec::print(const String& str, bool newLine)
{
    if (newLine)
    {
        tft->println(str);
    }
    else
    {
        tft->print(str);
    }
}

void CommandExec::setTextColor(const String& colorName)
{
    int index = colorName.indexOf(',');
    if (index >= 0)
    {
        int fcolor = parser.parseColor(colorName.substring(0, index));
        int bgcolor = parser.parseColor(colorName.substring(index + 1));
        tft->setTextColor(fcolor, bgcolor);
    }
    else
    {
        int color = parser.parseColor(colorName);
        tft->setTextColor(color);
    }
    
}

void CommandExec::setTextSize(const String& sizeStr)
{
    int size = atoi(sizeStr.c_str());
    tft->setTextSize(size);
}

void CommandExec::drawPixel(const String& xyColor)
{
    String split[3];
    if (!splitParameters("drawPixel", xyColor, split, 3))
    {
        return;
    }

    int index = 0;
    int x = atoi(split[index++].c_str());
    int y = atoi(split[index++].c_str());
    int color = parser.parseColor(split[index++]);

    tft->drawPixel(x, y, color);
}

void CommandExec::drawLine(const String& x0y0x1y1Color)
{
    String split[5];
    if (!splitParameters("drawLine", x0y0x1y1Color, split, 5))
    {
        return;
    }

    int x0 = atoi(split[0].c_str());
    int y0 = atoi(split[1].c_str());
    int x1 = atoi(split[2].c_str());
    int y1 = atoi(split[3].c_str());
    int color = parser.parseColor(split[4]);
    tft->drawLine(x0, y0, x1, y1, color);
}

void CommandExec::setCursor(const String& xy)
{
    int comma = xy.indexOf(',');
    if (comma < 0)
    {
        printBadCommandParam("setCursor", xy);
    }
    else
    {
        int x = atoi(xy.substring(0, comma).c_str());
        int y = atoi(xy.substring(comma + 1).c_str());
        tft->setCursor(x, y);
    }
}

void CommandExec::execCommand(const SerialCommand& command, const String& param)
{
    Serial.print("Command: ");
    Serial.print(command.getGfxCommandName());
    Serial.print(", param: ");
    Serial.println(param);

    if (command.getGfxCommand() == unknown)
    {
        return;
    }

    switch (command.getGfxCommand())
    {
    case ::fillScreen:
        this->fillScreen(param);
        return;
    case ::print:
        print(param, false);
        return;
    case ::println:
        print(param, true);
        return;
    case ::setTextColor:
        setTextColor(param);
        return;
    case ::setTextSize:
        setTextSize(param);
        return;
    case ::setCursor:
        setCursor(param);
        return;
    case ::drawPixel:
        drawPixel(param);
        return;
    case ::drawLine:
        drawLine(param);
        return;
    }

    Serial.print("EXEC NOT MAPPED FOR ");
    Serial.println(command.getGfxCommandName());
}

void CommandExec::printBadCommandParam(const String& command, const String& param)
{
    Serial.print("BAD PARAM FOR ");
    Serial.print(command);
    Serial.print(" '");
    Serial.print(param);
    Serial.println("'");
}

bool CommandExec::splitParameters(const String& command, const String& param, String split[], int numParams)
{
    if (numParams == 0)
    {
        return true;
    }

    if (numParams == 1)
    {
        split[0] = param;
        return true;
    }

    int lastCommaIndex = -1;
    int splitIndex = 0;
    for (splitIndex = 0; splitIndex < numParams - 1; ++splitIndex)
    {
        int commaIndex = param.indexOf(',', lastCommaIndex + 1);
        if (commaIndex < 0)
        {
            printBadCommandParam(command, param);
        }

        split[splitIndex] = param.substring(lastCommaIndex + 1, commaIndex);
        lastCommaIndex = commaIndex;
    }
    split[splitIndex] = param.substring(lastCommaIndex + 1, param.length());
    return true;
}
