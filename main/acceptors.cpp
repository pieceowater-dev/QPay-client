#include "acceptors.h"
#include "output.h"

// Переменные для отслеживания количества сигналов от монетоприемника и купюроприемника
volatile int coinSignalCount = 0;
volatile int billSignalCount = 0;

// Время последнего сигнала (для дебаунсинга)
volatile unsigned long lastCoinSignalTime = 0;
volatile unsigned long lastBillSignalTime = 0;

// Минимальное время между сигналами в миллисекундах
const unsigned long debounceDelay = 50; // Можно изменить по необходимости

// Обработчик прерывания для сигналов от монетоприемника
void IRAM_ATTR onCoinSignal() {
  unsigned long currentTime = millis();
  if (currentTime - lastCoinSignalTime > debounceDelay) {
    coinSignalCount++;
    lastCoinSignalTime = currentTime;
  }
}

// Обработчик прерывания для сигналов от купюроприемника
void IRAM_ATTR onBillSignal() {
  unsigned long currentTime = millis();
  if (currentTime - lastBillSignalTime > debounceDelay) {
    billSignalCount++;
    lastBillSignalTime = currentTime;
  }
}

// Настройка пинов для монетоприемника и купюроприемника и привязка прерываний
void setupAcceptors() {
  // Настройка пина монетоприемника как вход с подтягивающим резистором
  pinMode(COIN_ACCEPTOR_PIN, INPUT_PULLUP);
  // Привязка обработчика прерывания на падающий фронт сигнала
  attachInterrupt(digitalPinToInterrupt(COIN_ACCEPTOR_PIN), onCoinSignal, FALLING);

  // Настройка пина купюроприемника как вход с подтягивающим резистором
  pinMode(BILL_ACCEPTOR_PIN, INPUT_PULLUP);
  // Привязка обработчика прерывания на падающий фронт сигнала
  attachInterrupt(digitalPinToInterrupt(BILL_ACCEPTOR_PIN), onBillSignal, FALLING);
}

// Обработка сигналов от монетоприемника и купюроприемника и вывод результатов в консоль
void processAcceptors() {
  // Переменные для отслеживания предыдущего состояния счетчиков сигналов
  static int lastCoinSignalCount = 0;
  static int lastBillSignalCount = 0;

  // Проверка, изменилось ли количество сигналов от монетоприемника
  if (coinSignalCount != lastCoinSignalCount) {
    int coinSignals = coinSignalCount - lastCoinSignalCount;
    float coinValue = coinSignals * SIGNAL_VALUE;
    Serial.print("Coin acceptor signals: ");
    Serial.print(coinSignals);
    Serial.print(" -> Value: ");
    Serial.print(coinValue);
    Serial.println(" Tenge");
    lastCoinSignalCount = coinSignalCount;
    Serial.print("Total coins sig count = ");
    Serial.println(lastCoinSignalCount);
    sendPulses(lastCoinSignalCount);  // Add a semicolon here
  }

  if (billSignalCount != lastBillSignalCount) {
      int billSignals = billSignalCount - lastBillSignalCount;
      float billValue = billSignals * BILL_SIGNAL_VALUE;
      Serial.print("Bill acceptor signals: ");
      Serial.print(billSignals);
      Serial.print(" -> Value: ");
      Serial.print(billValue);
      Serial.println(" Tenge");
      lastBillSignalCount = billSignalCount;
      Serial.print("Total bills sig count = ");
      Serial.println(lastBillSignalCount);
      sendPulses(lastBillSignalCount);  // Add a semicolon here
  }
}
