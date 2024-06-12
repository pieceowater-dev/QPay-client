//net.cpp
#include "net.h"
#include <WiFi.h>
#include "config.h"
#include <Arduino.h>

void connectToWiFi() {
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  String status = getWiFiStatus();
  Serial.println(status);
}

String getWiFiStatus() {
  wl_status_t status = WiFi.status();

  if (status == WL_CONNECTED) {
    String ipAddress = WiFi.localIP().toString();
    return "WiFi Connected, IP Address: " + ipAddress;
  } else {
    return "Not Connected";
  }
}