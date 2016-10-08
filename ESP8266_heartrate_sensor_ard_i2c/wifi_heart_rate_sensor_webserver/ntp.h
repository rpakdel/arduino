#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>

extern void printlnTime(time_t t, Print& print);
extern time_t getCurrentTime(time_t start);

WiFiUDP itpClient;

// time gap in seconds from 01.01.1900 (NTP time) to 01.01.1970 (UNIX time)
#define DIFF1900TO1970 2208988800UL

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

// Gets time_t as seconds since 1970/1/1 (Unix Epoch)
time_t getNTPPacket(Print &print)
{
    int tryCount = 0;
    while (tryCount < 10)
    {
        if (itpClient.parsePacket())
        {
            // We've received a packet, read the data from it
            // read the packet into the buffer
            itpClient.read(packetBuffer, NTP_PACKET_SIZE);

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
        print.print(F("."));
        delay(1000);
        tryCount++;
    }

    return 0;
}

time_t setStartTimeUsingNTP(time_t& start, Print& print)
{
    itpClient.begin(123);
    print.println(F("Sending ITP packet"));
    sendNTPpacket();    
    print.print(F("Waiting for time"));
    start = getNTPPacket(print);
    print.println();
    // all subsequent millis are counted against timeOffset

    time_t now = getCurrentTime(start);
    print.print(F("NTP time: "));
    printlnTime(now, print);
    //DEBUG_SERIAL.println();
    //DEBUG_SERIAL.print(F("Seconds since Jan 1 1900: "));
    //DEBUG_SERIAL.println(startTime);
    //DEBUG_SERIAL.print(F("Local offset (ms): "));
    //DEBUG_SERIAL.println(timeOffset);

    itpClient.stop();

    return now;
}


