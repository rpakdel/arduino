#pragma once

#include <Arduino.h>

typedef struct _GpsData
{
	// bot id
	int bid;
	// longtitude from -180 to 180
	float lon;
	// latitude from -90 to 90
	float lat;
} GpsData;

void printGpsData(GpsData& gpsData, Print& print)
{
	print.print(gpsData.bid);
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