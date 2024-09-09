//net.cpp
//gsm model SIMKOM A7670E
#include "net.h"
#include <WiFi.h>

// Define the GSM modem model (example: SIM800)
//TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM7000SSL 

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
  
  // Check if the modem is powered and responding
  Serial.println("Sending AT command to check modem...");
  if (!modem.testAT()) {
    Serial.println("Failed to communicate with GSM modem");
    return false;
  }
  
  modem.restart();
  
  if (!modem.waitForNetwork()) {
    Serial.println("GSM network not available");
    return false;
  }

  // Debugging information about APN credentials
  Serial.print("APN: ");
  Serial.println(MOBILE_APN);
  Serial.print("User: ");
  Serial.println(strlen(MOBILE_USER) > 0 ? MOBILE_USER : "none");
  Serial.print("Pass: ");
  Serial.println(strlen(MOBILE_PASS) > 0 ? MOBILE_PASS : "none");

  bool connected;
  if (strlen(MOBILE_USER) > 0 && strlen(MOBILE_PASS) > 0) {
    connected = modem.gprsConnect(MOBILE_APN, MOBILE_USER, MOBILE_PASS);
  } else {
    connected = modem.gprsConnect(MOBILE_APN);
  }

  if (!connected) {
    Serial.println("GPRS connection failed");
    return false;
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
