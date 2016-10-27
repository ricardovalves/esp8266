#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>
#include <Homie.h>
#include <Ticker.h>
#include "Utils.h"

/* Homie OTA details */
#define FW_NAME "leds"
#define FW_VERSION "1.0.0"

/* FastLED properties */
#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN D1
#define CLOCK_PIN D2

#define FPS 25
#define FRAME_PERIOD (1000 / FPS)

/* Main color */
CHSV color(0, 255, 255);

/* Ticker will handle ledstrip rendering */
Ticker renderTicker;

/* Instantiate leds */
CRGB leds[NUM_LEDS];

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

/* Define HomieNode */
HomieNode stripNode("ledstrip", "led-strip");

/* Handle color changes 
   Receive color from the MQTT broker and update the color property
   Render method will handle the ledstrip rendering
*/
bool colorOnHandler(String value) {  
  Serial.printf("colorHandler(%s)\n", value.c_str());
  const char *pData = value.c_str();
  color = toHSV(pData);
  return true;
}

void setup() {  

  Serial.begin(115200);
  Serial.println();
  Serial.println(FW_NAME " " FW_VERSION);
  Serial.println("Built @ " __DATE__ " " __TIME__);

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);

  renderTicker.attach_ms(FRAME_PERIOD, render);
  render();
  
  // set firmware info
  Homie.setFirmware(FW_NAME, FW_VERSION);

  stripNode.subscribe("color", colorOnHandler);
  Homie.registerNode(stripNode);

  Homie.setup();
}

void loop() {
  Homie.loop();
}

void render() {
  FastLED.clear();
  random16_set_seed(0);

  Serial.printf("Hue: %u, Sat: %u, Val: %u\n", color.hue, color.sat, color.val);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t hue = color.hue;
    uint8_t sat = color.sat;
    uint8_t val = color.val;    
    leds[i] = CHSV(hue, sat, val);
  }

  FastLED.show();
}
