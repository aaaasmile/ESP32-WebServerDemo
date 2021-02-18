#include <Arduino.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

#include "secret.h"

#define BUILTIN_LED 2

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booted");

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed to connect to the WiFi. Restart the micro");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Wifi connected");
  Serial.println(WiFi.localIP());
  
  SPIFFS.begin();

  MDNS.begin("MySt32-Web");
  pinMode(BUILTIN_LED, OUTPUT);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.addHandler(new AsyncCallbackJsonWebHandler("/led", [](AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject &jsonObj = json.as<JsonObject>();
    if (jsonObj["on"]) {
      Serial.println("Turn on LED");
      digitalWrite(BUILTIN_LED, HIGH);
    } else {
      Serial.println("Turn off LED");
      digitalWrite(BUILTIN_LED, LOW);
    }
    request->send(200, "OK");
  }));

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      Serial.println("Not found");
      request->send(404, "Not found");
    }
  });

  server.begin();
}

void loop() {
}