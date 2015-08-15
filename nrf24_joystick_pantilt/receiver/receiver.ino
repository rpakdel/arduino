/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Servo.h> 
#include <string.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>

#include "RF24.h"

#define SERVO_X_PIN 4
#define SERVO_Y_PIN 3

Servo xServo;
Servo yServo;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

char buffer[33];

void setup()
{
    xServo.attach(SERVO_X_PIN);
    yServo.attach(SERVO_Y_PIN);

    buffer[0] = '\0';
    Serial.begin(115200);
    Serial.println(F("Receiver"));

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("RECEIVER"));
    display.display();

    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);

    radio.openReadingPipe(0, addresses[1]);

    radio.startListening();
}

#define NUM_SAMPLES 1
int xs[NUM_SAMPLES];
int ys[NUM_SAMPLES];
int sampleIndex = 0;

void loop()
{
    if (radio.available())
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("RECEIVER*"));

        radio.read(&buffer, 32);

        display.setCursor(0, 16);
        //display.print(buffer);
        
        // Spew it
        Serial.print(F("Got: "));
        Serial.println(buffer);

        // get x
        char *ch = strtok(buffer, ",");
        int x = atoi(ch);
        x = map(x, 0, 1023, 0, 180);
        xs[sampleIndex] = x;
        

        // get y
        ch = strtok(NULL, ",");
        int y = atoi(ch);
        y = map(y, 0, 1023, 0, 180);
        ys[sampleIndex] = y;

        // get switch
        ch = strtok(NULL, ",");
        int s = atoi(ch);

        sampleIndex++;
        if (sampleIndex >= NUM_SAMPLES)
        {
            sampleIndex = 0;
        }
        x = 0;
        y = 0;
        for (int i = 0; i < NUM_SAMPLES; ++i)
        {
            x += xs[i];
            y += ys[i];
        }
        x = x / NUM_SAMPLES;
        y = y / NUM_SAMPLES;
        xServo.write(x);
        yServo.write(y);
        display.println(x);
        display.println(y);
        display.display();
        
        //display.print(s);

        //display.display();
    }
    else
    {
        //display.setCursor(0, 0);
        //display.println(F("RECEIVER"));
        //display.setCursor(0, 16);
        //display.print(buffer);
        //Serial.println(F("NO DATA"));
    }
    

    //Serial.println(F("Waiting"));
    // Try again 1s later
    delay(10);
    
}