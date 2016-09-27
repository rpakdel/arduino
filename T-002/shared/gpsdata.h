#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

typedef struct _GpsData
{
    // bot id
    byte Id;
    // whether gps data is valid
    byte IsValid;
    // longtitude from -180 to 180
    float Lon;
    // latitude from -90 to 90
    float Lat;
    // altitude in meters
    float Alt;
} GpsData;

const size_t gpsDataSize = sizeof(GpsData);

void printGpsData(GpsData& gpsData, Print& print)
{
    print.print(gpsData.Id);
    print.print(F(","));
    print.print(gpsData.IsValid);
    print.print(F(","));
    print.print(gpsData.Lon);
    print.print(F(","));
    print.print(gpsData.Lat);
    print.print(F(","));
    print.print(gpsData.Alt);
}

void printlnGpsData(GpsData& gpsData, Print& print)
{
    printGpsData(gpsData, print);
    print.println();
}

size_t jsonSerializeGpsData(GpsData &gpsData, char* buffer, size_t bufferSize)
{
    if (!gpsData.IsValid)
    {
        return 0;
    }

    StaticJsonBuffer<255> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    JsonObject& gps = root.createNestedObject("gps");
    gps["Id"] = gpsData.Id;
    gps["IsValid"] = gpsData.IsValid;
    gps["Lon"] = gpsData.Lon;
    gps["Lat"] = gpsData.Lat;
    gps["Alt"] = gpsData.Alt;

    return gps.printTo(buffer, bufferSize);
}