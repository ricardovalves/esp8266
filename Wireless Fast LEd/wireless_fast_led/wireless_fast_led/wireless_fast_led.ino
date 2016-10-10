#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "Ricardo Alves";
const char* password = "2079A22DE1";

#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN D1
#define CLOCK_PIN D2

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
            
      char* incoming = (char *) &payload[0];
      Serial.printf("Received payload: %s\n", incoming);
      
      char* token = strtok(incoming, ",");
      int red = atoi(token);
      Serial.printf("RED: %i ", red);

      token = strtok(NULL, ",");
      int green = atoi(token);
      Serial.printf("GREEN: %i ", green);

      token = strtok(NULL, ",");
      int blue = atoi(token);
      Serial.printf("BLUE: %i ", blue);

      token = strtok(NULL, ",");
      int brightness = atoi(token);
      Serial.printf("BRIGHTNESS: %i\n", brightness);

      FastLED.clear();
      fillLedStrip(red, green, blue, brightness);
            
      break;
    }
  }
}

void setup() {
  
  Serial.begin(9600);
  Serial.println("Connecting...");
  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println("Attempting to connect...");
  }
  
  Serial.print("Connected to: ");
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(255);
  // clear
  FastLED.clear();

  fillLedStrip(255, 0, 0, 255);
}

void loop() {
  webSocket.loop();
}

void fillLedStrip(int red, int green, int blue, int brightness) {
    
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB(red, green, blue);
  }
  FastLED.setBrightness(brightness);
  FastLED.show();  
}

