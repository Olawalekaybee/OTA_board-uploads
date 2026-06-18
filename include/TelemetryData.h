#pragma once

#include <Arduino.h>

struct TelemetryData {
    // Environmental sensors
    float indoorTemperature = 0.0f;
    float indoorHumidity = 0.0f;
    float outdoorTemperature = 0.0f;
    float outdoorHumidity = 0.0f;
    
    // Light sensor
    float lightVoltage = 0.0f;
    uint16_t lightIntensityRaw = 0;
    
    // GPS
    bool gpsValid = false;
    float latitude = 0.0f;
    float longitude = 0.0f;
    uint8_t gpsSatellites = 0;
    
    // Date/Time
    char date[12] = "--";
    char time[12] = "--";
    
    // System status
    bool wifiConnected = false;
    bool firebaseReady = false;
    uint32_t lastUpdate = 0;
    uint32_t timestamp = 0;
    bool dataReceived = false;
};
