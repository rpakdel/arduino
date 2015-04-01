/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
* Version 1.3   January 2014
* Copyright 2014 by Chris Young http://cyborg5.com
* Based on original example sketch for IRremuote library
* Version 0.11 September, 2009
* Copyright 2009 Ken Shirriff
* http://www.righto.com/
*/
/*
* IRLib: IRrecvDump - dump details of IR codes with IRrecv
* An IR detector/demodulator must be connected to the input RECV_PIN.
*/

#include <IRLib.h>

int RECV_PIN = 11;

IRrecv receiver(RECV_PIN);

IRdecode decoder;
unsigned int Buffer[RAWBUF];

void setup()
{
    Serial.begin(9600);
    delay(500); while (!Serial);//delay for Leonardo
    receiver.enableIRIn(); // Start the receiver
    decoder.UseExtnBuf(Buffer);
    Serial.println("Using IRLib");
}

void translateIR(unsigned long value)
{
    switch (value)
    {

    case 0xFF629D: Serial.println("UP");
        break;
    case 0xFF22DD: Serial.println("LEFT");
        break;
    case 0xFF02FD: Serial.println("OK");
        break;
    case 0xFFC23D: Serial.println("RIGHT");
        break;
    case 0xFFA857: Serial.println("DOWN");
        break;
    case 0xFF6897: Serial.println("1");
        break;
    case 0xFF9867: Serial.println("2");
        break;
    case 0xFFB04F: Serial.println("3");
        break;
    case 0xFF30CF: Serial.println("4");
        break;
    case 0xFF18E7: Serial.println("5");
        break;
    case 0xFF7A85: Serial.println("6");
        break;
    case 0xFF10EF: Serial.println("7");
        break;
    case 0xFF38C7: Serial.println("8");
        break;
    case 0xFF5AA5: Serial.println("9");
        break;
    case 0xFF42BD: Serial.println("*");
        break;
    case 0xFF4AB5: Serial.println("0");
        break;
    case 0xFF52AD: Serial.println("#");
        break;
    case 0xFFFFFFFF: Serial.println("REPEAT");
        break;

    default:
        Serial.println(value, HEX);
        break;
    }
}

void loop() {
    if (receiver.GetResults(&decoder)) {
        //Restart the receiver so it can be capturing another code
        //while we are working on decoding this one.
        receiver.resume();
        if (decoder.decode())
        {
            translateIR(decoder.value);
        }
    }
}
