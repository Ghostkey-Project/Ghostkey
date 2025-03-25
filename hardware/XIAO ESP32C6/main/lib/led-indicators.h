#ifndef LED_INDICATORS_H
#define LED_INDICATORS_H

#include "hardware-config.h"

class LEDController {
private:
    // XIAO ESP32C6 has built-in LED on D13
    const uint8_t ledPin = LED_BUILTIN;
    bool ledState = false;
    unsigned long lastBlink = 0;

public:
    LEDController() {
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);
    }

    // Non-blocking blink pattern implementation
    void updateBlink(unsigned long interval) {
        if (millis() - lastBlink >= interval) {
            lastBlink = millis();
            ledState = !ledState;
            digitalWrite(ledPin, ledState);
        }
    }

    // Status indication patterns
    void indicateWiFiConnecting() {
        updateBlink(500); // Slow blink while connecting
    }

    void indicateError(HardwareStatus error) {
        switch(error) {
            case HardwareStatus::WIFI_ERROR:
                updateBlink(100); // Fast blink for WiFi error
                break;
            case HardwareStatus::I2C_ERROR:
                updateBlink(250); // Medium blink for I2C error
                break;
            default:
                digitalWrite(ledPin, HIGH); // Solid on for other errors
                break;
        }
    }

    void indicateNormal() {
        digitalWrite(ledPin, LOW); // LED off during normal operation
    }
};

#endif // LED_INDICATORS_H