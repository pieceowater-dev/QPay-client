//net.h
#ifndef NET_H
#define NET_H

#include <Arduino.h>

bool attemptWiFiConnection();
String getWiFiStatus();
bool attemptGSMConnection();
String getGSMStatus();

#endif // NET_H
