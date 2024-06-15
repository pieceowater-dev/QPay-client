// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <IPAddress.h>
#include <cstdint>

extern const char SSID[];
extern const char PASS[];

extern const char MOBILE_APN[];
extern const char MOBILE_USER[];
extern const char MOBILE_PASS[];

extern const char* DEVICE_TOKEN;

extern const IPAddress SERVER_IP;
extern const uint16_t SERVER_PORT;

extern const int COIN_ACCEPTOR_PIN;
extern const int BILL_ACCEPTOR_PIN;
extern const int OUTPUT_PIN;
extern const int GSM_TX_PIN;
extern const int GSM_RX_PIN;

#endif // CONFIG_H