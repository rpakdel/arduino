// 
// 
// 

#include "TFTLCDColorParser.h"

int TFTLCDColorParser::getColor(String colorName)
{
    String c(colorName);
    c.toUpperCase();
    for (int i = 0; i < colorsCount; ++i)
    {
        if (c == colorNames[i])
        {
            return colors[i];
        }
    }

    Serial.print("BAD COLOR NAME ");
    Serial.print("'");
    Serial.print(colorName);
    Serial.println("'");
    return BLACK;
}
