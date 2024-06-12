//socket.cpp
#include "socket.h"
#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

SocketIOclient socketIO;

unsigned long messageTimestamp = 0;

void connectToWebsocket() {
  Serial.println("WebSockets Client started");
  Serial.print("Connecting to: ");
  Serial.print(SERVER_IP);
  Serial.print(", port: ");
  Serial.println(SERVER_PORT);

  socketIO.setReconnectInterval(10000);

  // String authorizationHeader = "Authorization: Bearer "; //something wrong here
  // authorizationHeader += DEVICE_TOKEN;
  // socketIO.setExtraHeaders(authorizationHeader.c_str());

  socketIO.begin(SERVER_IP, SERVER_PORT);

  socketIO.onEvent([](socketIOmessageType_t type, uint8_t* payload, size_t length) {
    switch (type) {
      case sIOtype_DISCONNECT:
        Serial.println("[IOc] Disconnected");
        break;

      case sIOtype_CONNECT:
        Serial.print("[IOc] Connected to url: ");
        Serial.println((char*)payload);

        // join default namespace (no auto join in Socket.IO V3)
        socketIO.send(sIOtype_CONNECT, "/");
        break;

      case sIOtype_EVENT:
        Serial.print("[IOc] Got event: ");
        Serial.println((char*)payload);
        break;

      case sIOtype_ACK:
        Serial.print("[IOc] Get ack: ");
        Serial.println(length);
        //hexdump(payload, length);
        break;

      case sIOtype_ERROR:
        Serial.print("[IOc] Get error: ");
        Serial.println(length);
        //hexdump(payload, length);
        break;

      case sIOtype_BINARY_EVENT:
        Serial.print("[IOc] Get binary: ");
        Serial.println(length);
        //hexdump(payload, length);
        break;

      case sIOtype_BINARY_ACK:
        Serial.print("[IOc] Get binary ack: ");
        Serial.println(length);
        //hexdump(payload, length);
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
  });
}

void loopThroughtWebsocket() {
  socketIO.loop();

  uint64_t now = millis();

  if (now - messageTimestamp > 30000)
  {
    messageTimestamp = now;

    // creat JSON message for Socket.IO (event)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("ping");

    // add payload (parameters) for the event
    JsonObject param1 = array.createNestedObject();
    param1["now"]     = (uint32_t) now;

    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);

    // Send event
    socketIO.sendEVENT(output);

    // Print JSON for debugging
    Serial.println(output);
  }
}