/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "gpsdata.h"
#include "joydata.h"

const char* ssid = "";
const char* password = "";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

String newLine = "\r\n";

void setup() 
{
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void handleGetLoc(WiFiClient& client)
{
  client.flush();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  
  JsonObject& gps = root.createNestedObject("gps");
  gps["bid"] = 0;
  gps["lng"] = -43;
  gps["lat"] = 128;  
  
  // Prepare the response
  String s = "HTTP/1.1 200 OK";
  s += newLine;
  s += "Content-Type: application/json";
  s += newLine;
  s += newLine;    
  root.printTo(s);

  // Send the response to the client
  client.print(s);
  
  delay(1);
}

void handlePutJoy(WiFiClient& client)
{
	bool currentLineIsBlank = false;
	while (client.available())
	{		
		char c = client.read();
		if (c == '\n' && currentLineIsBlank)
		{
			// finished reading a blank line
			// so message body starts here
			String messageBody = client.readStringUntil('\r');			
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(messageBody);
			// could not parse message body
			if (!root.success())
			{
				String res = "HTTP/1.1 400 Bad Request";
				client.flush();
				client.println(res);
			}
			else
			{
				// parsed json data
				JoyData joyData;
				joyData.bid = root["bid"];
				joyData.X = root["x"];
				joyData.Y = root["y"];
				printlnJoyData(joyData, Serial);				
				
				String res = "HTTP/1.1 200 OK";
				client.flush();
				client.println(res);
			}
		}
		else if (c == '\n')
		{
			// assume current line is blank unless we read 
			// a character other than \r
			currentLineIsBlank = true;
		}
		else if (c != '\r')
		{
			// current line is not blank
			currentLineIsBlank = false;			
		}
	}
}

void handleClient(WiFiClient& client)
{
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.read(); // \r
  client.read(); // \n
  Serial.print("[");
  Serial.print(req);
  Serial.print("]");
  Serial.println();
  
  // Match the request  
  if (req.indexOf("/api/v1/loc") != -1)
  {
    handleGetLoc(client);  
  }
  else if (req.indexOf("/api/v1/joy") != -1)
  {
    handlePutJoy(client);
  }
  else
  {
    Serial.println("Unknown request");
    client.stop();    
  }
}

void loop()
{
	// Check if a client has connected
	WiFiClient client = server.available();
	if (!client)
	{
		return;
	}

	// Wait until the client sends some data
	Serial.println("------New Client-------");
	Serial.print("New client, waiting ...");
	while (!client.available())
	{
		delay(1);
	}

	Serial.println(" got data.");
	handleClient(client);
	
	Serial.println("---Client disonnected---");

	// The client will actually be disconnected 
	// when the function returns and 'client' object is detroyed
}

