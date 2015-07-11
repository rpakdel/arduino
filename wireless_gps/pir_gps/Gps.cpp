#include "Gps.h"

Gps::Gps()
{
    mTiny = NULL;
    gpsSerial = NULL;
}


Gps::~Gps()
{
    if (mTiny != NULL)
    {
        delete mTiny;
        mTiny = NULL;
    }

    if (gpsSerial != NULL)
    {
        delete gpsSerial;
        gpsSerial = NULL;
    }
}

void Gps::Setup()
{
    gpsSerial = new SoftwareSerial(GPS_RX, GPS_TX);
    gpsSerial->begin(GPS_BAUD);

    mTiny = new TinyGPSPlus();
}

void Gps::Loop()
{
    // This sketch displays information every time a new sentence is correctly encoded.
    while (gpsSerial->available() > 0)
    {
        if (mTiny->encode(gpsSerial->read()))
        {
            displayInfo();
        }
    }
}

TinyGPSPlus& Gps::GetTiny()
{
    return *(this->mTiny);
}

void Gps::ToggleLED()
{
    SetLED(!ledOn);
}

void Gps::SetLED(bool on)
{
    ledOn = on;
    if (ledOn)
    {
        digitalWrite(13, HIGH);
    }
    else
    {
        digitalWrite(13, LOW);
    }
}

bool Gps::GetLatLngAlt(char* buffer, int len)
{
    if (!mTiny->location.isValid())
    {
        return false;
    }

    char lat[32];
    dtostrf(mTiny->location.lat(), 9, 6, lat);
    char lng[32];
    dtostrf(mTiny->location.lng(), 10, 6, lng);
    char alt[32];
    dtostrf(mTiny->altitude.meters(), 6, 2, alt);
    snprintf(
        buffer, 
        len, 
        "%s, %s, %s", 
        lat, lng, alt);
    return true;
}

void Gps::displayInfo()
{
    char buffer[255];
    if (GetLatLngAlt(buffer, 255))
    {
        Serial.println(buffer);
    }

    return;
    Serial.print(F("Location: "));
    if (mTiny->location.isValid())
    {
        Serial.print(mTiny->location.lat(), 6);
        Serial.print(F(","));
        Serial.print(mTiny->location.lng(), 6);
        printXYZ();
    }
    else
    {
        Serial.print(F("INVALID"));
        SetLED(false);
    }

    Serial.print(F(" Alt: "));
    if (mTiny->altitude.isValid())
    {
        Serial.print(mTiny->altitude.feet(), 6);
        Serial.print(" ft");
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (mTiny->date.isValid())
    {
        Serial.print(mTiny->date.month());
        Serial.print(F("/"));
        Serial.print(mTiny->date.day());
        Serial.print(F("/"));
        Serial.print(mTiny->date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (mTiny->time.isValid())
    {
        if (mTiny->time.hour() < 10) Serial.print(F("0"));
        Serial.print(mTiny->time.hour());
        Serial.print(F(":"));
        if (mTiny->time.minute() < 10) Serial.print(F("0"));
        Serial.print(mTiny->time.minute());
        Serial.print(F(":"));
        if (mTiny->time.second() < 10) Serial.print(F("0"));
        Serial.print(mTiny->time.second());
        Serial.print(F("."));
        if (mTiny->time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(mTiny->time.centisecond());
        Serial.print(" UTC");
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}

double Gps::DegToRad(double deg)
{
    return deg * 0.01745278;
}

void Gps::printXYZ()
{
    int earthRadius = 6367; // km
    double lat = DegToRad(mTiny->location.lat());
    double lon = DegToRad(mTiny->location.lng());
    double x = earthRadius * cos(lat) * cos(lon);
    double y = earthRadius * cos(lat) * sin(lon);
    double z = earthRadius * sin(lat);
    Serial.print(" XYZ(");
    Serial.print(x, 2);
    Serial.print(", ");
    Serial.print(y, 2);
    Serial.print(", ");
    Serial.print(z, 2);
    Serial.print(")");
}