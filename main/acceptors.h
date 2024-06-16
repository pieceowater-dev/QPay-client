//acceptors.h
#ifndef ACCEPTORS_H
#define ACCEPTORS_H

#include <Arduino.h>
#include "config.h"

// Function prototypes
void setupAcceptors();
void processAcceptors();

// Interrupt service routines
void IRAM_ATTR onCoinSignal();
void IRAM_ATTR onBillSignal();

// External variables to track signal counts
extern volatile int coinSignalCount;
extern volatile int billSignalCount;

#endif // ACCEPTORS_H
