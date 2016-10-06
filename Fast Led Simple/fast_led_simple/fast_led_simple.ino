#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN D1
#define CLOCK_PIN D2

CRGB leds[NUM_LEDS];

void setup() { 

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //we put BRG here as the color scheme because nodemcu implements it that way... god knows why!!!
  
  FastLED.setBrightness(255);
  FastLED.show();
}


void loop() { 
   // Fill leds with rainbow
  fill_rainbow(leds, NUM_LEDS, millis());
  FastLED.show();
  delay(20);
}
