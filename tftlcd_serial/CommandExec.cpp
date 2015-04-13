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

void CommandExec::exec(String str)
{
    SerialCommand* command = parser.parseCommand(str);

    if (command->getGfxCommand() == unknown)
    {
        Serial.print("UNKNOWN COMMAND '");
        Serial.print(str);
        Serial.println("'");
    }

    String param = command->parseParam(str);
    execCommand(command, param);
}

void CommandExec::fillScreen(String colorName)
{
    int color = parser.parseColor(colorName);
    tft->fillScreen(color);
}

void CommandExec::print(String str, bool newLine)
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

void CommandExec::setTextColor(String colorName)
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

void CommandExec::setTextSize(String sizeStr)
{
    int size = atoi(sizeStr.c_str());
    tft->setTextSize(size);
}

void CommandExec::drawPixel(String xyColor)
{
    int comma1 = xyColor.indexOf(',');
    if (comma1 < 0)
    {
        printBadCommandParam("drawPixel", xyColor);
        return;
    }
    
    int comma2 = xyColor.indexOf(',', comma1 + 1);
    if (comma2 < 0)
    {
        printBadCommandParam("drawPixel", xyColor);
        return;
    }

    int x = atoi(xyColor.substring(0, comma1).c_str());
    int y = atoi(xyColor.substring(comma1 + 1, comma2).c_str());
    int color = parser.parseColor(xyColor.substring(comma2 + 1).c_str());

    tft->drawPixel(x, y, color);
}

void CommandExec::execCommand(SerialCommand* command, String param)
{
    Serial.print("Command: ");
    Serial.print(command->getGfxCommandName());
    Serial.print(", param: ");
    Serial.println(param);

    if (command->getGfxCommand() == unknown)
    {
        return;
    }

    switch (command->getGfxCommand())
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
    }

    Serial.print("EXEC NOT MAPPED FOR ");
    Serial.println(command->getGfxCommandName());
}

void CommandExec::setCursor(String xy)
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

void CommandExec::printBadCommandParam(String command, String param)
{
    Serial.print("BAD PARAM FOR ");
    Serial.print(command);
    Serial.print(" '");
    Serial.print(param);
    Serial.println("'");
}

