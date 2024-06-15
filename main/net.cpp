//net.spp
#include "net.h"
#include <WiFi.h>

// Define the GSM modem model (example: SIM800)
#define TINY_GSM_MODEM_SIM800

// Include TinyGsmClient after defining the modem model
#include <TinyGsmClient.h>
#include "config.h"

// Use HardwareSerial for ESP32 or other boards
HardwareSerial SerialGSM(1);

// Initialize GSM modem
TinyGsm modem(SerialGSM);

bool attemptWiFiConnection() {
  int attempts = 0;
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED && attempts < 3) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    String status = getWiFiStatus();
    Serial.println(status);
    return true;
  } else {
    Serial.println("WiFi connection failed");
    return false;
  }
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

bool attemptGSMConnection() {
  Serial.println("Initializing GSM...");
  modem.restart();

  if (!modem.waitForNetwork()) {
    Serial.println("GSM network not available");
    return false;
  }

  if (strlen(MOBILE_USER) > 0 && strlen(MOBILE_PASS) > 0) {
    if (!modem.gprsConnect(MOBILE_APN, MOBILE_USER, MOBILE_PASS)) {
      Serial.println("GPRS connection failed");
      return false;
    }
  } else {
    if (!modem.gprsConnect(MOBILE_APN)) {
      Serial.println("GPRS connection failed");
      return false;
    }
  }

  String status = getGSMStatus();
  Serial.println(status);
  return true;
}

String getGSMStatus() {
  if (modem.isNetworkConnected()) {
    return "GSM Connected";
  } else {
    return "Not Connected";
  }
}
