/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example runs directly on ESP8266 chip.
 *
 * You need to install this for ESP8266 development:
 *   https://github.com/esp8266/Arduino
 * 
 * Please be sure to select hte right ESP8266 module
 * in the Tools -> Board menu!
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "3cbb9420b7cf4b41a639ba379afc7264";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "Area51", "bullseye");
}

int frontBack = 0;
int leftRight = 0;

BLYNK_WRITE(V1)
{
  //BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use: 
  frontBack = param[1].asInt();
  leftRight = param[0].asInt();
  Serial.print(frontBack);
  Serial.print(",");
  Serial.println(leftRight);
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, frontBack);
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3, leftRight);
}

void loop()
{
  Blynk.run();
}

