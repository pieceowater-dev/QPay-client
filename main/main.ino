// example: 
//    https://github.com/pieceowater-dev/QPay-client/blob/7e259b440088b357870fe77074eb8b760be79439/Qpay-client/Qpay-client.ino
//    https://github.com/pieceowater-dev/QPay-client/blob/e29e2e27573446343e93ddf16079d9ea9e5450fa/Qpay-client/Qpay-client.ino
//
// workflow:
// 1 - connect to wss / gsm
// 2 - auth payload wss with Bearer eyJh...casd123
// 3 - emit "subscribe"
// 4 - listen for "subscribe", "kaspi-pay", "kaspi-check"
// 5 - if "kaspi-check" recieved, send (emit) txn_id back to "kaspi-check" 
// 6 - if "kaspi-pay" recieved, release N*100 impulses, send (emit) txn_id to "kaspi-pay" 
// 7 - if paid with cash, release N*100 impulses, send (emit) "cash-payment" ({sum: "N"})

//main.ino
#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     4

#include "socket.h"
#include "net.h"
#include "config.h"


void setup() {
  Serial.begin(115200);
  Serial.println("---pieceowater---");

  if (!attemptWiFiConnection()) {
    attemptGSMConnection();
  }

  // connectToWebsocket(); //todo: uncomment later
}

void loop() {
  // loopThroughtWebsocket();
}