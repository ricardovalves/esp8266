#define FASTLED_FORCE_SOFTWARE_SPI

#include <FastLED.h>

#define NUM_LEDS 30

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<APA102, D2, D3>(leds, NUM_LEDS);
}

void loop() { 
  // Turn the LED on, then pause

  for(int i=0 ;i< 30; i++){
   leds[i] = CRGB::Blue;
  } 
  FastLED.show();
  delay(30);
}

