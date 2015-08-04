/**
* @example TCPServer.ino
* @brief The TCPServer demo of library WeeESP8266.
* @author Wu Pengfei<pengfei.wu@itead.cc>
* @date 2015.02
*
* @par Copyright:
* Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version. \n\n
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <SoftwareSerial.h>
#include "ESP8266.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define SSID        "Area51"
#define PASSWORD    "bullseye"

SoftwareSerial SoftSerial(11, 10);

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C	lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


ESP8266 wifi(SoftSerial);

void setup(void)
{
    lcd.begin(16, 2);
    lcd.home();
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    pinMode(13, OUTPUT);
    SoftSerial.begin(9600);
    lcd.println("setup begin");

    lcd.println("FW Version:");
    lcd.println(wifi.getVersion().c_str());
    delay(1000);

    if (wifi.setOprToStationSoftAP()) {
        lcd.println("to station + softap ok");
    }
    else {
        lcd.println("to station + softap err");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        lcd.println("Join AP success");
        lcd.print("IP: ");
        lcd.println(wifi.getLocalIP().c_str());
    }
    else {
        lcd.println("Join AP failure");
    }

    if (wifi.enableMUX()) {
        lcd.println("multiple ok");
    }
    else {
        lcd.println("multiple err");
    }

    if (wifi.startTCPServer(8090)) {
        lcd.println("start tcp server ok");
    }
    else {
        lcd.println("start tcp server err");
    }

    if (wifi.setTCPServerTimeout(60)) {
        lcd.println("set tcp server timout 60 seconds");
    }
    else {
        lcd.println("set tcp server timout err");
    }

    lcd.println("setup end");
}

void blink()
{
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
}

void loop(void)
{
    blink();
    uint8_t buffer[128] = { 0 };
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    if (len > 0) {
        lcd.print("Status:[");
        lcd.print(wifi.getIPStatus().c_str());
        lcd.println("]");

        lcd.print("Received from :");
        lcd.print(mux_id);
        lcd.print("[");
        for (uint32_t i = 0; i < len; i++) {
            lcd.print((char)buffer[i]);
        }
        lcd.print("]");

        if (wifi.send(mux_id, buffer, len)) {
            lcd.print("send back ok");
        }
        else {
            lcd.print("send back err");
        }

        if (wifi.releaseTCP(mux_id)) {
            lcd.print("release tcp ");
            lcd.print(mux_id);
            lcd.println(" ok");
        }
        else {
            lcd.print("release tcp");
            lcd.print(mux_id);
            lcd.println(" err");
        }

        lcd.print("Status:[");
        lcd.print(wifi.getIPStatus().c_str());
        lcd.println("]");
    }
}
