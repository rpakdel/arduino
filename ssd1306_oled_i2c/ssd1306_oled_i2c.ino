#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() 
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    
    display.display();
    delay(5000);
    display.clearDisplay();

    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    display.print("0x"); 
    display.println(0xDEADBEEF, HEX);
    display.display();
}

void loop() 
{
  // put your main code here, to run repeatedly:

}
