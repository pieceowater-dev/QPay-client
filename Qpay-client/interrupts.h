#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <Arduino.h>

extern volatile int coinCount;
extern volatile int billCount;

const int coinAcceptorPin = 34;
const int billAcceptorPin = 35;
const int kaspiQrPin = 25;

const int gsmTxPin = 14;
const int gsmRxPin = 12;

void IRAM_ATTR handleCoinInterrupt() {
  coinCount++;
}

void IRAM_ATTR handleBillInterrupt() {
  billCount++;
}

void setupInterrupts(int coinPin, int billPin, int qrPin) {
  pinMode(coinPin, INPUT_PULLUP);
  pinMode(billPin, INPUT_PULLUP);
  pinMode(qrPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(coinPin), handleCoinInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(billPin), handleBillInterrupt, FALLING);
}

#endif // INTERRUPTS_H
