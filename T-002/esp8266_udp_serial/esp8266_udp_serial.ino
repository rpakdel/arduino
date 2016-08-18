#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
const char* ssid = "Area51";
const char* password = "bullseye";

unsigned int localPort = 6969;      // local port to listen on
IPAddress remoteIP;
uint16_t remotePort;
bool hasConnection;

#define PACKET_MAX_SIZE 255
char packetBuffer[PACKET_MAX_SIZE]; //buffer to hold incoming packet

WiFiUDP Udp;

void sendSetupToSerial()
{
    StaticJsonBuffer<255> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["ssid"] = ssid;
    root["ip"] = WiFi.localIP().toString();
    root["port"] = localPort;

    root.printTo(Serial);
    Serial.println();
}

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(10);
 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      //Serial.print(".");
  }
  Serial.println();
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  sendSetupToSerial();
}

int serialCharCount = 0;
char serialData[PACKET_MAX_SIZE];

void sendFromSerialToUdp()
{
    while (Serial.available())
    {
        char c = Serial.read();        
        if (c == '\n' || serialCharCount == PACKET_MAX_SIZE - 1)
        {
            serialData[serialCharCount] = 0;

            if (hasConnection)
            {
                Serial.print("sending ");
                Serial.print(serialData);
                Serial.print(" to ");
                Serial.print(remoteIP.toString());
                Serial.print(":");
                Serial.println(remotePort);

                Udp.beginPacket(remoteIP, remotePort);
                Udp.write(serialData, serialCharCount);
                Udp.endPacket();
            }

            serialCharCount = 0;
        }
        else
        {
            serialData[serialCharCount] = c;
            serialCharCount++;
        }
    }
}

void sendErrorToSerial(const char* errMsg)
{
    StaticJsonBuffer<255> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["err"] = errMsg;
    root.printTo(Serial);
}

void sendFromUdpToSerial()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        remoteIP = Udp.remoteIP();
        remotePort = Udp.remotePort();
        hasConnection = true;

        if (packetSize > PACKET_MAX_SIZE)
        {
            sendErrorToSerial("UDP packet is too big");
            return;
        }

        // read the packet into packetBufffer
        int len = Udp.read(packetBuffer, PACKET_MAX_SIZE);
        if (len > 0)
        {
            packetBuffer[len] = 0;
            Serial.println(packetBuffer);
        }
    }
}

void loop()
{
    sendFromSerialToUdp();
    sendFromUdpToSerial();
    delay(1);
}
