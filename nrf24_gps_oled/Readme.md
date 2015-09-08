* GPS + NRF24 + OLED sender
* NRF24 + LCD receiver
* An the sender side, the Adafruit SSD1306 uses 1k of RAM since it is buffer and so
the sketch is Arduino out of memory.
* https://github.com/greiman/SSD1306Ascii uses less RAM but is ASCII text only.