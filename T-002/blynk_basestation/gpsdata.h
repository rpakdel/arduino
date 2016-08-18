#pragma once

#include <Arduino.h>

typedef struct _GpsData
{
	// bot id
	int bid;
	// whether gps data is valid
	int valid;
	// longtitude from -180 to 180
	float lon;
	// latitude from -90 to 90
	float lat;
} GpsData;

void printGpsData(GpsData& gpsData, Print& print)
{
	print.print(gpsData.bid);
	print.print(",");
	print.print(gpsData.valid);
	print.print(",");
	print.print(gpsData.lon);
	print.print(",");
	print.print(gpsData.lat);
}

void printlnGpsData(GpsData& gpsData, Print& print)
{
	printGpsData(gpsData, print);
	print.println();
}

size_t jsonSerializeGpsData(GpsData &gpsData, char* buffer, size_t bufferSize)
{
	if (!gpsData.valid)
	{
		return 0;
	}

	StaticJsonBuffer<255> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& gps = root.createNestedObject("gps");
	gps["bid"] = gpsData.bid;
	gps["valid"] = gpsData.valid;
	gps["lng"] = gpsData.lon;
	gps["lat"] = gpsData.lat;

	return gps.printTo(buffer, bufferSize);
}