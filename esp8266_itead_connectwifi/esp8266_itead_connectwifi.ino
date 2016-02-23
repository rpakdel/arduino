#include <Wire.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h> 

#include <SimpleTimer.h>

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_SoftSer.h>
#include <BlynkSimpleShieldEsp8266_SoftSer.h>

SimpleTimer timer;

// Set ESP8266 Serial object
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

ESP8266 wifi(EspSerial);

#define SSID        "Area51"
#define PASSWORD    "bullseye"


RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

char auth[] = "3cbb9420b7cf4b41a639ba379afc7264";
char buffer[32];

bool isReceiverAck = false;
WidgetLED receiverLED(2);

bool turbo = false;

BLYNK_WRITE(V0)
{
	int v = param.asInt();
	turbo = v == 1;
	Serial.println(turbo);
}

BLYNK_WRITE(V1)
{
  //BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use: 
  int frontBack = param[1].asInt();
  int leftRight = param[0].asInt();
  int scale = 16;
  if (turbo)
  {
	  scale = 8;
  }
  sprintf(buffer, "%d,%d\0", frontBack / scale, leftRight / 8);

  //Serial.println(buffer);
  isReceiverAck = radio.write(buffer, strlen(buffer));
}

void setup(void)
{
    Serial.begin(9600);
    Serial.println(F("Setup"));    
    
    EspSerial.begin(9600);
    
	radio.begin();
	radio.setRetries(1, 15);
	radio.setPALevel(RF24_PA_HIGH);

	radio.openWritingPipe(addresses[1]);
	radio.stopListening();
        
	Blynk.begin(auth, wifi, SSID, PASSWORD);
  while (Blynk.connect() == false) {
    // Wait until connected
  }
  timer.setInterval(1000L, receiverLEDStatus);
  Serial.println(F("Ready"));
}

void receiverLEDStatus()
{
  if (isReceiverAck)
  {
    receiverLED.on();
  }
  else
  {
    receiverLED.off();
  }
}

bool first = true;
void loop(void)
{
  Blynk.run();  
}

