#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus/TinyGPS++.h>

#define GPS_RX 8
#define GPS_TX 9
#define GPS_BAUD 9600

class Gps
{
public:
    Gps();
    ~Gps();
    void Setup();
    void Loop();

    TinyGPSPlus& GetTiny();
    bool GetLatLngAlt(char* buffer, int len);
    
private:
    SoftwareSerial* gpsSerial;
    TinyGPSPlus* mTiny;

    void displayInfo();
    void printXYZ();
    void ToggleLED();
    void SetLED(bool on);
    double DegToRad(double deg);
    bool ledOn;
};

#endif