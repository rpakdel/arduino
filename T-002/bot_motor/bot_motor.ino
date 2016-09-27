#include <Wire.h>

#include "C:/Users/User/Dropbox/git/Arduino/T-002/shared/joydata.h"

#define BOT_I2C_ADDRESS 0x8

JoyData joyData;

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


void setup()
{
    Serial.begin(115200);
    Wire.begin(BOT_I2C_ADDRESS);
    Wire.onReceive(receiveEvent);
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

float speedFactor = 1.25;

void toggleSpeedFactor()
{
    speedFactor += 0.5;
    if (speedFactor > 2)
    {
        speedFactor = 1;
    }
}

void moveBot(JoyData& joyData)
{
    // http://home.kendra.com/mauser/Joystick.html
    float v = (255 - abs(joyData.X)) * (joyData.Y / 255.0) + joyData.Y;
    float w = (255 - abs(joyData.Y)) * (joyData.X / 255.0) + joyData.X;

    // right motor
    float denom = 2.0 * speedFactor;
    int motor1Speed = (v - w) / denom;
    int motor2Speed = (v + w) / denom;

    motorMove(1, motor1Speed);
    motorMove(2, motor2Speed);
}

byte joyDataBytes[joyDataSize];

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
    while (Wire.available())
    {
        size_t numBytes = Wire.readBytes(joyDataBytes, joyDataSize);
        if (numBytes != joyDataSize)
        {
            Serial.println("Incorrect data size");
        }
        JoyData joyData;
        memcpy(&joyData, joyDataBytes, joyDataSize);
        //printlnJoyData(joyData, Serial);
        if (joyData.Button > 127)
        {
            toggleSpeedFactor();
        }
        moveBot(joyData);
    }
}

void loop()
{
    
}
