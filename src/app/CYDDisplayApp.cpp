#include "app/CYDDisplayApp.h"

extern TFT_eSPI tft;

void CYDDisplayApp::begin() {
    Serial.println("[CYDDisplayApp] Starting...");
    tft.fillScreen(0x0844);  // Navy background
    
    tft.setTextColor(0x07FF, 0x0844);  // Cyan on navy
    tft.setTextFont(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("CYD Display Ready", 160, 120);
    
    Serial.println("[CYDDisplayApp] ✓ Ready");
}

void CYDDisplayApp::update() {
    unsigned long now = millis();
    
    // Update display periodically
    if (now - lastRender >= RENDER_INTERVAL_MS) {
        lastRender = now;
        
        // Draw some basic status
        tft.fillRect(0, 200, 320, 40, 0x0844);
        tft.setTextColor(0xFFFF, 0x0844);
        tft.setTextFont(1);
        tft.setCursor(10, 210);
        tft.printf("Uptime: %lu s", now / 1000);
    }
}
