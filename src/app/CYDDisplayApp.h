#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class CYDDisplayApp {
public:
    void begin();
    void update();
    
private:
    unsigned long lastRender = 0;
    static constexpr unsigned long RENDER_INTERVAL_MS = 2000;
};
