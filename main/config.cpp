// config.cpp
#include "config.h"
#include <IPAddress.h>

// WIFI Имя сети и пароль
const char SSID[] = "Pieceowater";
const char PASS[] = "Idontwanttosettheworldonfire";

// Mobile network credentials
const char MOBILE_APN[] = "internet";
const char MOBILE_USER[] = "";  // Опционально
const char MOBILE_PASS[] = "";  // Опционально

// Ключ устройства
const char* DEVICE_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwb3N0SWQiOjIsImlhdCI6MTcyNzYwMDQ5NywiZXhwIjoxNzI3NjA0MDk3fQ.2-5yy_7AV-9JqkSNT10xGE0hGuLymjwwUYQbjKicBbw";

// Адрес вебсокета 51.20.217.61
const IPAddress SERVER_IP(51, 20, 217, 61);
const uint16_t SERVER_PORT = 80;

// Стоимость сигнала в локальной валюте
const float KASPI_SIGNAL_VALUE = 10.0;  // 10 signal = 100 Tenge
const float SIGNAL_VALUE = 10.0;  // 1 signal = 10 Tenge
const float BILL_SIGNAL_VALUE = 100.0;  // 1 signal = 100 Tenge

// Пины
const int COIN_ACCEPTOR_PIN = 34;
const int BILL_ACCEPTOR_PIN = 35;
const int OUTPUT_PIN = 25;
const int GSM_TX_PIN = 14;
const int GSM_RX_PIN = 12;