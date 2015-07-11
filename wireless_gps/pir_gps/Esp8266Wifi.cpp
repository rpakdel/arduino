#include "Esp8266Wifi.h"


Esp8266Wifi::Esp8266Wifi(Gps& gps) :
    mGps(gps)
{
    wifi = NULL;
    wifiSerial = NULL;
}


Esp8266Wifi::~Esp8266Wifi()
{
    if (wifi != NULL)
    {
        delete wifi;
        wifi = NULL;
    }
    
    if (wifiSerial != NULL)
    {
        delete wifiSerial;
        wifiSerial = NULL;
    }
    
}

bool Esp8266Wifi::Setup()
{
    Serial.println("ESP8266> Setup");

    wifiSerial = new SoftwareSerial(WIFI_RX, WIFI_TX);
    wifiSerial->begin(WIFI_BAUD);

    wifi = new ESP8266(*wifiSerial, WIFI_BAUD);

    if (wifi->setOprToStation())
    {
        Serial.println("ESP8266> Set to station mode");
    }
    else
    {
        Serial.println("ESP8266> Failed to set to station mode");
        return false;
    }

    if (wifi->joinAP(SSID, PASSWORD))
    {
        Serial.println("ESP8266> Joined wireless network " + String(SSID));
        Serial.println("ESP8266> IP = " + wifi->getLocalIP());
    }
    else
    {
        Serial.println("ESP8266> Failed connecting to " + String(SSID));
        return false;
    }

    if (wifi->enableMUX())
    {
        Serial.println("ESP8266> Enabled multiple connections");
    }
    else
    {
        Serial.println("ESP8266> Could not enable multiple connections");
        return false;
    }

    if (wifi->startTCPServer(80))
    {
        Serial.println("ESP8266> TCP Started on port 80");
    }
    else
    {
        Serial.println("ESP8266> Failed to start TCP server");
        return false;
    }

    if (wifi->setTCPServerTimeout(3))
    {
        Serial.println("ESP8266> Set TCP server timout to 3s");
    }
    else
    {
        Serial.println("ESP8266> Failed to set TCP server timout");
        return false;
    }

    return true;
}

void Esp8266Wifi::Loop()
{
    uint8_t pir = 0;
    uint8_t buffer[128] = { 0 };
    uint8_t mux_id;
    uint32_t len = wifi->recv(&mux_id, buffer, sizeof(buffer), 100);
    if (len > 0)
    {
        Serial.print("Status:[");
        Serial.print(wifi->getIPStatus().c_str());
        Serial.println("]");

        Serial.print("Received from :");
        Serial.println(mux_id);

        PrintBuffer(buffer, len);
        SetBuffer(buffer, len, pir);
        if (wifi->send(mux_id, buffer, GetBufferLen(buffer)))
        {
            Serial.print("sent back ");
            PrintBuffer(buffer, len);
        }
        else
        {
            Serial.print("send back err");
        }

        if (wifi->releaseTCP(mux_id))
        {
            Serial.print("release tcp ");
            Serial.print(mux_id);
            Serial.println(" ok");
        }
        else
        {
            Serial.print("release tcp");
            Serial.print(mux_id);
            Serial.println(" err");
        }
    }
}

void Esp8266Wifi::PrintBuffer(uint8_t buffer[], uint32_t length)
{
    Serial.print("buffer = [");
    Serial.print((char*)buffer);
    Serial.print("], len = ");
    Serial.println(GetBufferLen(buffer));
}

void Esp8266Wifi::SetBuffer(uint8_t buffer[], uint32_t length, int pir)
{
    sprintf((char*)buffer, "%i", pir);
}

uint32_t Esp8266Wifi::GetBufferLen(uint8_t buffer[])
{
    return strlen((char*)buffer);
}

