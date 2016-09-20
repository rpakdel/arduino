/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include "pulse_sensor.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <Time.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include "heartbeat.h"

#define DEBUG_SERIAL Serial 
#define WIFI_SERIAL Serial



#include "pulse_sensor.h"

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
WiFiUDP itpClient;

const char newLine[] PROGMEM = "\r\n";
const char http_status_200_OK[] PROGMEM = "HTTP/1.1 200 OK";
const char http_status_404_NOTFOUND[] PROGMEM = "HTTP/1.1 404 NOT FOUND";

const char content_type_js[] PROGMEM = "Content-Type: application/javascript";
const char content_type_css[] PROGMEM = "Content-Type: text/css";
const char content_type_html[] PROGMEM = "Content-Type: text/html";
const char content_type_json[] PROGMEM = "Content-Type: application/json";

time_t startTime = 0;
time_t elapsedTime = 0;
time_t timeOffset = 0;

// store bmp if more than 5 second has passed
#define BMP_TIME_DELAY 5000

// time gap in seconds from 01.01.1900 (NTP time) to 01.01.1970 (UNIX time)
#define DIFF1900TO1970 2208988800UL

#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

// must disable visual micro deep search for this to work
const char index_html_FileContent[] PROGMEM =
#include "..\ESP8266_heartrate_sensor_nodemcu\index.html"
;

const char client_js_FileContent[] PROGMEM =
#include "..\ESP8266_heartrate_sensor_nodemcu\client.js"
;

const char style_css_FileContent[] PROGMEM =
#include "..\ESP8266_heartrate_sensor_nodemcu\style.css"
;

// can't store this in program memory
const char ntpServer[] = "pool.ntp.org";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packet

// send an NTP request to the time server at the given address
void sendNTPpacket() 
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
                             // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    itpClient.beginPacket(ntpServer, 123); //NTP requests are to port 123
    itpClient.write(packetBuffer, NTP_PACKET_SIZE);
    itpClient.endPacket();
}

time_t getNTPPacket()
{
    int tryCount = 0;
    while (tryCount < 10)
    {
        if (itpClient.parsePacket())
        {
            // We've received a packet, read the data from it
            itpClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

                                                     // the timestamp starts at byte 40 of the received packet and is four bytes,
                                                     // or two words, long. First, extract the two words:

            time_t highWord = word(packetBuffer[40], packetBuffer[41]);
            time_t lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            time_t secsSince1900 = highWord << 16 | lowWord;
            itpClient.stop();
            // the different between NTP and UNIX time
            return secsSince1900 - DIFF1900TO1970;
        }
        //DEBUG_SERIAL.print(F("."));
        delay(1000);
        tryCount++;
    }

    return 0;
}

void oledPrintTime(time_t t)
{
    // convert to PST (with daylight savings)
    // subtract 7 hours or 25200 seconds
    t -= 25200;

    oled.setCursor(0, 1);
    oled.clearToEOL();
    oled.print(year(t));
    oled.print("/");
    oled.print(month(t));
    oled.print("/");
    oled.print(day(t));

    oled.print(" ");

    oled.print(hour(t));
    oled.print(":");
    oled.print(minute(t));
}

void getTimeUsingNTP()
{
    itpClient.begin(123);
    //DEBUG_SERIAL.println(F("Sending ITP packet"));
    sendNTPpacket();
    // assume th
    //DEBUG_SERIAL.print(F("Waiting for time"));
    startTime = getNTPPacket();
    // all subsequent millis are counted against timeOffset
    

    //DEBUG_SERIAL.println();
    //DEBUG_SERIAL.print(F("Seconds since Jan 1 1900: "));
    //DEBUG_SERIAL.println(startTime);
    //DEBUG_SERIAL.print(F("Local offset (ms): "));
    //DEBUG_SERIAL.println(timeOffset);

    time_t now = getCurrentTimeInSeconds();
    oledPrintTime(now);
    
    DEBUG_SERIAL.print(year(now));
    DEBUG_SERIAL.print("/");
    DEBUG_SERIAL.print(month(now));
    DEBUG_SERIAL.print("/");
    DEBUG_SERIAL.print(day(now));

    DEBUG_SERIAL.print(" ");

    DEBUG_SERIAL.print(hour(now));
    DEBUG_SERIAL.print(":");
    DEBUG_SERIAL.print(minute(now));
    DEBUG_SERIAL.print(":");
    DEBUG_SERIAL.print(second(now));

    DEBUG_SERIAL.println();

    itpClient.stop();
}

#define MAX_HEARTBEATS 1000

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
        h.time = getCurrentTimeInSeconds();
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

void setupOLED()
{
    Wire.begin();
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.set400kHz();
    oled.setFont(Adafruit5x7);
    oled.clear();
}

