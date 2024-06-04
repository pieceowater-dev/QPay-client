#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_ 4

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

WiFiMulti WiFiMulti;
SocketIOclient socketIO;

// Select the IP address according to your local network
IPAddress serverIP(16, 171, 58, 227);
uint16_t serverPort = 80;

int status = WL_IDLE_STATUS;

char ssid[] = "Pieceowater";       
char pass[] = "Idontwanttosettheworldonfire";    

String eventName;
DynamicJsonDocument doc(1024);

// Define pins for coin and bill acceptor
const int coinAcceptorPin = 34;
const int billAcceptorPin = 35;
const int kaspiQrPin = 25;
const int gsmTxPin = 14;
const int gsmRxPin = 12;

volatile int coinCount = 0;
volatile int billCount = 0;

void IRAM_ATTR handleCoinInterrupt() {
  coinCount++;
}

void IRAM_ATTR handleBillInterrupt() {
  billCount++;
}

void sendKaspiQrSignal(int amount) {
  for (int i = 0; i < amount / 10; i++) {
    digitalWrite(kaspiQrPin, HIGH);
    delay(100);
    digitalWrite(kaspiQrPin, LOW);
    delay(100);
  }
}

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;
    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*) payload);
      deserializeJson(doc, payload, length);
      eventName = doc[0].as<String>();
      if (eventName == "ping") {
        Serial.println("Received ping event");
      } else if (eventName == "subscribe") {
        Serial.println("Received subscribe event");
        String deviceId = doc[1]["deviceId"].as<String>();
        Serial.print("Device ID: ");
        Serial.println(deviceId);
      } else if (eventName == "kaspi-pay") {
        Serial.println("Received kaspi-pay event");
      } else if (eventName == "kaspi-check") {
        Serial.println("Received kaspi-check event");
      }
      break;
    case sIOtype_ACK:
      Serial.print("[IOc] Get ack: ");
      Serial.println(length);
      break;
    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println(length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.print("[IOc] Get binary: ");
      Serial.println(length);
      break;
    case sIOtype_BINARY_ACK:
      Serial.print("[IOc] Get binary ack: ");
      Serial.println(length);
      break;
    case sIOtype_PING:
      Serial.println("[IOc] Get PING");
      break;
    case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");
      break;
    default:
      break;
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Client IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(200);

  Serial.print("\nStart ESP32_WebSocketClientSocketIO on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  WiFiMulti.addAP(ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);
  socketIO.setReconnectInterval(10000);
  socketIO.begin(serverIP, serverPort);
  socketIO.onEvent(socketIOEvent);

  // Set up pins and interrupts for coin and bill acceptor
  pinMode(coinAcceptorPin, INPUT_PULLUP);
  pinMode(billAcceptorPin, INPUT_PULLUP);
  pinMode(kaspiQrPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(coinAcceptorPin), handleCoinInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(billAcceptorPin), handleBillInterrupt, FALLING);

  // Setup UART for GSM module
  Serial1.begin(9600, SERIAL_8N1, gsmRxPin, gsmTxPin);
}

unsigned long messageTimestamp = 0;

void loop() {
  // socketIO.loop();
  // uint64_t now = millis();

  // if (now - messageTimestamp > 30000) {
  //   messageTimestamp = now;

  //   DynamicJsonDocument doc(1024);
  //   JsonArray array = doc.to<JsonArray>();

  //   array.add("ping");
  //   JsonObject param1 = array.createNestedObject();
  //   param1["now"] = (uint32_t) now;
  //   String output;
  //   serializeJson(doc, output);
  //   socketIO.sendEVENT(output);
  //   Serial.println(output);
  // }

  // Send coin and bill count events
  if (coinCount > 0) {
    Serial.print("Coins inserted: ");
    Serial.println(coinCount);
    sendKaspiQrSignal(coinCount * 10);

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
    sendKaspiQrSignal(billCount * 100);

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
