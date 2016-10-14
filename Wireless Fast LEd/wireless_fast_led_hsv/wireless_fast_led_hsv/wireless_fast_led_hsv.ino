#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>
#include <Ticker.h>
#include "Utils.h"

#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "Ricardo Alves";
const char* password = "2079A22DE1";

#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN D1
#define CLOCK_PIN D2

#define FPS 15
#define FRAME_PERIOD (1000 / FPS)

CHSV color(0, 255, 255);
uint8_t hueRange = 10;
uint8_t satRange = 100;

Ticker renderTicker;

CRGB leds[NUM_LEDS];

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED: {
      break;
    }
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.print("IP Address: ");
      Serial.printf("Connected from %d.%d.%d.%d url: %s\n", ip[0], ip[1], ip[2], ip[3], payload);
      break;
    }
    case WStype_TEXT: {
            
      char *incoming = (char *) &payload[0];
      Serial.printf("Received payload: %s\n", incoming);

      color = toHSV(incoming);                 
      break;
    }
  }
}

void setup() {
  
  Serial.begin(9600);
  Serial.printf("Connecting...\n");
  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.printf("Attempting to connect...\n");
  }
  
  Serial.print("Connected to: ");
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  
  renderTicker.attach_ms(FRAME_PERIOD, render);

  // call first render
  render();
}

inline uint8_t flunctuate(uint8_t value, uint8_t range, bool wrap) {
  uint8_t s = beatsin8(random8(1, 20), 0, 2 * range, 0, random8());
  if (wrap) {
    return value + s - range;
  } else {
    int shift = (int)s - (int)range;
    int newValue = value + shift;
    if (newValue < 0) {
      return 0;
    } else if (newValue > 255) {
      return 255;
    } else {
      return newValue;
    }
  }
}

void loop() {
  webSocket.loop();
}

void render() {
  FastLED.clear();
  random16_set_seed(0);

  uint8_t hue = flunctuate(color.hue, hueRange, true);
  uint8_t sat = flunctuate(color.sat, satRange, false);
  uint8_t val = color.val;
  for (int i = 0; i < NUM_LEDS; i++) {      
    leds[i] = CHSV(hue, sat, val);
  }
  FastLED.show();  
}
