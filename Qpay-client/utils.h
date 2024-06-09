// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

void sendKaspiQrSignal(int amount, int pin) {
    for (int i = 0; i < amount / 10; i++) {
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
        delay(100);
        Serial.println("!!!!!!!!!!!!!!!!!ONE MORE IMPULSE RELEASED!!!!!!!!!!!!!!!!!");
    }
}

#endif // UTILS_H
