#pragma once

typedef struct _Heartbeat
{
    ulong time;
    byte bpm;
} Heartbeat;

String heartbeatToJSON(const Heartbeat& heartbeat)
{
    String s = F("{ \"unix_time_s\" : ");
    s += heartbeat.time;
    s += ", \"bpm\" : ";
    s += heartbeat.bpm;
    s += " }";

    return s;
}

String heartbeatsToJSON(const Heartbeat* heartbeats, int length, ulong fromTime)
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
