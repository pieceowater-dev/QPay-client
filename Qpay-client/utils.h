#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

extern const int kaspiQrPin;

void sendKaspiQrSignal(int amount) {
  for (int i = 0; i < amount / 10; i++) {
    digitalWrite(kaspiQrPin, HIGH);
    delay(100);
    digitalWrite(kaspiQrPin, LOW);
    delay(100);
    Serial.println("!!!!!!!!!!!!!!!!!ONE MORE IMPULSE RELEASED!!!!!!!!!!!!!!!!!");
  }
}

#endif // UTILS_H
