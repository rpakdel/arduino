#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

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

bool jsonDeserializeJoyData(char* data, JoyData& joyData)
{
	StaticJsonBuffer<255> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(data);
	if (!root.success())
	{
		return false;
	}
	joyData.bid = root["bid"];
	joyData.X = root["x"];
	joyData.Y = root["y"];
	return true;
}