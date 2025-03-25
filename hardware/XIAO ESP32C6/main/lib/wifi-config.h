#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include "hardware-config.h"
#include "led-indicators.h"

class WiFiManager {
private:
    LEDController& led;
    static constexpr unsigned long TIMEOUT = 30000; // 30 second timeout
    static constexpr unsigned long RETRY_INTERVAL = 5000; // 5 second retry interval

public:
    WiFiManager(LEDController& ledController) : led(ledController) {}

    bool connect(const char* ssid, const char* password) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
            if (millis() - startTime > TIMEOUT) {
                led.indicateError(HardwareStatus::WIFI_ERROR);
                return false;
            }
            led.indicateWiFiConnecting();
            delay(100);
        }

        led.indicateNormal();
        return true;
    }

    bool maintainConnection(const char* ssid, const char* password) {
        static unsigned long lastCheck = 0;
        
        if (millis() - lastCheck >= RETRY_INTERVAL) {
            lastCheck = millis();
            if (WiFi.status() != WL_CONNECTED) {
                return connect(ssid, password);
            }
        }
        return WiFi.status() == WL_CONNECTED;
    }

    IPAddress getLocalIP() {
        return WiFi.localIP();
    }
};

#endif // WIFI_CONFIG_H