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
#include <printf.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "joydata.h"
#include "gpsdata.h"

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

JoyData joyData;
GpsData gpsData;

// Front of robot is where the motor driver heat sink is
// Motor 1 is on the right side when viewed from the front. 
// Motor 1 is left motor when looking at the motors from the back
// Motor 1 connected to OUT1 and OUT2 of L298H motor driver
#define dir1PinA 8
#define dir2PinA 7
// needs to be PWM to control motor speed
#define speedPinA 6


// Motor 2 is on the left side when viewed from the front (heat sink side)
// Motor 2 is on the right when looking at the motor from the back
// Motor 2 is connected to OUT3 and OUT4 of L298H motor driver
#define dir1PinB 3
#define dir2PinB 4
 // Needs to be a PWM pin to be able to control motor speed
#define speedPinB 5

#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

#define GPS_RX 2
#define GPS_TX -1
#define GPS_BAUD 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus tinyGPS;

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

bool GetLatLngAlt(TinyGPSPlus &tiny, char* buffer, int len)
{
    if (!tiny.location.isValid())
    {
        return false;
    }

    //Serial.println(F("Got location"));
    char lat[16];
    dtostrf(tiny.location.lat(), 9, 6, lat);
    char lng[16];
    dtostrf(tiny.location.lng(), 10, 6, lng);
    char alt[16];
    dtostrf(tiny.altitude.meters(), 6, 2, alt);
    snprintf(
        buffer,
        len,
        "%s,%s,%s",
        lat, lng, alt);
    return true;
}

void setup()
{
  initDisplay();

  gpsData.valid = false;
  gpsSerial.begin(GPS_BAUD);

  display.print(F("T-002: Init"));
  
    pinMode(dir1PinA, OUTPUT);
    pinMode(dir2PinA, OUTPUT);
    pinMode(speedPinA, OUTPUT);
    pinMode(dir1PinB, OUTPUT);
    pinMode(dir2PinB, OUTPUT);
    pinMode(speedPinB, OUTPUT);

    Serial.begin(115200);
    //Serial.println(F("Receiver"));
    

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

    display.setCursor(0, 0);
    display.print(F("T-002: Ready"));
}

void motorMove(int mindex, int mspeed)
{
  // setup pins based on motor being moved
    int speedPin = speedPinA;
    int dir1pin = dir1PinA;
    int dir2pin = dir2PinA;

    if (mindex == 1)
    {
        speedPin = speedPinB;
        dir1pin = dir1PinB;
        dir2pin = dir2PinB;

   display.setCursor(0, 2);
   display.clearToEOL();
   display.print("Motor 2: ");
   display.print(mspeed);   
    }
  else
  {
    display.setCursor(0, 1);
   display.clearToEOL();
   display.print("Motor 1: ");
   display.print(mspeed);   
  }

    if (mspeed >= 0)
    {
        // forward
        digitalWrite(dir1pin, LOW);
        digitalWrite(dir2pin, HIGH);

        analogWrite(speedPin, mspeed);
    }
    else
    {
        digitalWrite(dir1pin, HIGH);
        digitalWrite(dir2pin, LOW);

        analogWrite(speedPin, -mspeed);
    }	
}

void move(JoyData& joyData)
{
    // http://home.kendra.com/mauser/Joystick.html
    float v = (255 - abs(joyData.X)) * (joyData.Y / 255.0) + joyData.Y;
    float w = (255 - abs(joyData.Y)) * (joyData.X / 255.0) + joyData.X;

    // right motor
    int motor1Speed = (v - w) / 2.0;
    int motor2Speed = (v + w) / 2.0;

    motorMove(1, motor1Speed);
    motorMove(2, motor2Speed);
}

void displayGpsData(GpsData& data)
{
    display.setCursor(0, 3);
    display.clearToEOL();
    display.print("lat: ");
    if (data.valid)
    {
        display.print(data.lat);
    }
    else
    {
        display.print("inv");
    }

    display.setCursor(0, 4);
    display.clearToEOL();
    display.print("lng: ");
    if (data.valid)
    {
        display.print(data.lon);
    }
    else
    {
        display.print("inv");
    }
}

void loop()
{
    
    byte pipeNo;
    if (radio.available(&pipeNo))
    {
        radio.read(&joyData, sizeof(JoyData));
        // respond with gps data
        radio.writeAckPayload(pipeNo, &gpsData, sizeof(gpsData));
        if (gpsData.valid)
        {
            //Serial.print(F("Pipe "));
            //Serial.print(pipeNo);
            //Serial.print(F(", Sending "));
            //Serial.print(gpsData.lon);
            //Serial.print(F(", "));
            //Serial.println(gpsData.lat);            
        }
        // Spew it
        printlnJoyData(joyData, Serial);

        // clamp the values to [-255, 255]
        clampJoyData(joyData, -255, 255);
        // zero small values
        zeroDeadZoneJoyData(joyData, 20);

        printlnJoyData(joyData, Serial);
        
        move(joyData);
    }
    else
    {
        //Serial.println(F("NO DATA"));
    }

    while (gpsSerial.available() > 0)
    {
        int g = gpsSerial.read();
        if (tinyGPS.encode(g))
        {
            gpsData.valid = true;
            gpsData.lat = tinyGPS.location.lat();
            gpsData.lon = tinyGPS.location.lng();
            displayGpsData(gpsData);
        }
    }
}
