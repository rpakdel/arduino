Wifi enabled heart rate sensor (similar to https://github.com/rpakdel/arduino/tree/master/ESP8266_heartrate_sensor_ard)

Hardware
* NodeMCU V3 (Lolin) with base board
* Heart rate sensor (http://pulsesensor.com)
* Voltage divider 5V to 1V with R1 (pulse sensor) = 330k Ohm and R2 (GND) = 100k (1.163)

Software
* A basic web-server with simple web-api
* Pages and files are compiled as literal strings along with the rest of the sketch
* Use Internal Time Protocol (ITP) to get current time

Web-API
* "GET / ": index.html
* "GET /client.js": client side JS
* "GET /style.css": css style
* "GET /favicon.ico": fav icon (404 for now)
* "GET /api/v1/time": get the current time in UNIX timestamp in JSON { "time" : sec }
* "GET /api/v1/bpm": get the beats per minute in JSON { "unix_time_s" : sec, "bpm", bpm }
* "GET /api/v1/bpm?gt=" get the bpm greater or equal to time (for incremental updates)

Client side
* Pretty basic for now
* Chart.js for graphing BPM

TODO
* Improve performance
* client.flush when body for message is not needed
* Save heartbeat data in non-destructive flash
* Use Websockets to indicate pulses
