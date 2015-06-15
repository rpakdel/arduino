// GfxCommand.h

#ifndef _COMMAND_h
#define _COMMAND_h

#include <avr/pgmspace.h>
const char GfxCommand_unknown[] PROGMEM = "unknown";   // "String 0" etc are strings to store - change to suit.
const char GfxCommand_print[] PROGMEM = "print";
const char GfxCommand_println[] PROGMEM = "println";
const char GfxCommand_setTextColor[] PROGMEM = "setTextColor";
const char GfxCommand_setTextSize[] PROGMEM = "setTextSize";
const char GfxCommand_setTextWrap[] PROGMEM = "setTextWrap";
const char GfxCommand_setCursor[] PROGMEM = "setCursor";
const char GfxCommand_drawPixel[] PROGMEM = "drawPixel";
const char GfxCommand_drawLine[] PROGMEM = "drawLine";
const char GfxCommand_drawRect[] PROGMEM = "drawRect";
const char GfxCommand_fillRect[] PROGMEM = "fillRect";
const char GfxCommand_fillScreen[] PROGMEM = "fillScreen";
const char GfxCommand_drawCircle[] PROGMEM = "drawCircle";
const char GfxCommand_fillCircle[] PROGMEM = "fillCircle";
const char GfxCommand_drawTriangle[] PROGMEM = "drawTriangle";
const char GfxCommand_fillTriangle[] PROGMEM = "fillTriangle";
const char GfxCommand_drawRoundRect[] PROGMEM = "drawRoundRect";
const char GfxCommand_fillRoundRect[] PROGMEM = "fillRoundRect";
const char GfxCommand_drawChar[] PROGMEM = "drawChar";
const char GfxCommand_setRotation[] PROGMEM = "setRotation";

// order matters because enum integer values are used to access
// the program mem strings
const char* const GfxCommand_table[] PROGMEM = 
{ 
    GfxCommand_unknown, 
    GfxCommand_print, 
    GfxCommand_println,
    GfxCommand_setTextColor,
    GfxCommand_setTextSize,
    GfxCommand_setTextWrap,
    GfxCommand_setCursor,
    GfxCommand_drawPixel,
    GfxCommand_drawLine,
    GfxCommand_drawRect,
    GfxCommand_fillRect,
    GfxCommand_fillScreen,
    GfxCommand_drawCircle,
    GfxCommand_fillCircle,
    GfxCommand_drawTriangle,
    GfxCommand_fillTriangle,
    GfxCommand_drawRoundRect,
    GfxCommand_fillRoundRect,
    GfxCommand_drawChar,
    GfxCommand_setRotation
};

// order matters because enum integer values are used to access
// the program mem strings
typedef enum GfxCommand
{
    unknown = 0,
    print,
    println,
    setTextColor,
    setTextSize,
    setTextWrap,
    setCursor,
    drawPixel,
    drawLine,
    drawRect,
    fillRect,
    fillScreen,
    drawCircle,
    fillCircle,
    drawTriangle,
    fillTriangle,
    drawRoundRect,
    fillRoundRect,
    drawChar,
    setRotation
};

#endif

