#pragma once

struct Payload
{
  byte A;
  byte B;
  byte C;
  byte D;
  byte E;
};

void Initialize(Payload &payload)
{
  payload.A = 0;
  payload.B = 1;
  payload.C = 2;
  payload.D = 3;
  payload.E = 4;
}

void Increment(Payload &payload)
{
  payload.A++;
  payload.B++;
  payload.C++;
  payload.D++;
  payload.E++;
}

void Print(Payload &payload)
{
  Serial.print("A:");
  Serial.print(payload.A);
  Serial.print(", B:");
  Serial.print(payload.B);
  Serial.print(", C:");
  Serial.print(payload.C);
  Serial.print(", D:");
  Serial.print(payload.D);
  Serial.print(", E:");
  Serial.print(payload.E);
  Serial.print(" (");
  Serial.print(sizeof(Payload));
  Serial.print(" bytes)");
  Serial.println();
}
