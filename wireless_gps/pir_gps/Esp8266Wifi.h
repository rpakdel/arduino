#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266.h>

#include "Gps.h"

#define SSID        "Area51"
#define PASSWORD    "bullseye"

#define WIFI_RX 10
#define WIFI_TX 11
#define WIFI_BAUD 9600

class Esp8266Wifi
{
public:
    Esp8266Wifi(Gps& gps);
    ~Esp8266Wifi();

    bool Setup();
    void Loop();
private:
    SoftwareSerial* wifiSerial;
    ESP8266* wifi;

    void PrintBuffer(uint8_t buffer[], uint32_t length);
    void SetBuffer(uint8_t buffer[], uint32_t length, int pir);
    uint32_t GetBufferLen(uint8_t buffer[]);
    Gps& mGps;
};

#endif