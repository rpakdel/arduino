#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <TimerOne.h>
#include <ColorRecognition.h>
#include <ColorRecognitionTCS230.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
ColorRecognitionTCS230* tcs230;

void setup() {
     Serial.begin(9600);  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    
    display.display();
    display.clearDisplay();

    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("White balance..."));
    display.display();
    
    

  
  tcs230 = ColorRecognitionTCS230::getInstance();
  tcs230->initialize(3, 4, 5);
  tcs230->setFilter(ColorRecognitionTCS230::CLEAR_FILTER);
  
  display.setCursor(0, 0);
  Serial.println(F("Adjusting the white balance..."));
  
  // Show something white to it during 4 seconds.
  tcs230->adjustWhiteBalance();
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
}

void loop() 
{
     //Serial.print("Red: ");
    int red = tcs230->getRed();
    Serial.print(red);
    Serial.print(F(","));
   //Serial.print(F("Green: "));
    int green = tcs230->getGreen();
    Serial.print(green);
    //Serial.print("Blue ");
    Serial.print(F(","));
    int blue = tcs230->getBlue();
    Serial.println(blue);
    display.clearDisplay();
    display.setCursor(0, 16);
    display.println(red);
    display.println(green);
    display.println(blue);
    display.display();
    delay(1000);
}
