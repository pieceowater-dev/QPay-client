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
  #error This code is intended to run only on the ESP32 boards! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_ 4

#include "socket.h"
#include "net.h"
#include "config.h"
#include "acceptors.h"
#include "output.h"

void setup() {
  Serial.begin(115200);
  Serial.println("---pieceowater---");

  setupAcceptors();

  if (!attemptWiFiConnection()) {
    attemptGSMConnection();
  }

  connectToWebsocket(); // Uncomment

  // sendPulses(5);
}

void loop() {
  processAcceptors();
  loopThroughtWebsocket();
}

