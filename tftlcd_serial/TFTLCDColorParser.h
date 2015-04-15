// TFTLCDColor.h

#ifndef _TFTLCDCOLOR_h
#define _TFTLCDCOLOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0xFFFF - 0x0000
#define	BLUE    0xFFFF - 0x001F
#define	RED     0xFFFF - 0xF800
#define	GREEN   0xFFFF - 0x07E0
#define CYAN    0xFFFF - 0x07FF
#define MAGENTA 0xFFFF - 0xF81F
#define YELLOW  0xFFFF - 0xFFE0
#define WHITE   0xFFFF - 0xFFFF

const String colorNames[] = { "BLACK", "BLUE", "RED", "GREEN", "CYAN", "MAGENTA", "YELLOW", "WHITE" };
const int colors[] = { BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE };
const int colorsCount = 8;

class TFTLCDColorParser
{
 protected:


 public:
     int getColor(const String& colorName);
};


#endif

