#include <Homie.h>

#define FW_NAME "relay"
#define FW_VERSION "1.0.1"

const int PIN_RELAY1 = D1;
const int PIN_RELAY2 = D2;

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

HomieNode lightNode("light", "switch");



bool lightOnHandler(String value) {

  Serial.println(value);
  
  if (value == "true") {
    digitalWrite(PIN_RELAY1, HIGH);
    digitalWrite(PIN_RELAY2, HIGH);
    Homie.setNodeProperty(lightNode, "on", "true"); // Update the state of the light
    Serial.println("Light is on");
  } else if (value == "false") {
    digitalWrite(PIN_RELAY1, LOW);
    digitalWrite(PIN_RELAY2, LOW);
    Homie.setNodeProperty(lightNode, "on", "false");
    Serial.println("Light is off");
  } else {
    return false;
  }

  return true;
}

void setup() {

  Serial.begin(9600);
  
  pinMode(PIN_RELAY1, OUTPUT);
  digitalWrite(PIN_RELAY1, LOW);

  pinMode(PIN_RELAY2, OUTPUT);
  digitalWrite(PIN_RELAY2, LOW);
  
  lightNode.subscribe("on", lightOnHandler);
  Homie.registerNode(lightNode);

  Homie.setFirmware(FW_NAME, FW_VERSION);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
