//Qpay-client.ino
// 1 - connect to wss
// 2 - auth payload wss with Bearer eyJh...casd123
// 3 - emit "subscribe"
// 4 - listen for "subscribe", "kaspi-pay", "kaspi-check"
// 5 - if "kaspi-check" recieved, send (emit) txn_id back to "kaspi-check" 
// 6 - if "kaspi-pay" recieved, release N*100 impulses, send (emit) txn_id to "kaspi-pay" 
// 7 - if paid with cash, release N*100 impulses, send (emit) "cash-payment" ({sum: "N"})

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIOclient_Generic.h>
#include "wifi_setup.h"
#include "interrupts.h"
#include "socket_events.h"
#include "utils.h"

// Config
const char* SSID = "Pieceowater"; // WiFi name
const char* PASSWORD = "Idontwanttosettheworldonfire"; // WiFi password
const char* DEVICE_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwb3N0SWQiOjksImlhdCI6MTcxNzkyOTMzNCwiZXhwIjoxNzE3OTMyOTM0fQ.pNmXBoXYVESXNckTv5TdbGtjFSxS908T9evzc5yeyzg";
const char* SERVER_HOST = "16.171.58.227";
const uint16_t SERVER_PORT = 80;
const char* SERVER_PATH = "/socket.io/?EIO=4";

const int COIN_ACCEPTOR_PIN = 34;
const int BILL_ACCEPTOR_PIN = 35;
const int KASPI_QR_PIN = 25;
const int GSM_TX_PIN = 14;
const int GSM_RX_PIN = 12;

// Globals
WiFiMulti WiFiMulti;
SocketIOclient socketIO;
volatile int coinCount = 0;
volatile int billCount = 0;
unsigned long messageTimestamp = 0;
DynamicJsonDocument doc(1024);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    delay(200);

    setupWiFi(WiFiMulti, SSID, PASSWORD);
    socketIO.begin(SERVER_HOST, SERVER_PORT, SERVER_PATH, "socket.io");
    socketIO.onEvent(socketIOEvent);
    setupInterrupts(COIN_ACCEPTOR_PIN, BILL_ACCEPTOR_PIN, KASPI_QR_PIN);
    Serial1.begin(9600, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);
}

void loop() {
    socketIO.loop();
    uint64_t now = millis();

    if (now - messageTimestamp > 30000) {
        messageTimestamp = now;

        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        array.add("ping");
        JsonObject param1 = array.createNestedObject();
        param1["now"] = (uint32_t)now;
        String output;
        serializeJson(doc, output);
        socketIO.sendEVENT(output);
        Serial.println(output);
    }

    // Send coin and bill count events
    if (coinCount > 0) {
        Serial.print("Coins inserted: ");
        Serial.println(coinCount);
        sendKaspiQrSignal(coinCount * 10, KASPI_QR_PIN);

        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        array.add("coin-inserted");
        JsonObject param1 = array.createNestedObject();
        param1["count"] = coinCount;
        String output;
        serializeJson(doc, output);
        socketIO.sendEVENT(output);
        coinCount = 0;
    }

    if (billCount > 0) {
        Serial.print("Bills inserted: ");
        Serial.println(billCount);
        sendKaspiQrSignal(billCount * 100, KASPI_QR_PIN);

        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        array.add("bill-inserted");
        JsonObject param1 = array.createNestedObject();
        param1["count"] = billCount;
        String output;
        serializeJson(doc, output);
        socketIO.sendEVENT(output);
        billCount = 0;
    }
}
