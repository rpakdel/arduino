
#include "RadioRole.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "RadioRole.h"
#include "Payload.h"

RF24 radio(9, 10);

// Topology
// Radio pipe addresses for the 2 nodes to communicate.
byte pipes[][6] = 
{ 
  "1Node", "2Node"
};

RadioRole role = Invalid;

// The debug-friendly names of those roles
const char* role_friendly_name[] = 
{ 
  "Invalid", 
  "PTX", 
  "PRX"
};  

Payload payload;

void SetToRole(RadioRole r)
{
  role = r;
  Serial.print("> Setting to role ");
  Serial.println(role_friendly_name[role]);
  switch (r)
  {
  case PTX:
    Serial.print("PTX> Writing to pipe address ");
    Serial.println((const char*)pipes[0]);
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
    radio.stopListening();
    break;

  case PRX:   
    Serial.print("PRX> Reading from pipe address ");
    Serial.println((const char*)pipes[1]);
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
    radio.startListening();
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  // Setup and configure rf radio
  radio.begin();
  radio.setAutoAck(true);                 // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloa
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries  
  radio.setChannel(108);
  radio.enableDynamicPayloads();
  radio.setPayloadSize(sizeof(Payload));
  radio.setPALevel(RF24_PA_MAX);
  SetToRole(PRX);

  Initialize(payload);
}

void Transmit()
{
  radio.stopListening();
  Serial.print("PTX> Now sending ");
  Print(payload);
  
  if (!radio.write(&payload, sizeof(Payload)))
  {
    Serial.println(F("PTX> failed"));
    return;
  }

  // check payload
  if (!radio.isAckPayloadAvailable())
  {
    Serial.println(F("PTX> Blank payload"));
    return;
  }

  // read payload
  while (radio.available())
  {
    radio.read(&payload, sizeof(Payload));
    Serial.print("PTX> Got payload ");
  Print(payload);
  }
}

void Receive()
{
  byte pipeNo;
  while (radio.available(&pipeNo))
  {
    radio.read(&payload, sizeof(Payload));
    Serial.print("PRX> Got ");
  Print(payload);
  Increment(payload);
    Serial.print("PRX> Responding with ");
  Print(payload);

    radio.writeAckPayload(pipeNo, &payload, sizeof(Payload));
  }
}

void loop(void)
{
  switch (role)
  {
  case PTX:
    Transmit();
  delay(200);
    break;

  case PRX:
    Receive();
    break;
  }

  // Try again later
  
}

