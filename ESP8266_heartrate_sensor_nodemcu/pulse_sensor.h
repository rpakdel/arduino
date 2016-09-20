#pragma once

#include <Arduino.h>
#include <Ticker.h>

/*  Pulse Sensor Amped 1.4    by Joel Murphy and Yury Gitman   http://www.pulsesensor.com

----------------------  Notes ----------------------  ----------------------
This code:
1) Blinks an LED to User's Live Heartbeat   PIN 13
2) Fades an LED to User's Live HeartBeat
3) Determines BPM
4) Prints All of the Above to Serial

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
----------------------       ----------------------  ----------------------
*/

//  Variables
#define PULSE_PIN 0                 // Pulse Sensor purple wire connected to analog pin 0

                                  // Volatile Variables, used in the interrupt service routine!

volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
volatile int BPM = 0;                // int that holds raw Analog in 0. updated every 2mS

#define P_SEED 128
#define T_SEED 128
#define THRESH_SEED 128

volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P = P_SEED;                      // used to find peak in pulse wave, seeded
volatile int T = T_SEED;                     // used to find trough in pulse wave, seeded
volatile int thresh = THRESH_SEED;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

volatile int minSignal = 1023;
volatile int maxSignal = 0;

#define NUM_DISPLAY_SIGNALS 25 // 128 pixels / 5 pixel per char
int signals[25];
int currentSignal = 0;

void resetSignals()
{
    for (int i = 0; i < NUM_DISPLAY_SIGNALS; ++i)
    {
        signals[i] = 0;
    }
}

void storeAndIncSignal(int value)
{
    signals[currentSignal] = value;
    currentSignal++;
    if (currentSignal >= NUM_DISPLAY_SIGNALS)
    {
        currentSignal = 0;
    }
}

Ticker ticker;


// Timer 2 makes sure that we take a reading every 2 miliseconds
void check_pulse_sensor()
{

    Signal = analogRead(PULSE_PIN);              // read the Pulse Sensor 
    storeAndIncSignal(Signal);
    //minSignal = min(minSignal, Signal);
    //maxSignal = max(maxSignal, Signal);
                                                //Serial.println(Signal);
    sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

                                                //  find the peak and trough of the pulse wave
    if (Signal < thresh && N >(IBI / 5) * 3) {       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T) {                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
        }
    }

    if (Signal > thresh && Signal > P) {          // thresh condition helps avoid noise
        P = Signal;                             // P is the peak
    }                                        // keep track of highest point in pulse wave

                                             //Serial.print("S: ");
                                             //Serial.print(Signal);

                                             //Serial.print(" T: ");
                                             //Serial.print(T);

                                             //Serial.print(" P: ");
                                             //Serial.println(P);
                                             //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
                                             // signal surges up in value every time there is a pulse
    if (N > 250) {                                   // avoid high frequency noise
        if ((Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3)) {
            Pulse = true;                               // set the Pulse flag when we think there is a pulse
                                                        //digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
            IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
            lastBeatTime = sampleCounter;               // keep track of time for next pulse

            if (secondBeat) {                        // if this is the second beat, if secondBeat == TRUE
                secondBeat = false;                  // clear secondBeat flag
                for (int i = 0; i <= 9; i++) {             // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;
                }
            }

            if (firstBeat) {                         // if it's the first time we found a beat, if firstBeat == TRUE
                firstBeat = false;                   // clear firstBeat flag
                secondBeat = true;                   // set the second beat flag
                sei();                               // enable interrupts again
                return;                              // IBI value is unreliable so discard it
            }


            // keep a running total of the last 10 IBI values
            word runningTotal = 0;                  // clear the runningTotal variable    

            for (int i = 0; i <= 8; i++) {                // shift data in the rate array
                rate[i] = rate[i + 1];                  // and drop the oldest IBI value 
                runningTotal += rate[i];              // add up the 9 oldest IBI values
            }

            rate[9] = IBI;                          // add the latest IBI to the rate array
            runningTotal += rate[9];                // add the latest IBI to runningTotal
            runningTotal /= 10;                     // average the last 10 IBI values 
            BPM = 60000 / runningTotal;               // how many beats can fit into a minute? that's BPM!
            QS = true;                              // set Quantified Self flag 
                                                    // QS FLAG IS NOT CLEARED INSIDE THIS ISR
        }

    }

    if (Signal < thresh && Pulse == true) {   // when the values are going down, the beat is over
                                              //digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
        Pulse = false;                         // reset the Pulse flag so we can do it again
        amp = P - T;                           // get amplitude of the pulse wave
        thresh = amp / 2 + T;                    // set thresh at 50% of the amplitude
        P = thresh;                            // reset these for next time
        T = thresh;
    }

    if (N > 2500) {                           // if 2.5 seconds go by without a beat
        thresh = THRESH_SEED;                          // set thresh default
        P = P_SEED;                               // set P default
        T = T_SEED;                               // set T default
        lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
        firstBeat = true;                      // set these to avoid noise
        secondBeat = false;                    // when we get the heartbeat back
    }
}

void pulseSensorSetup()
{
    ticker.attach_ms(2, check_pulse_sensor);
}









