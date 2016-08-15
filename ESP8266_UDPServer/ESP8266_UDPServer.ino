#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "joydata.h"
#include "gpsdata.h"

int status = WL_IDLE_STATUS;
const char* ssid = "Area51";
const char* password = "bullseye";

unsigned int localPort = 80;      // local port to listen on

#define PACKET_MAX_SIZE 255
char packetBuffer[PACKET_MAX_SIZE]; //buffer to hold incoming packet

WiFiUDP Udp;

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(10);
  

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
	  delay(500);
	  Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");  
  Serial.println(WiFi.localIP());

  Serial.println("\nStarting UDP server.");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

GpsData gpsData;

void getGpsData()
{
	gpsData.valid = 1;
	gpsData.bid = 0;
	gpsData.lat = 49;
	gpsData.lon = -128;
}

void handleLoc()
{
	char buffer[PACKET_MAX_SIZE];
	// send a reply, to the IP address and port that sent us the packet we received
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	size_t count = jsonSerializeGpsData(gpsData, buffer, PACKET_MAX_SIZE);	
	buffer[count] = 0;
	Serial.println(buffer);
	Udp.write(buffer);
	Udp.endPacket();
}

void handleJoy(char* packetBuffer)
{
	if (strlen(packetBuffer) == 1)
	{
		return;
	}

	JoyData joyData;
	if (jsonDeserializeJoyData(&packetBuffer[1], joyData))
	{
		printlnJoyData(joyData, Serial);
	}
	else
	{
		Serial.println("Could not parse joy data");
	}
}

void loop()
{
	getGpsData();
	// if there's data available, read a packet
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		if (packetSize > PACKET_MAX_SIZE)
		{
			Serial.print("packet is too big (");
			Serial.print(packetSize);
			Serial.println(")");
			return;
		}

		//Serial.print("Received packet of size ");
		//Serial.println(packetSize);
		//Serial.print("From ");
		//IPAddress remoteIp = Udp.remoteIP();
		//Serial.print(remoteIp);
		//Serial.print(", port ");
		//Serial.println(Udp.remotePort());

		// read the packet into packetBufffer
		int len = Udp.read(packetBuffer, PACKET_MAX_SIZE);
		if (len > 0)
		{
			packetBuffer[len] = 0;
		}
		//Serial.println("Contents:");
		//Serial.println(packetBuffer);

		if (packetBuffer[0] == 'j')
		{
			handleJoy(packetBuffer);
		}
		else if (packetBuffer[0] == 'l')
		{
			handleLoc();
		}
		else
		{
			Serial.println("Unknown request");
			Serial.println(packetBuffer);
		}
	}
}
