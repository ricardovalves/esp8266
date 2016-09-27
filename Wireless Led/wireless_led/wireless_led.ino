#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "ssid";
const char* password = "password";

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
      String text = String((char *) &payload[0]);
      if (text == "ON") {
        digitalWrite(D4, LOW);
      }
      else if (text == "OFF") {
        digitalWrite(D4, HIGH);
      }
      break;
    }
  }
}

void setup() {

  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
  
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
}

void loop() {
  webSocket.loop();
}

