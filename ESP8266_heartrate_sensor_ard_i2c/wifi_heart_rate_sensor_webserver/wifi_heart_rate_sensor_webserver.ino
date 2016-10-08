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

#include <Time.h>
#include <RTClib.h>
#include <TimeLib.h>
#include <Wire.h>

#include "heartbeat.h"

#define DEBUG_SERIAL Serial 
#define DEBUG_SERIAL_BAUD 115200

#define ARD_I2C_ADDRESS 0x8

time_t startTime = 0;

unsigned long lastHeartbeatRequest = 0;

#include "ntp.h"
#include <../../ESP8266_heartrate_sensor_ard_i2c/shared/rtc.h>

// define MYSSID and MYPASSWORD in this file
// do not check this file into public repositories like github
// ------------ SAMPLE -----------
// #pragma once
// #define MYSSID "YOURSSID"
// #define MYPASSWORD "YOURPASSWORD"
// ------------ SAMPLE -----------
#include "myssid.h"

// Create an instance of the server
// specify the port to listen on as an argument
#define WEBSERVER_PORT 80

WiFiServer server(WEBSERVER_PORT);


const char newLine[] PROGMEM = "\r\n";
const char http_status_200_OK[] PROGMEM = "HTTP/1.1 200 OK";
const char http_status_404_NOTFOUND[] PROGMEM = "HTTP/1.1 404 NOT FOUND";

const char content_type_js[] PROGMEM = "Content-Type: application/javascript";
const char content_type_css[] PROGMEM = "Content-Type: text/css";
const char content_type_html[] PROGMEM = "Content-Type: text/html";
const char content_type_json[] PROGMEM = "Content-Type: application/json";


// must disable visual micro deep search for this to work
const char index_html_FileContent[] PROGMEM =
#include "..\..\ESP8266_heartrate_sensor_ard\wifi_heart_rate_sensor_webserver\index.html"
;

const char client_js_FileContent[] PROGMEM =
#include "..\..\ESP8266_heartrate_sensor_ard\wifi_heart_rate_sensor_webserver\client.js"
;

const char style_css_FileContent[] PROGMEM =
#include "..\..\ESP8266_heartrate_sensor_ard\wifi_heart_rate_sensor_webserver\style.css"
;



#define MAX_HEARTBEATS 100

Heartbeat heartbeats[MAX_HEARTBEATS];
unsigned int heartbeat_index = 0;

void addHeartbeat(Heartbeat h)
{
    if (h.bpm == 0)
    {
        return;
    }

    if (h.time == 0)
    {
        h.time = getCurrentTime(startTime);
    }

    // if the time hasn't changed, just replace the bpm
    if (heartbeat_index >= 1)
    {
        unsigned int lastIndex = heartbeat_index - 1;
        if (heartbeats[lastIndex].time == h.time)
        {
            heartbeats[lastIndex].bpm = h.bpm;
            return;
        }
    }

    heartbeats[heartbeat_index] = h;

    heartbeat_index++;
    if (heartbeat_index >= MAX_HEARTBEATS)
    {
        heartbeat_index = 0;
    }
}

void setup() 
{
    Wire.begin(0, 2); // master only
    // wait a bit if request is not responded to fast enough
    Wire.setClockStretchLimit(15000);
    Wire.setClock(400000L);

  DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD);
  delay(10);

  
  
  // Connect to WiFi network
  //DEBUG_SERIAL.print(F("Connecting to "));
  DEBUG_SERIAL.println(MYSSID);
  
  WiFi.begin(MYSSID, MYPASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    DEBUG_SERIAL.print(F("."));
  }
  
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println(F("WiFi connected"));
  
  startTime = RTCSetup(DEBUG_SERIAL);

  // Start the server
  server.begin();
  DEBUG_SERIAL.println(F("Server started"));

  // Print the IP address
  DEBUG_SERIAL.print(WiFi.localIP());
  DEBUG_SERIAL.print(F(":"));
  DEBUG_SERIAL.println(WEBSERVER_PORT);
}

void handleGetLoc(WiFiClient& client)
{
    /*
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
  */
}

void handlePutJoy(WiFiClient& client)
{
    /*
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
    */
}

void get_index_html(WiFiClient& client)
{
    // Note: don't pass FPSTR, too slow

    // HTTP header
    client.println(String(FPSTR(http_status_200_OK)));    
    client.println(String(FPSTR(content_type_html)));

    client.println(); 

    // HTTP body
    client.print(String(FPSTR(index_html_FileContent)));
}

void get_client_js(WiFiClient& client)
{
    // Note: don't pass FPSTR, too slow

    // HTTP header
    client.println(String(FPSTR(http_status_200_OK)));
    client.println(String(FPSTR(content_type_js)));
    
    client.println();

    // HTTP body
    client.println(String(FPSTR(client_js_FileContent)));
}

void get_style_css(WiFiClient& client)
{
    // Note: don't pass FPSTR, too slow

    // HTTP header
    client.println(String(FPSTR(http_status_200_OK)));
    client.println(String(FPSTR(content_type_css)));

    client.println();
    
    // HTTP body
    client.println(String(FPSTR(style_css_FileContent)));
}


void get_time(WiFiClient& client)
{
    time_t currentSeconds = getCurrentTime(startTime);

    // Note: don't pass FPSTR, too slow

    // HTTP header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    
    client.println();

    // HTTP body
    client.print("{\"time\":");
    client.print(String(currentSeconds));
    client.print("}");
}

void get_bpm(WiFiClient& client, ulong fromTime)
{
    String heartbeatsJSON = heartbeatsToJSON(heartbeats, heartbeat_index, fromTime, DEBUG_SERIAL);
    
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_json);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += heartbeatsJSON;
    //DEBUG_SERIAL.println(s);
    client.print(s);
}

