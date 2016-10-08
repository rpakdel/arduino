
#include "pulse_sensor.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Time.h>
#include "RTClib.h"
#include "pulse_sensor.h"
#include "heartbeat.h"
#include <../../ESP8266_heartrate_sensor_ard_i2c/shared/rtc.h>

#define DEBUG_SERIAL Serial
#define ARD_I2C_ADDRESS 0x8

unsigned long startTime = 0;

unsigned long lastMillis = 0;

Heartbeat lastHeartbeat = { -1, 0 };

void heartbeat_callback(byte bpm, Print& print)
{
    lastHeartbeat = { getCurrentTime(startTime), bpm };
    //print.print(F("PULSE "));
    //printlnHeartbeat(lastHeartbeat, print);
}

void printBytes(byte bytes[], size_t size, Print& print)
{
    print.print(size);
    print.print(F(" bytes: "));
    for (int i = 0; i < size; ++i)
    {
        print.print(bytes[i], BIN);
        print.print(" ");
    }
}

void i2c_onRequest()
{
    //DEBUG_SERIAL.println("Request receive");
    //DEBUG_SERIAL.print("Sending ");
    //printlnHeartbeat(lastHeartbeat, DEBUG_SERIAL);
    byte heartbeatBytes[heartbeatSize];
    memcpy(heartbeatBytes, &lastHeartbeat, heartbeatSize);
    //printBytes(heartbeatBytes, heartbeatSize, DEBUG_SERIAL);
    //DEBUG_SERIAL.println();
    Wire.write(&heartbeatBytes[0], heartbeatSize);
    //for (int i = 0; i < heartbeatSize; ++i)
    //{
    //    Wire.write(&heartbeatBytes[i], 1);
    //}
}

void setup() 
{
    Wire.begin(ARD_I2C_ADDRESS); // master with this address
    //Wire.setClock(400000L);
    Wire.onRequest(i2c_onRequest);
    DEBUG_SERIAL.begin(115200);
    interruptSetup8Mhz();
    startTime = RTCSetup(DEBUG_SERIAL);
    DEBUG_SERIAL.println(F("PULSE ready."));
}

void loop() 
{
    if (QS) 
    {
        QS = false;
        heartbeat_callback(BPM, DEBUG_SERIAL);
    }
}
