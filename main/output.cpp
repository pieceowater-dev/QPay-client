//output.cpp
#include "output.h"
#include "config.h"

// Инициализация вывода
void setupOutput() {
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, LOW);
}

// Функция для генерации импульсов на выводе
void sendPulses(int pulseCount) {
    const int pulseDuration = 100; // Длительность импульса в миллисекундах
    const int pauseDuration = 100; // Длительность паузы между импульсами в миллисекундах

    for (int i = 0; i < pulseCount; ++i) {
        digitalWrite(OUTPUT_PIN, HIGH);
        delay(pulseDuration);
        digitalWrite(OUTPUT_PIN, LOW);
        delay(pauseDuration);

        // Печать сообщения после каждого импульса
        Serial.print("Impulse ");
        Serial.print(i + 1);
        Serial.println(" sent");
    }
}
