#include <Wire.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <nRF24L01.h>

#include <SoftwareSerial.h> 

#include <SimpleTimer.h>

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "gpsdata.h"
#include "joydata.h"

SimpleTimer timer;

// Set ESP8266 Serial object
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

ESP8266 wifi(&EspSerial);

//#define SSID        "Reza2"
//#define PASSWORD    "12345678"

#define SSID        "Area51"
#define PASSWORD    "bullseye"

#define ESP_BAUD 9600

RF24 radio(9, 10);
byte addresses[][6] = { "1Node", "2Node" };

char auth[] = "4dd4a25a2bbf459caaca5d62e415514b";
char buffer[32];

bool isReceiverAck = false;
WidgetLED receiverLED(2);

bool turbo = false;

void send(JoyData& data)
{
	Serial.print(F("Now sending "));
	Serial.print(data.X);
	Serial.print(F(", "));
	Serial.print(data.Y);
	Serial.print(F("..."));

	if (radio.write(&data, sizeof(data)))
	{
		Serial.println(F("ok"));
	}
	else
	{
		Serial.println(F("no ack"));
	}
}

bool read(GpsData& gpsData)
{
	if (!radio.isAckPayloadAvailable())
	{
		Serial.println(F("No GPS data"));
		return false;
	}

	radio.read(&gpsData, sizeof(gpsData));
	Serial.print(F("GPS: "));
	Serial.print(gpsData.lat);
	Serial.print(F(", "));
	Serial.println(gpsData.lon);
	return true;
}

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

  JoyData joyData;
  joyData.X = frontBack / scale;
  joyData.Y = leftRight / 8;

  send(joyData);

  GpsData gpsData;
  read(gpsData);
}

void setupRadio()
{
	radio.begin();
	radio.setAutoAck(true);
	radio.enableAckPayload();
	radio.setPayloadSize(sizeof(GpsData));
	radio.setRetries(15, 15);
	radio.setPALevel(RF24_PA_LOW);
	radio.setDataRate(RF24_2MBPS);

	radio.openWritingPipe(addresses[0]);
	radio.stopListening();
}

void setupBlynk()
{
	Blynk.begin(auth, wifi, SSID, PASSWORD);
	delay(1000);
	while (!Blynk.connect(10000))
	{
		Serial.println(F("Waiting to connect"));
		// Wait until connected
	}
	//timer.setInterval(1000L, receiverLEDStatus);
}


void setup(void)
{
	Serial.begin(9600);
	Serial.println(F("Setup"));

	EspSerial.begin(ESP_BAUD);

	setupRadio();
	setupBlynk();

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

