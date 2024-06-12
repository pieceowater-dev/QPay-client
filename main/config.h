// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <IPAddress.h>
#include <cstdint>

// WIFI Имя сети и пароль
extern const char SSID[];
extern const char PASS[];

// Ключ устройства
extern const char* DEVICE_TOKEN;

// Адрес вебсокета
extern const IPAddress SERVER_IP;
extern const uint16_t SERVER_PORT;

// Пины
extern const int COIN_ACCEPTOR_PIN;
extern const int BILL_ACCEPTOR_PIN;
extern const int OUTPUT_PIN;
extern const int GSM_TX_PIN;
extern const int GSM_RX_PIN;

#endif // CONFIG_H