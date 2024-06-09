// wifi_setup.h
#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include <WiFiMulti.h>

extern char ssid[];
extern char pass[];

void setupWiFi(WiFiMulti& WiFiMulti, const char* ssid, const char* pass) {
  WiFiMulti.addAP(ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

#endif // WIFI_SETUP_H
