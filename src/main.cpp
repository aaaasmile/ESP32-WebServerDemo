#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <AsyncJson.h>
#include "secret.h"

AsyncServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booted");
}

void loop() {
}