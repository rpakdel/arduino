#include <SoftwareSerial.h>
#include <ESP8266.h>

#define PIR 9
#define LED 13

#define SSID        "Area51"
#define PASSWORD    "bullseye"

SoftwareSerial wifiSerial(10, 11);
ESP8266 wifi(wifiSerial);

bool SetupWifi()
{
    wifiSerial.begin(9600);
    Serial.println("ESP8266> Setup");

    if (wifi.setOprToStation())
    {
        Serial.println("ESP8266> Set to station mode");
    }
    else
    {
        Serial.println("ESP8266> Failed to set to station mode");
        return false;
    }

    if (wifi.joinAP(SSID, PASSWORD))
    {
        Serial.println("ESP8266> Joined wireless network " + String(SSID));
        Serial.println("ESP8266> IP = " + wifi.getLocalIP());
    }
    else
    {
        Serial.println("ESP8266> Failed connecting to " + String(SSID));
        return false;
    }

    if (wifi.enableMUX())
    {
        Serial.println("ESP8266> Enabled multiple connections");
    }
    else
    {
        Serial.println("ESP8266> Could not enable multiple connections");
        return false;
    }

    if (wifi.startTCPServer(80))
    {
        Serial.println("ESP8266> TCP Started on port 80");
    }
    else
    {
        Serial.println("ESP8266> Failed to start TCP server");
        return false;
    }

    if (wifi.setTCPServerTimeout(3))
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

// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(9600);
    // initialize digital pin 13 as an output.
    pinMode(LED, OUTPUT);
    pinMode(PIR, INPUT);
    if (!SetupWifi())
    {
        Serial.println("Could not setup wireless TCP server");
    }
}

void PrintBuffer(uint8_t buffer[], uint32_t length)
{
    Serial.print("buffer = [");
    Serial.print((char*)buffer);
    Serial.print("], len = ");
    Serial.println(GetBufferLen(buffer));
}

void SetBuffer(uint8_t buffer[], uint32_t length, int pir)
{
    sprintf((char*)buffer, "%i", pir);
}

uint32_t GetBufferLen(uint8_t buffer[])
{
    return strlen((char*)buffer);
}

void TCPServerLoop(uint8_t pir)
{
    uint8_t buffer[128] = { 0 };
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    if (len > 0)
    {
            Serial.print("Status:[");
            Serial.print(wifi.getIPStatus().c_str());
            Serial.println("]");

            Serial.print("Received from :");
            Serial.println(mux_id);

            PrintBuffer(buffer, len);
            SetBuffer(buffer, len, pir);
            if (wifi.send(mux_id, buffer, GetBufferLen(buffer)))
            {
                Serial.print("sent back ");
                PrintBuffer(buffer, len);
            }
            else
            {
                Serial.print("send back err");
            }

            if (wifi.releaseTCP(mux_id))
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

void loop()
{
    int pir = digitalRead(PIR);
    digitalWrite(LED, pir);
    TCPServerLoop(pir);
}
