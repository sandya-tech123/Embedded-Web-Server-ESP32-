/*
 * ESP32 Embedded Web Server
 * -------------------------
 * Serves a dashboard (from LittleFS) and a small REST API:
 *   GET  /api/status        -> JSON: uptime, heap, RSSI, temperature, LED state
 *   POST /api/led           -> body {"on":true|false}, sets the LED
 *   GET  /api/sensor        -> JSON: temperature (C) + humidity (%) (simulated if no sensor)
 *   GET  /api/wifi          -> JSON: ssid, ip, rssi
 *
 * Board: any ESP32 dev board (Arduino core 2.x / 3.x). Build with PlatformIO.
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"

WebServer server(80);
bool ledState = false;

// ---- Sensor (simulated; replace with DHT22/BME280 reads) ----
float readTemperature() { return 26.0f + 3.0f * sinf(millis() / 20000.0f); }
float readHumidity()    { return 55.0f + 10.0f * cosf(millis() / 30000.0f); }

void sendJson(int code, const JsonDocument &doc) {
  String out;
  serializeJson(doc, out);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "application/json", out);
}

void handleStatus() {
  JsonDocument doc;
  doc["uptime_s"] = millis() / 1000;
  doc["free_heap"] = ESP.getFreeHeap();
  doc["rssi"] = WiFi.RSSI();
  doc["temperature"] = readTemperature();
  doc["led"] = ledState;
  sendJson(200, doc);
}

void handleSensor() {
  JsonDocument doc;
  doc["temperature"] = readTemperature();
  doc["humidity"] = readHumidity();
  sendJson(200, doc);
}

void handleWifi() {
  JsonDocument doc;
  doc["ssid"] = WiFi.SSID();
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  sendJson(200, doc);
}

void handleLed() {
  JsonDocument req;
  if (deserializeJson(req, server.arg("plain")) || !req["on"].is<bool>()) {
    JsonDocument err;
    err["error"] = "body must be {\"on\":true|false}";
    sendJson(400, err);
    return;
  }
  ledState = req["on"];
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  JsonDocument res;
  res["led"] = ledState;
  sendJson(200, res);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  if (!LittleFS.begin(true)) Serial.println("LittleFS mount failed");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(300);
    Serial.print('.');
  }
  if (WiFi.status() != WL_CONNECTED) {  // fall back to access-point mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.printf("\nAP mode: %s  http://%s\n", AP_SSID, WiFi.softAPIP().toString().c_str());
  } else {
    Serial.printf("\nConnected: http://%s\n", WiFi.localIP().toString().c_str());
  }

  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/sensor", HTTP_GET, handleSensor);
  server.on("/api/wifi", HTTP_GET, handleWifi);
  server.on("/api/led", HTTP_POST, handleLed);
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.onNotFound([]() { server.send(404, "text/plain", "Not found"); });
  server.begin();
}

void loop() { server.handleClient(); }
