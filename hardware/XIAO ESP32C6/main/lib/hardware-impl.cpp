#include "hardware-impl.h"
#include "hardware-config.h"
#include <Arduino.h>

void internal() {
    //Serial.println("Using internal antenna");
    pinMode(ANTENNA_SELECT_PIN, OUTPUT);
    digitalWrite(ANTENNA_SELECT_PIN, LOW);
}

void external() {
    //Serial.println("Using external antenna");
    pinMode(ANTENNA_SELECT_PIN, OUTPUT);
    digitalWrite(ANTENNA_SELECT_PIN, HIGH);
}

void longRange() {
    Serial.println("Using long range mode");
    WiFi.enableLongRange(true);
}