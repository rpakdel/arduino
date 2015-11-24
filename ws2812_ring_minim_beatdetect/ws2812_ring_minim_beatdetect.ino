// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

SoftwareSerial serial2(5, 4);

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

int pixelColors[NUMPIXELS * 3]; // 16 pixels * RGB

#define NEWLINE 10
#define CARRIAGERETURN 13
#define COMMA 44
#define COLORSIZE 8
#define SEMICOLON 59

void setup() 
{
  Serial.begin(19200);
  serial2.begin(115200);
  serial2.println(F("----------ready-----------"));
  pixels.begin(); // This initializes the NeoPixel library.
}


void loop() 
{
  //for(int i = 0; i < NUMPIXELS; i++)
  //{    
    //int offset = i * 3;
    //pixelColors[offset] = 0;
    //pixelColors[offset + 1] = 0;
    //pixelColors[offset + 2] = 0;
  //}
  
  char colorBuffer[COLORSIZE + 1];
  if (Serial.available())
  {
    int pixelIndex = 0;
    while(pixelIndex < NUMPIXELS)
    {
      //serial2.print(pixelIndex);
      //serial2.print(":");
      
      int colorBufferIndex = 0;
      
      // read until COLORSIZE bytes are read
      bool gotNewLine = false;
      while (colorBufferIndex < COLORSIZE)
      {
        if (Serial.available())
        {
         byte b = Serial.read(); 
 
         
         
         if (b == SEMICOLON)
         {           
           gotNewLine = true; 
           break;
         }        

         
         colorBuffer[colorBufferIndex] = b;
         colorBufferIndex++;
        }
      }
      
      if (gotNewLine)
      {
        //serial2.println(F("NL"));
        
        break;
      }
      
      colorBuffer[colorBufferIndex] = '\0';  
    
    
      if (colorBufferIndex == COLORSIZE)
      {
        //serial2.print(colorBuffer);
        //serial2.print(" ");
        
        
        
        long c = strtol(&colorBuffer[2], NULL, 16);
        //serial2.print(F("Color: "));
        //serial2.print(c);
        ////serial2.print(": ");
        int r = c >> 16 & 0xFF;
        int g = c >> 8 & 0xFF;
        int b = c & 0xFF;
            
        int offset = pixelIndex * 3;
        pixelColors[offset] = r;
        pixelColors[offset + 1] = g;
        pixelColors[offset + 2] = b;
                
        //serial2.print("|");  
      }
      else
      {
        //serial2.println(F("Didn't get 6 bytes. Skipping pixel"));
      }
      
      pixelIndex++;         
    }       
    
    ////serial2.println();
  }

  // For a set of NeoPixels the first NeoPixel is 0, 
  // second is 1, all the way up to the count of pixels minus one.
  for(int i = 0; i < NUMPIXELS; i++)
  {
    int offSet = i * 3; 
    int red = pixelColors[offSet];
    int green = pixelColors[offSet + 1];
    int blue = pixelColors[offSet + 2];
    
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255    
    pixels.setPixelColor(i, pixels.Color(red,green,blue));

    pixels.show();
    
  }
}