void setup() 
{  
  DEBUG_SERIAL.begin(115200);
  delay(10);

  setupOLED();

  oled.print("SSID: ");
  oled.print(MYSSID);

  // Connect to WiFi network
  //DEBUG_SERIAL.print(F("Connecting to "));
  DEBUG_SERIAL.println(MYSSID);
  
  WiFi.begin(MYSSID, MYPASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    //DEBUG_SERIAL.print(F("."));
  }

  oled.clear();
  oled.print(WiFi.localIP());
  oled.print(":");
  oled.print(WEBSERVER_PORT);

  // Print the IP address
  DEBUG_SERIAL.print(WiFi.localIP());
  DEBUG_SERIAL.print(F(":"));
  DEBUG_SERIAL.println(WEBSERVER_PORT);
  
  //DEBUG_SERIAL.println();
  //DEBUG_SERIAL.println(F("WiFi connected"));
  
  getTimeUsingNTP();

  // Start the server
  server.begin();
  //DEBUG_SERIAL.println(F("Server started"));

  

  pulseSensorSetup();
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
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_html);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR(index_html_FileContent);
    //DEBUG_SERIAL.println(s);
    client.print(s);
}

void get_client_js(WiFiClient& client)
{
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_js);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR(client_js_FileContent);
    //DEBUG_SERIAL.println(s);
    client.print(s);
}

void get_style_css(WiFiClient& client)
{
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_css);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR(style_css_FileContent);
    //DEBUG_SERIAL.println(s);
    client.print(s);
}

time_t getCurrentTimeInSeconds()
{
    unsigned long t = millis() - timeOffset;
    unsigned long sec = t / 1000;
    unsigned long currentSeconds = startTime + sec;
    return currentSeconds;
}

void get_time(WiFiClient& client)
{
    unsigned long currentSeconds = getCurrentTimeInSeconds();

    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_json);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR("{ \"time\": ");
    s += String(currentSeconds);
    s += FPSTR(" }");
    //DEBUG_SERIAL.println(s);
    client.print(s);
}

void get_bpm(WiFiClient& client, ulong fromTime)
{
    String heartbeatsJSON = heartbeatsToJSON(heartbeats, heartbeat_index, fromTime);

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


void handleClient(WiFiClient& client)
{
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
        get_index_html(client);
    }
    else if (req.indexOf(F("GET /client.js")) >= 0)
    {
        get_client_js(client);
    }
    else if (req.indexOf(F("GET /style.css")) >= 0)
    {
        get_style_css(client);
    }
    else if (req.indexOf(F("GET /favicon.ico")) >= 0)
    {
        get_favicon(client);
    }
    else if (req.indexOf(F("GET /api/v1/time")) >= 0)
    {
        get_time(client);
    }
    else if (req.indexOf(F("GET /api/v1/bpm")) >= 0)
    {
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
    else
    {
        //DEBUG_SERIAL.println(F("Unknown request"));
        client.stop();
    }
}

Heartbeat tmph;
const size_t heartbeatSize = sizeof(tmph);

unsigned long lastHeartbeatCheck = 0;

void oledPrintSignals()
{
    //oled.clear(5, 3, 25, 3);
    //oled.print(minSignal);
    //oled.print(" ");
    //oled.print(maxSignal);
    for (int i = 0; i < NUM_DISPLAY_SIGNALS; ++i)
    {
        if (signals[i] != 0)
        {
            int col = i;
            // map the signal value to rows 4 to 9 (64 pixels / 7 pixels per char)
            int row = map(signals[i], 0, 300, 4, 9);

            // clear the column
            oled.clear(col, col + 1, 4, 9);
            oled.setCursor(col, row);
            oled.print("-");
        }
    }
}

void oledClearHeartbeat()
{
    oled.setCursor(0, 3);
    oled.clearToEOL();
}

void oledPrintHeartbeat()
{
    oledClearHeartbeat();
    oled.print(BPM);
    if (Pulse)
    {
        oled.print(" ");
        oled.print("*");
    }
}

void checkHeartbeat()
{
    unsigned long m = millis();

    if (QS)
    {
        QS = false;
        oledPrintHeartbeat();
        if ((m - lastHeartbeatCheck) >= 5000)
        {
            lastHeartbeatCheck = m;
            Heartbeat h = { 0, BPM };
            addHeartbeat(h);
        }
    }
    else
    {
        if ((m - lastHeartbeatCheck) >= 5000)
        {
            oledClearHeartbeat();
        }
    }
}

unsigned long lastCurrentTimeCheck = 0;

void oledPrintCurrentTime()
{
    unsigned long m = millis();
    if ((m - lastCurrentTimeCheck) > 60000)
    {
        lastCurrentTimeCheck = m;
        time_t now = getCurrentTimeInSeconds();
        oledPrintTime(now);
    }
}

void checkClient()
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }

    // Wait until the client sends some data
    String clientInfo = client.remoteIP().toString() + ":" + client.remotePort();
    //DEBUG_SERIAL.print(F("New Client "));
    //DEBUG_SERIAL.println(clientInfo);
    //DEBUG_SERIAL.print(F("Waiting for data"));
    while (!client.available())
    {
        delay(10);
        //DEBUG_SERIAL.print(F("."));
    }
    //DEBUG_SERIAL.println(F(" ok."));

    handleClient(client);

    //DEBUG_SERIAL.print(F("Done "));
    //DEBUG_SERIAL.println(clientInfo);

    // The client will actually be disconnected 
    // when the function returns and 'client' object is detroyed
}



void loop()
{
    //DEBUG_SERIAL.print(F("VCC: "));
    //DEBUG_SERIAL.println(ESP.getVcc());
    oledPrintCurrentTime();
    // check for heartbeat data
    oledPrintSignals();

    checkHeartbeat();

    checkClient();
}

