//Qpay-client.ino
// 1 - connect to wss
// 2 - auth payload wss with Bearer eyJh...casd123
// 3 - emit "subscribe"
// 4 - listen for "subscribe", "kaspi-pay", "kaspi-check"
// 5 - if "kaspi-check" recieved, send (emit) txn_id back to "kaspi-check" 
// 6 - if "kaspi-pay" recieved, release N*100 impulses, send (emit) txn_id to "kaspi-pay" 
// 7 - if paid with cash, release N*100 impulses, send (emit) "cash-payment" ({sum: "N"})

#include <Arduino.h>
#include "wifi_manager.h"
#include "websocket_manager.h"
#include "message_manager.h"

// WiFi credentials
const char* SSID = "Pieceowater";
const char* PASSWORD = "Idontwanttosettheworldonfire";

// Authorization token
const char* DEVICE_TOKEN = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwb3N0SWQiOjksImlhdCI6MTcxNzkyOTMzNCwiZXhwIjoxNzE3OTMyOTM0fQ.pNmXBoXYVESXNckTv5TdbGtjFSxS908T9evzc5yeyzg";

// WebSocket server settings
const char* SERVER_HOST = "16.171.58.227";
const uint16_t SERVER_PORT = 80;
const char* SERVER_PATH = "/socket.io/?EIO=4";

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(200);

  // Инициализация модулей
  initWiFi(SSID, PASSWORD);
  initWebSocket();
  // initMessageManager();
}


void loop() {
  // Обработка событий веб-сокета
  webSocketLoop();
  // Обработка событий сообщений
  // handleMessage();
}

