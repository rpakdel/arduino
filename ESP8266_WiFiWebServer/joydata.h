#pragma once

#include <Arduino.h>

typedef struct _JoyData
{
    // bot id
	int bid;
	// Joy-X value from -255 to 255
	int X;
	// Joy-Y value from -255 to 255
	int Y;
} JoyData;

void printJoyData(JoyData& joyData, Print& print)
{
	print.print(joyData.bid);
	print.print(",");
	print.print(joyData.X);
	print.print(",");
	print.print(joyData.Y);
}

void printlnJoyData(JoyData& joyData, Print& print)
{
	printJoyData(joyData, print);
	print.println();
}