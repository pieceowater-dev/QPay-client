//output.h
#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>

// Функция для инициализации вывода
void setupOutput();

// Функция для генерации импульсов на выводе
void sendPulses(int pulseCount);

#endif // OUTPUT_H
