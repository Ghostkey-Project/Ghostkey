#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "hardware-config.h"

class BLEManager {
private:
    BLEServer* pServer = nullptr;
    BLECharacteristic* pCharacteristic = nullptr;
    bool deviceConnected = false;
    
    // BLE service and characteristic UUIDs
    static constexpr const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    static constexpr const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

public:
    void init(const char* deviceName) {
        BLEDevice::init(deviceName);
        pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        
        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY
        );
        
        pCharacteristic->addDescriptor(new BLE2902());
        pService->start();
        
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

    void sendNotification(const char* message) {
        if (pCharacteristic != nullptr) {
            pCharacteristic->setValue((uint8_t*)message, strlen(message));
            pCharacteristic->notify();
        }
    }
};

#endif // BLE_CONFIG_H