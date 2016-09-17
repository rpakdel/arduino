#pragma once

typedef struct _Heartbeat
{
    unsigned long time;
    byte bpm;
} Heartbeat;

String heartbeatToJSON(const Heartbeat& heartbeat)
{
    String s = F("{ \"time\" : ");
    s += heartbeat.time;
    s += ", \"bpm\" : ";
    s += heartbeat.bpm;
    s += " }";

    return s;
}

String heartbeatsToJSON(const Heartbeat* heartbeats, int length, unsigned long fromTime)
{
    String s = "[ ";
    for (int i = 0; i < length; ++i)
    {
        Heartbeat h = heartbeats[i];
        // skip if heart beat is less than given time
        if (h.time <= fromTime)
        {
            continue;
        }

        String hs = heartbeatToJSON(h);
        s += hs;
        if (i < (length - 1))
        {
            s += ", ";
        }
    }
    s += " ]";
    return s;
}

void printHeartbeat(Heartbeat& h, Print& print)
{
    print.print(h.time);
    print.print(",");
    print.print(h.bpm);
}

void printlnHeartbeat(Heartbeat& h, Print& print)
{
    printHeartbeat(h, print);
    print.println();
}