void get_favicon(WiFiClient& client)
{
    String s = FPSTR(http_status_404_NOTFOUND);
    //DEBUG_SERIAL.println(s);
    client.print(s);
};

void route_client(WiFiClient& client, Print& print)
{
    //print.print(F("Client: "));
    //print.print(client.remoteIP().toString());
    //print.print(F(":"));
    //print.print(client.remotePort());

    // Read the first line of the request
    String req = client.readStringUntil('\r');
    client.read(); // \r
    client.read(); // \n
    //DEBUG_SERIAL.print(F("["));
    //DEBUG_SERIAL.print(req);
    //DEBUG_SERIAL.print(F("]"));
    //DEBUG_SERIAL.println();

    // Match the request  

    if (req.indexOf(F("GET / ")) >= 0) // space after / to make sure it's root
    {
        print.print(F(" INDEX "));
        get_index_html(client);
    }
    else if (req.indexOf(F("GET /client.js")) >= 0)
    {
        print.print(F(" CLIENT.JS "));
        get_client_js(client);
    }
    else if (req.indexOf(F("GET /style.css")) >= 0)
    {
        print.print(F(" STYLE.CSS "));
        get_style_css(client);
    }
    else if (req.indexOf(F("GET /favicon.ico")) >= 0)
    {
        print.print(F(" FAVICON "));
        get_favicon(client);
    }
    else if (req.indexOf(F("GET /api/v1/time")) >= 0)
    {
        //print.print(F(" TIME "));
        get_time(client);
    }
    else if (req.indexOf(F("GET /api/v1/bpm")) >= 0)
    {
        print.print(F(" BPM "));
        // if gt is specifed, return heartbeats from time greater than
        // parameter
        int startIndex = req.indexOf(F("?gt="));
        if (startIndex >= 0)
        {
            String lastTimeValueStr;

            int length = 4; // strlen("?gt=");
            startIndex += length;
            int endIndex = req.indexOf(F("&"), startIndex);
            if (endIndex >= 0)
            {
                lastTimeValueStr = req.substring(startIndex, endIndex);                
            }
            else
            {
                lastTimeValueStr = req.substring(startIndex);
            }
            unsigned long fromTime = strtoul(lastTimeValueStr.c_str(), NULL, 10);
            get_bpm(client, fromTime);
        }
        else
        {
            get_bpm(client, 0);
        }
    }
    
    print.println();
    delay(1);
    client.stop();
}

/*
void checkHeartbeat()
{
    while (WIFI_SERIAL.available())
    {
        byte heartbeatBytes[heartbeatSize];
        size_t numBytes = WIFI_SERIAL.readBytes(heartbeatBytes, heartbeatSize);
        if (numBytes != heartbeatSize)
        {
            DEBUG_SERIAL.print("Incorrect size: ");
            DEBUG_SERIAL.print(numBytes);
            DEBUG_SERIAL.print("/");
            DEBUG_SERIAL.println(heartbeatSize);
            //continue;
        }
        DEBUG_SERIAL.print("Heartbeat ");
        //DEBUG_SERIAL.print(numBytes);
        //DEBUG_SERIAL.println(" bytes");
        Heartbeat heartbeat;
        memcpy(&heartbeat, heartbeatBytes, heartbeatSize);
        printlnHeartbeat(heartbeat, DEBUG_SERIAL);
        addHeartbeat(heartbeat);
    }
}
*/

void printBytes(byte bytes[], size_t size, Print& print)
{
    print.print(size);
    print.print(F(" bytes: "));
    for (int i = 0; i < size; ++i)
    {
        print.print(bytes[i], BIN);
        print.print(" ");
    }
}

Heartbeat lastHeartbeat = { 0, 0 };

void requestHeartbeat(Print& print)
{
    unsigned long m = millis();
    if ((m - lastHeartbeatRequest) > 10000) // every 10 second
    {
        lastHeartbeatRequest = m;
        Wire.requestFrom(ARD_I2C_ADDRESS, heartbeatSize);
        byte heartbeatBytes[heartbeatSize];
        while (Wire.available())
        {
            size_t numReadBytes = Wire.readBytes(&heartbeatBytes[0], heartbeatSize);
            
            Heartbeat heartbeat;
            memcpy(&heartbeat, &heartbeatBytes[0], heartbeatSize);
            
            // only add if last heartbeat time is different
            if (heartbeat.time != lastHeartbeat.time)
            {
                print.print("Heartbeat ");
                printlnHeartbeat(heartbeat, print);

                lastHeartbeat = heartbeat;
                addHeartbeat(heartbeat);
            }
            
        }
    }
}



void loop()
{
    requestHeartbeat(DEBUG_SERIAL);

	// Check if a client has connected
	WiFiClient client = server.available();
	if (!client)
	{
		return;
	}

	// Wait until the client sends some data
    //String clientInfo = client.remoteIP().toString() + ":" + client.remotePort();
    //DEBUG_SERIAL.print(F("New Client "));
    //DEBUG_SERIAL.println(clientInfo);
    //DEBUG_SERIAL.print(F("Waiting for data"));
	while (!client.available())
	{
		delay(10);
        //DEBUG_SERIAL.print(F("."));
	}
	//DEBUG_SERIAL.println(F(" ok."));

	route_client(client, DEBUG_SERIAL);
	
    //DEBUG_SERIAL.print(F("Done "));
    //DEBUG_SERIAL.println(clientInfo);

	// The client will actually be disconnected 
	// when the function returns and 'client' object is detroyed
    delay(1);
}
