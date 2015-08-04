
#include <math.h>

#include "Wire.h"
#include "WiiChuck.h"
#include <Servo.h> 

#define MAXANGLE 90
#define MINANGLE -90

#define LNG 9
#define LAT 10

Servo lngServo;  // create servo object to control a servo 
Servo latServo;

WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup() {
  
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  
    lngServo.attach(LNG);  // attaches the servo on pin 9 to the servo object 
      latServo.attach(LAT);
}


void loop() {
  delay(20);
  chuck.update(); 

  int roll = chuck.readRoll();
  lngServo.write(90 - roll);
  
  Serial.print(roll);
    Serial.print(", ");
    
    int pitch = chuck.readPitch(); 
    latServo.write(180 - pitch); 
  Serial.print(pitch);
    Serial.print(", ");  

    Serial.print((int)chuck.readAccelX()); 
    Serial.print(", ");  
    Serial.print((int)chuck.readAccelY()); 
    Serial.print(", ");  

    Serial.print((int)chuck.readAccelZ()); 

    Serial.println();
   delay(10);
}

