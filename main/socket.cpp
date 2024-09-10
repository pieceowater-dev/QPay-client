//socket.cpp
#include "socket.h"
#include "config.h"
#include "output.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

SocketIOclient socketIO;

unsigned long messageTimestamp = 0;


void emitCashPaymentEvent(int sum) {
  // Create a JSON object
  DynamicJsonDocument doc(1024);
  JsonObject eventData = doc.to<JsonObject>();
  
  // Add sum as a string to the JSON object
  eventData["sum"] = String(sum);

  // Convert the JSON object to a string
  String output;
  serializeJson(doc, output);

  // Create the event message in the required SocketIO format
  String message = "42[\"cash-payment\"," + output + "]";

  // Emit the event
  socketIO.send(sIOtype_EVENT, message.c_str());

  // Debug: Print the event that is sent
  Serial.println("Emitting cash-payment event: " + message);
}

void handleKaspiCheck(JsonObject& eventData, uint8_t* payload) {
  // Handle kaspi-check event
  socketIO.send(sIOtype_EVENT, payload);
  Serial.println("Handling kaspi-check event");
}

void handleKaspiPay(JsonObject& eventData, uint8_t* payload) {
  // Handle kaspi-pay event
  Serial.println("Handling kaspi-pay event");

  // Extract the pulse count (N) from the eventData object
  // Check if "sum" is a string and convert it to an integer
  String sumString = eventData["sum"] | "0";  // Extract sum as a string
  int sum = sumString.toInt();  // Convert sum from string to integer

  int pulseCount = sum / 10;  // Divide sum by 100 to get pulseCount
  String txn_id = eventData["txn_id"] | "";  // Extract txn_id from the eventData object

  if (pulseCount > 0) {
    // Call sendPulses
    int totalPulses = pulseCount;
    sendPulses(totalPulses);
    Serial.print("Sent ");
    Serial.print(totalPulses);
    Serial.println(" pulses.");

    // Emit txn_id back to the server in a "kaspi-pay" event acknowledgment
    DynamicJsonDocument doc(256);
    JsonArray array = doc.to<JsonArray>();

    // Add the event name and payload to the JSON array
    array.add("kaspi-pay");
    JsonObject payloadObj = array.createNestedObject();
    payloadObj["txn_id"] = txn_id;

    // Serialize the JSON array to a string
    String output;
    serializeJson(doc, output);

    // Send the event with txn_id back to the server
    // socketIO.sendEVENT(output);
    socketIO.send(sIOtype_EVENT, payload);
    Serial.print("Acknowledgment sent for txn_id: ");
    Serial.println(txn_id);
  } else {
    Serial.println("Invalid pulse count, skipping pulse send.");
  }
}

void handleSocketEvent(uint8_t* payload, size_t length) {
  Serial.print("[IOc] Handing event: ");
  Serial.println((char*)payload);
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  if (!doc.is<JsonArray>()) {
    Serial.println("Expected JSON array");
    return;
  }

  JsonArray array = doc.as<JsonArray>();
  if (array.size() < 2) {
    Serial.println("Invalid event format");
    return;
  }

  String eventName = array[0].as<String>();
  JsonObject eventData = array[1].as<JsonObject>();

  if (eventName == "kaspi-check") {
    handleKaspiCheck(eventData, payload);
  } else if (eventName == "kaspi-pay") {
    handleKaspiPay(eventData, payload);
  } else if (eventName == "subscribe") {
     Serial.println("Subscribed!");
  } else if (eventName == "ping") {
     Serial.println("I said ping, server said pong!");
  } else {
    Serial.print("Unknown event: ");
    Serial.println(eventName);
  }
}

void connectToWebsocket() {
  Serial.println("WebSockets Client started");
  Serial.print("Connecting to: ");
  Serial.print(SERVER_IP);
  Serial.print(", port: ");
  Serial.println(SERVER_PORT);

  socketIO.setReconnectInterval(10000);

  String authorizationHeader = "Authorization: Bearer ";
  authorizationHeader += DEVICE_TOKEN;
  socketIO.setExtraHeaders(authorizationHeader.c_str());

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
        socketIO.send(sIOtype_CONNECT, "40");
        socketIO.send(sIOtype_EVENT, "42[\"subscribe\"]");
        break;
      case sIOtype_EVENT:
        handleSocketEvent(payload, length);
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