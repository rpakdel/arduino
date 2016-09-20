
#include "pulse_sensor.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Time.h>
#include "RTClib.h"
#include "pulse_sensor.h"
#include "heartbeat.h"


// D3 is connected to ESP-01 RX pin (7)
// D2 is connected to ESP-01 TX pin (2)
SoftwareSerial softSerial(2, 3); // RX, TX

#define WIFI_SERIAL Serial
#define DEBUG_SERIAL softSerial

// only send once per seconds
#define SEND_HEARTBEAT_DELAY 5000

const size_t heartbeatSize = sizeof(Heartbeat);
unsigned long heartbeat_index = 0;

void sendHeartbeatToWifiSerial(Heartbeat& h)
{
    byte heartbeatBytes[heartbeatSize];
    memcpy(heartbeatBytes, &h, heartbeatSize);
    size_t numBytes = WIFI_SERIAL.write(heartbeatBytes, heartbeatSize);
    DEBUG_SERIAL.print(F("PULSE "));
    printlnHeartbeat(h, DEBUG_SERIAL);
}

unsigned long lastMillis = 0;

RTC_DS3231 rtc;

unsigned long getTimeUsingRTC()
{
    DateTime t = rtc.now();
    return t.unixtime();
}

void printlnDateTime(DateTime t)
{
    DEBUG_SERIAL.print(t.year());
    DEBUG_SERIAL.print("/");
    DEBUG_SERIAL.print(t.month());
    DEBUG_SERIAL.print("/");
    DEBUG_SERIAL.print(t.day());

    DEBUG_SERIAL.print(" ");

    DEBUG_SERIAL.print(t.hour());
    DEBUG_SERIAL.print(":");
    DEBUG_SERIAL.print(t.minute());
    DEBUG_SERIAL.print(":");
    DEBUG_SERIAL.print(t.second());

    DEBUG_SERIAL.println();
}

void heartbeat_callback(byte bpm)
{
    unsigned long m = millis();
    if ((m - lastMillis) >= SEND_HEARTBEAT_DELAY)
    {
        lastMillis = m;
        Heartbeat h = { getTimeUsingRTC(), bpm };
        heartbeat_index++;
        sendHeartbeatToWifiSerial(h);
    }
}

void setRTCTimeToBuildTime()
{
    DEBUG_SERIAL.println(F("Setting RTC time to last build time."));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void RTCSetup()
{
    if (!rtc.begin()) 
    {
        DEBUG_SERIAL.println(F("Couldn't find RTC"));
        return;
    }

    //setRTCTimeToBuildTime();

    DEBUG_SERIAL.print(F("PULSE "));
    printlnDateTime(rtc.now());
}

void setup() 
{
    DEBUG_SERIAL.begin(115200);
    WIFI_SERIAL.begin(57600);
    interruptSetup8Mhz();
    RTCSetup();
    DEBUG_SERIAL.println(F("PULSE ready."));
}



void loop() 
{
    if (QS) 
    {
        QS = false;
        heartbeat_callback(BPM);
    }
    while (WIFI_SERIAL.available())
    {
        String s = WIFI_SERIAL.readString();
        if (s.length() > 0)
        {
            DEBUG_SERIAL.print(F("ESP: "));
            DEBUG_SERIAL.println(s);
        }
    }
}
