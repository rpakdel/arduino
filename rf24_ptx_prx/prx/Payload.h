#pragma once

#define PAYLOADITEMS 8

struct Payload
{
  double A;
#if PAYLOADITEMS > 1
  double B;
#endif
#if PAYLOADITEMS > 2
  double C;
#endif
#if PAYLOADITEMS > 3
  double D;
#endif
#if PAYLOADITEMS > 4
  double E;
#endif
#if PAYLOADITEMS > 5
  double F;
#endif
#if PAYLOADITEMS > 6
  double G;
#endif
#if PAYLOADITEMS > 7
  double H;
#endif
#if PAYLOADITEMS > 8
  double I;
#endif
};

void Initialize(Payload &payload)
{
  byte i = 0;
  payload.A = i;
#if PAYLOADITEMS > 1
  i++;
  payload.B = i;
#endif
#if PAYLOADITEMS > 2
  i++;
  payload.C = i;
#endif
#if PAYLOADITEMS > 3
  i++;
  payload.D = i;
#endif
#if PAYLOADITEMS > 4
  i++;
  payload.E = i;
#endif
#if PAYLOADITEMS > 5
  i++;
  payload.F = i;
#endif
#if PAYLOADITEMS > 6
  i++;
  payload.G = i;
#endif
#if PAYLOADITEMS > 7
  i++;
  payload.H = i;
#endif
#if PAYLOADITEMS > 8
  i++;
  payload.I = i;
#endif
}

void Increment(Payload &payload)
{
  payload.A++;
#if PAYLOADITEMS > 1
  payload.B++;
#endif
#if PAYLOADITEMS > 2
  payload.C++;
#endif
#if PAYLOADITEMS > 3
  payload.D++;
#endif
#if PAYLOADITEMS > 4
  payload.E++;
#endif
#if PAYLOADITEMS > 5
  payload.F++;
#endif
#if PAYLOADITEMS > 6
  payload.G++;
#endif
#if PAYLOADITEMS > 7
  payload.H++;
#endif
#if PAYLOADITEMS > 8
  payload.I++;
#endif
}

void Print(Payload &payload)
{
  Serial.print("A:");
  Serial.print(payload.A);
#if PAYLOADITEMS > 1
  Serial.print(", B:");
  Serial.print(payload.B);
#endif
#if PAYLOADITEMS > 2
  Serial.print(", C:");
  Serial.print(payload.C);
#endif
#if PAYLOADITEMS > 3
  Serial.print(", D:");
  Serial.print(payload.D);
#endif
#if PAYLOADITEMS > 4
  Serial.print(", E:");
  Serial.print(payload.E);
#endif
#if PAYLOADITEMS > 5
  Serial.print(", F:");
  Serial.print(payload.F);
#endif
#if PAYLOADITEMS > 6
  Serial.print(", G:");
  Serial.print(payload.G);
#endif
#if PAYLOADITEMS > 7
  Serial.print(", H:");
  Serial.print(payload.H);
#endif
#if PAYLOADITEMS > 8
  Serial.print(", I:");
  Serial.print(payload.I);
#endif

  Serial.print(" (");
  Serial.print(sizeof(Payload));
  Serial.print(" bytes)");
  Serial.println();
}
