#ifndef SOCKET_EVENTS_H
#define SOCKET_EVENTS_H

#include <SocketIOclient_Generic.h>
#include <ArduinoJson.h>
#include "utils.h"

extern char deviceToken[];
extern DynamicJsonDocument doc;
extern String eventName;
extern SocketIOclient socketIO;

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;
    case sIOtype_CONNECT: {
      socketIO.send(sIOtype_CONNECT, "/");

      // Authenticate with Bearer token
      DynamicJsonDocument authPayload(1024);
      JsonObject authObject = authPayload.to<JsonObject>();
      authObject["token"] = "Bearer " + String(deviceToken);
      String output;
      serializeJson(authPayload, output);
      socketIO.sendEVENT(output);
      Serial.println("Sent auth payload: " + output);

      // Emit subscribe event
      // DynamicJsonDocument subscribePayload(1024);
      // JsonArray subscribeArray = subscribePayload.to<JsonArray>();
      // subscribeArray.add("subscribe");
      // String subscribeOutput;
      // serializeJson(subscribePayload, subscribeOutput);
      // socketIO.sendEVENT(subscribeOutput);
      // Serial.println("Sent subscribe event: " + subscribeOutput);
      }
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
        // Release N*100 impulses and send txn_id to "kaspi-pay"
        int amount = doc[1]["sum"].as<int>();
        sendKaspiQrSignal(amount / 10);
        DynamicJsonDocument response(1024);
        JsonArray array = response.to<JsonArray>();
        array.add("kaspi-pay");
        JsonObject param1 = array.createNestedObject();
        param1["txn_id"] = doc[1]["txn_id"];
        String output;
        serializeJson(response, output);
        socketIO.sendEVENT(output);
      } else if (eventName == "kaspi-check") {
        Serial.println("Received kaspi-check event");
        // Send txn_id back to "kaspi-check"
        DynamicJsonDocument response(1024);
        JsonArray array = response.to<JsonArray>();
        array.add("kaspi-check");
        JsonObject param1 = array.createNestedObject();
        param1["txn_id"] = doc[1]["txn_id"];
        String output;
        serializeJson(response, output);
        socketIO.sendEVENT(output);
      }
      break;
    case sIOtype_ACK:
      // Serial.print("[IOc] Get ack: ");
      // Serial.println(length);
      break;
    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println((char*) payload);
      break;
    case sIOtype_BINARY_EVENT:
      // Serial.print("[IOc] Get binary: ");
      // Serial.println(length);
      break;
    case sIOtype_BINARY_ACK:
      // Serial.print("[IOc] Get binary ack: ");
      // Serial.println(length);
      break;
    case sIOtype_PING:
      socketIO.send(sIOtype_PONG, "");
      Serial.println("Sent PONG in response to PING");
      break;
    case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");
      break;
    default:
      break;
  }
}

void setupSocketIO(SocketIOclient& socketIO, IPAddress& serverIP, uint16_t serverPort, 
                   std::function<void(const socketIOmessageType_t&, uint8_t*, const size_t&)> callback) {
  socketIO.setReconnectInterval(30000);
  socketIO.begin(serverIP, serverPort);
  socketIO.onEvent(callback);

  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);
}

#endif // SOCKET_EVENTS_H
