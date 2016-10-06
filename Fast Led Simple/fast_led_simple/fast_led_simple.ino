#define FASTLED_ESP8266_RAW_PIN_ORDER
// we need to include this before the FastLED import to define the mappings
// more info here https://github.com/FastLED/FastLED/wiki/ESP8266-notes

#include <FastLED.h>
#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN D1
#define CLOCK_PIN D2

CRGB leds[NUM_LEDS];

void setup() { 

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // we put BRG here as the color scheme because nodemcu implements it that way... god knows why!!!

  FastLED.setBrightness(255);
  // clear
  clear();
  FastLED.show();
}

void loop() {
  // Fill leds with rainbow
  fill_rainbow(leds, NUM_LEDS, millis());
  FastLED.show();
  delay(20);
}

void clear() {
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    }
}
