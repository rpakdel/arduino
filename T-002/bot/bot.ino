/*
* Motor B (right when looking from front)
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <SPI.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "../../T-002/shared/joydata.h"
#include "../../T-002/shared/gpsdata.h"

#define DEBUG_SERIAL Serial

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

#define MOTOR_RX -1 // purple
#define MOTOR_TX 4 // grey
SoftwareSerial motorSerial(MOTOR_RX, MOTOR_TX);

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

#define GPS_RX 2
#define GPS_TX -1
#define GPS_BAUD 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus tinyGPS;

static GpsData gpsData;

void initDisplay()
{
    //Serial.println(F("Init OLED"));
    Wire.begin();
    display.begin(&Adafruit128x64, I2C_ADDRESS);
    display.set400kHz();
    display.setFont(Adafruit5x7);
    display.clear();
    display.setCursor(0, 0);
    //Serial.println(F("OLED ready"));
}

void initRadio()
{
    radio.begin();
    radio.setAutoAck(1);
    radio.enableAckPayload();
    radio.setPayloadSize(sizeof(GpsData));
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(0, addresses[0]);
    radio.startListening();
}

void initGPS()
{
    gpsData.IsValid = false;
    gpsSerial.begin(GPS_BAUD);
}

void initMotor()
{
    motorSerial.begin(115200);
}

void setup()
{
    DEBUG_SERIAL.begin(115200);

    DEBUG_SERIAL.print(F("Size of joystick data (bytes): "));
    DEBUG_SERIAL.println(joyDataSize);

    DEBUG_SERIAL.print(F("Size of GPS data (bytes): "));
    DEBUG_SERIAL.println(gpsDataSize);

    initDisplay();
    display.print(F("T-002: Init"));

    initRadio();
    initMotor();
    initGPS();
    
    // ensure the gps serial port is listening
    gpsSerial.listen();

    display.setCursor(0, 0);
    display.clearToEOL();
    display.print(F("T-002: Ready"));
}

byte displayJoyData(JoyData& joyData, byte rowIndex)
{
    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print("X: ");
    display.print(joyData.X);
    rowIndex++;

    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print("Y: ");
    display.print(joyData.Y);
    rowIndex++;

    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print("B: ");
    display.print(joyData.Button);
    rowIndex++;

    return rowIndex;
}

static byte joyDataBytes[joyDataSize];

void sendJoyDataToMotor(JoyData& joyData)
{
    memcpy(&joyDataBytes[0], &joyData, joyDataSize);
    motorSerial.write(&joyDataBytes[0], joyDataSize);
}

byte displayGpsData(GpsData& data, byte rowIndex)
{
    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print("Lat: ");
    if (data.IsValid)
    {
        display.print(data.Lat);
    }
    else
    {
        display.print("Inv");
    }
    rowIndex++;

    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print(F("Lon: "));
    if (data.IsValid)
    {
        display.print(data.Lon);
    }
    else
    {
        display.print("Inv");
    }
    rowIndex++;

    display.setCursor(0, rowIndex);
    display.clearToEOL();
    display.print(F("Alt: "));
    if (data.IsValid)
    {
        display.print(data.Alt);
    }
    else
    {
        display.print("Inv");
    }
    rowIndex++;

    return rowIndex;
}

void loop()
{
    byte pipeNo = 0;
    if (radio.available(&pipeNo))
    {
        JoyData joyData;
        radio.read(&joyData, sizeof(JoyData));
        // respond with gps data
        radio.writeAckPayload(pipeNo, &gpsData, gpsDataSize);

        // clamp the values to [-255, 255]
        clampJoyDataXY(joyData, -255, 255);
        // zero small values
        zeroDeadZoneJoyData(joyData, 20, 10);

        displayJoyData(joyData, 2);

        sendJoyDataToMotor(joyData);
    }

    while (gpsSerial.available() > 0)
    {
        int g = gpsSerial.read();
        if (tinyGPS.encode(g))
        {
            gpsData.Id = 0;
            gpsData.IsValid = true;
            gpsData.Lat = tinyGPS.location.lat();
            gpsData.Lon = tinyGPS.location.lng();
            if (tinyGPS.altitude.isValid())
            {
                gpsData.Alt = tinyGPS.altitude.meters();
            }
            else
            {
                gpsData.Alt = 0;
            }
            displayGpsData(gpsData, 5);
        }

    }
    
}
