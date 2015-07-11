/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#define LNG 9
#define LAT 10
#define ANG 1
#define DLA 2

#include <Servo.h> 
 
Servo lngServo;  // create servo object to control a servo 
Servo latServo;
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  lngServo.attach(LNG);  // attaches the servo on pin 9 to the servo object 
  lngServo.write(45);
  latServo.attach(LAT);
  latServo.write(90);
} 
 
void loop() 
{ 
  
  for(pos = 0; pos <= 180; pos += ANG) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    lngServo.write(pos);              // tell servo to go to position in variable 'pos' 
  
    delay(DLA);                       // waits 15ms for the servo to reach the position 
  } 
  delay(100);
  
  for(pos = 0; pos <= 180; pos += ANG) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    latServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(DLA);                       // waits 15ms for the servo to reach the position 
  }

  delay(100);  
  for(pos = 180; pos>=0; pos-=ANG)     // goes from 180 degrees to 0 degrees 
  {                                
    lngServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(DLA);                       // waits 15ms for the servo to reach the position 
  } 
  
   
  delay(100);
  for(pos = 180; pos>=0; pos-=ANG)     // goes from 180 degrees to 0 degrees 
  {                                
    latServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(DLA);                       // waits 15ms for the servo to reach the position 
  } 
  delay(100);
} 

