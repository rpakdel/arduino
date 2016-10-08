#pragma once

#include <Arduino.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>

static RTC_DS3231 rtc;

void printlnDateTime(DateTime t, Print& print)
{
    print.print(t.year());
    print.print("/");
    print.print(t.month());
    print.print("/");
    print.print(t.day());

    print.print(" ");

    print.print(t.hour());
    print.print(":");
    print.print(t.minute());
    print.print(":");
    print.print(t.second());

    print.println();
}

void printlnTime(time_t t, Print& print)
{
    print.print(year(t));
    print.print("/");
    print.print(month(t));
    print.print("/");
    print.print(day(t));

    print.print(" ");

    print.print(hour(t));
    print.print(":");
    print.print(minute(t));
    print.print(":");
    print.print(second(t));

    print.println();
}

// Gets the current time by comparing number of seconds passed since given
// start time.
time_t getCurrentTime(time_t start)
{
    unsigned long t = millis();
    unsigned long sec = t / 1000;
    unsigned long currentSeconds = start + sec;
    return currentSeconds;
}

// get the time using RTC
// 32-bit time as seconds since 1970/1/1 (Unix Epoch)
unsigned long getTimeUsingRTC()
{
    DateTime t = rtc.now();
    return t.unixtime();
}

time_t setStartTimeUsingRTC(time_t& start, Print& print)
{
    start = getTimeUsingRTC();

    time_t now = getCurrentTime(start);
    print.print(F("RTC time: "));
    printlnTime(now, print);
    return now;
}



#ifdef SET_RTC_TO_NTP
time_t setRTCTimeToNTPTime(Print &print)
{
    time_t start = 0;
    time_t now = setStartTimeUsingNTP(start, print);
    DateTime t(now);
    rtc.adjust(now);
    return now;
}
#endif

#ifdef SET_RTC_TO_BUILD
time_t setRTCTimeToBuildTime(Print& print)
{
    DateTime buildTime(F(__DATE__), F(__TIME__));
    print.print(F("Setting RTC time to "));
    printlnDateTime(buildTime, print);
    rtc.adjust(buildTime);
    return buildTime.unixtime();
}
#endif

time_t RTCSetup(Print& print)
{
    print.println(F("RTC setup"));
    if (!rtc.begin())
    {
        print.println(F("Couldn't find RTC"));
        return 0;
    }

#ifdef SET_RTC_TO_NTP
    return setRTCTimeToNTPTime(print);
#elif SET_RTC_TO_BUILD
    return setRTCTimeToBuildTime(print);
#else
    time_t startTime = 0;
    setStartTimeUsingRTC(startTime, print);
    return startTime;
#endif
}

