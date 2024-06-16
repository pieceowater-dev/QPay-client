// config.cpp
#include "config.h"
#include <IPAddress.h>

// WIFI Имя сети и пароль
const char SSID[] = "Pieceowater";
const char PASS[] = "Idontwanttosettheworldonfire";

// Mobile network credentials
const char MOBILE_APN[] = "internet.altel.kz";
const char MOBILE_USER[] = "";  // Опционально
const char MOBILE_PASS[] = "";  // Опционально

// Ключ устройства
const char* DEVICE_TOKEN = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwb3N0SWQiOjksImlhdCI6MTcxNzkyOTMzNCwiZXhwIjoxNzE3OTMyOTM0fQ.pNmXBoXYVESXNckTv5TdbGtjFSxS908T9evzc5yeyzg";

// Адрес вебсокета
const IPAddress SERVER_IP(16, 171, 58, 227);
const uint16_t SERVER_PORT = 80;

// Пины
const int COIN_ACCEPTOR_PIN = 34;
const int BILL_ACCEPTOR_PIN = 35;
const int OUTPUT_PIN = 25;
const int GSM_TX_PIN = 14;
const int GSM_RX_PIN = 12;