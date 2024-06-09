// socket_events.h
#include <ArduinoJson.h>
#include <SocketIOclient_Generic.h>

extern SocketIOclient socketIO;
extern const char* DEVICE_TOKEN;
extern const int KASPI_QR_PIN;
extern DynamicJsonDocument doc;

void sendKaspiQrSignal(int amount, int pin);
void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case sIOtype_CONNECT:
            socketIO.send(sIOtype_CONNECT, "/");
            break;

        case sIOtype_DISCONNECT:
            Serial.println("[IOc] Disconnected!");
            break;

        case sIOtype_EVENT: {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload, length);
            String eventName = doc[0].as<String>();

            if (eventName == "kaspi-check") {
                String txn_id = doc[1]["txn_id"].as<String>();
                DynamicJsonDocument doc(1024);
                JsonArray array = doc.to<JsonArray>();
                array.add("kaspi-check");
                JsonObject param1 = array.createNestedObject();
                param1["txn_id"] = txn_id;
                String output;
                serializeJson(doc, output);
                socketIO.sendEVENT(output);
            } else if (eventName == "kaspi-pay") {
                String txn_id = doc[1]["txn_id"].as<String>();
                int amount = doc[1]["sum"].as<int>();
                sendKaspiQrSignal(amount / 10, KASPI_QR_PIN);
                DynamicJsonDocument doc(1024);
                JsonArray array = doc.to<JsonArray>();
                array.add("kaspi-pay");
                JsonObject param1 = array.createNestedObject();
                param1["txn_id"] = txn_id;
                String output;
                serializeJson(doc, output);
                socketIO.sendEVENT(output);
            }
            break;
        }

        case sIOtype_ACK: {
            // Handle ACK type
            break;
        }

        case sIOtype_ERROR: {
            // Handle ERROR type
            break;
        }

        case sIOtype_BINARY_EVENT: {
            // Handle BINARY_EVENT type
            break;
        }

        case sIOtype_BINARY_ACK: {
            // Handle BINARY_ACK type
            break;
        }

        case sIOtype_PING: {
            socketIO.send(sIOtype_PONG, "");
            break;
        }

        case sIOtype_PONG: {
            // Handle PONG type
            break;
        }

        default:
            break;
    }
}
