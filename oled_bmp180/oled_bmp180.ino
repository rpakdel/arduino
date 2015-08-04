#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

int getX(int numChars, int textSize)
{
    return (6 * textSize * numChars);
}

int getXRightAlign(int numChars, int textSize)
{
    return 128 - getX(numChars, textSize);
}

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Pressure Sensor Test"); Serial.println("");

    if (!bmp.begin())
    {
        Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
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

void displayPressure(float pressure)
{
    display.setCursor(0, 0);
    display.setTextSize(2);
    
    display.print(pressure);
    display.print(" hPa");
}

void displayTemp(float temperature)
{
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.print(temperature);
    display.setCursor(116, 16);
    display.print("C");
}

void displayAlt(float alt)
{
    display.setTextSize(2);
    display.setCursor(0, 32);
    display.print(alt);
    display.setCursor(116, 32);
    display.print("m");
}

void loop(void)
{
    sensors_event_t event;
    bmp.getEvent(&event);

    if (event.pressure)
    {
        display.clearDisplay();

        displayPressure(event.pressure);

        float temperature;
        bmp.getTemperature(&temperature);
        displayTemp(temperature);

        float seaLevelPressure = 1020;
        float alt = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
        displayAlt(alt);
        

        display.display();
    }
    else
    {
        Serial.println("Sensor error");
    }
}