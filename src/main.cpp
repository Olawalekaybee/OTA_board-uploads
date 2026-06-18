

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <TFT_eSPI.h>

// Include display and animation
#include "display/OTABootAnimation.h"
#include "app/CYDDisplayApp.h"

// ═══════════════════════════════════════════════════════════════
//  GLOBAL OBJECTS
// ═══════════════════════════════════════════════════════════════
TFT_eSPI tft = TFT_eSPI();  // 320x240 ILI9341
OTABootAnimation otaAnim(tft);
CYDDisplayApp displayApp;

// ═══════════════════════════════════════════════════════════════
//  OTA CALLBACK HANDLERS
// ═══════════════════════════════════════════════════════════════

void onOTAStart() {
    Serial.println("\n[OTA] Update started!");
    
    // Transition to OTA animation
    otaAnim.begin();
    otaAnim.setState(OTA_INITIALIZING);
    
    Serial.println("[OTA] Showing boot animation sequence...");
}

void onOTAProgress(unsigned int progress, unsigned int total) {
    static uint32_t lastUpdate = 0;
    
    uint8_t percent = (progress * 100) / total;
    
    // Update progress every 5% to reduce Serial spam
    if (percent % 5 == 0 && (millis() - lastUpdate) > 500) {
        lastUpdate = millis();
        Serial.printf("[OTA] Progress: %d%% (%u / %u bytes)\n", 
                      percent, progress, total);
    }
    
    // Determine phase
    if (percent < 40) {
        otaAnim.setState(OTA_DOWNLOADING);
    } else if (percent < 85) {
        otaAnim.setState(OTA_FLASHING);
    } else {
        otaAnim.setState(OTA_VERIFYING);
    }
    
    otaAnim.setProgress(percent);
    
    // Keep animation smooth
    otaAnim.update();
    yield();
}

void onOTAEnd() {
    Serial.println("\n[OTA] Update complete!");
    
    otaAnim.setState(OTA_COMPLETE);
    
    // Show success screen for 3 seconds
    uint32_t endTime = millis() + 3000;
    while (millis() < endTime) {
        otaAnim.update();
        yield();
    }
    
    Serial.println("[OTA] Rebooting device...");
    delay(500);
    ESP.restart();
}

void onOTAError(ota_error_t error) {
    Serial.printf("\n[OTA] Error[%u]: ", error);
    
    const char* errorMsg = "Unknown error";
    
    if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth failed");
        errorMsg = "Authentication failed";
    } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin failed");
        errorMsg = "OTA begin failed";
    } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect failed");
        errorMsg = "Server connection failed";
    } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive failed");
        errorMsg = "Data receive failed";
    } else if (error == OTA_END_ERROR) {
        Serial.println("End failed");
        errorMsg = "Flash write error";
    }
    
    otaAnim.setError(errorMsg);
    
    // Show error screen for 10 seconds
    uint32_t errorTime = millis() + 10000;
    while (millis() < errorTime) {
        otaAnim.update();
        yield();
    }
    
    Serial.println("[OTA] Rebooting device...");
    delay(500);
    ESP.restart();
}

// ═══════════════════════════════════════════════════════════════
//  SETUP - Initialize everything
// ═══════════════════════════════════════════════════════════════
void setup() {
    // Serial for debugging
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n═══════════════════════════════════════════════════");
    Serial.println("  AgriNode CYD Display + OTA Animation");
    Serial.println("  PlatformIO ESP32 Build");
    Serial.println("═══════════════════════════════════════════════════\n");
    
    // ── Initialize TFT display ──────────────────────────────────
    Serial.println("[Setup] Initializing TFT display...");
    tft.init();
    tft.setRotation(1);  // 320x240 landscape
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("[Setup] ✓ TFT initialized");
    
    // ── Start WiFi in background (non-blocking) ─────────────────
    Serial.println("[Setup] Starting WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(true);
    
    // Replace with your Secrets.h credentials:
    const char* WIFI_SSID = "YOUR_SSID";
    const char* WIFI_PASS = "YOUR_PASSWORD";
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("[Setup] ✓ WiFi connection started (non-blocking)");
    
    // ── Setup ArduinoOTA ────────────────────────────────────────
    Serial.println("[Setup] Configuring OTA...");
    
    ArduinoOTA.setHostname("agrinode-cyd");
    ArduinoOTA.setPassword("admin");  // Change this!
    
    ArduinoOTA.onStart(onOTAStart);
    ArduinoOTA.onProgress(onOTAProgress);
    ArduinoOTA.onEnd(onOTAEnd);
    ArduinoOTA.onError(onOTAError);
    
    // Only start OTA if WiFi connects (will be activated from loop)
    Serial.println("[Setup] ✓ OTA ready (will start when WiFi connects)");
    
    // ── Show welcome splash ─────────────────────────────────────
    tft.fillScreen(0x0844);  // Navy background
    tft.setTextColor(0x07FF, 0x0844);  // Cyan text
    tft.setTextFont(4);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("AgriNode", 160, 80);
    
    tft.setTextFont(2);
    tft.setTextColor(0x7BEF, 0x0844);  // Grey text
    tft.drawString("CYD Display Node", 160, 130);
    
    tft.setTextFont(1);
    tft.setTextColor(0xFFFF, 0x0844);  // White text
    tft.drawString("Initializing...", 160, 180);
    
    delay(2000);
    
    // ── Initialize display app ──────────────────────────────────
    Serial.println("[Setup] Initializing display application...");
    displayApp.begin();
    Serial.println("[Setup] ✓ Display app ready");
    
    Serial.println("\n═══════════════════════════════════════════════════");
    Serial.println("  Setup complete! System running...");
    Serial.println("═══════════════════════════════════════════════════\n");
}

// ═══════════════════════════════════════════════════════════════
//  LOOP - Main execution
// ═══════════════════════════════════════════════════════════════
void loop() {
    static uint32_t lastOTACheck = 0;
    static bool otaStarted = false;
    
    // ── Check if WiFi is connected and OTA not yet initialized ──
    if (!otaStarted && WiFi.status() == WL_CONNECTED) {
        Serial.print("[WiFi] Connected! IP: ");
        Serial.println(WiFi.localIP());
        
        ArduinoOTA.begin();
        otaStarted = true;
        Serial.println("[OTA] Ready for firmware updates");
    }
    
    // ── Handle OTA updates (every 100 ms) ───────────────────────
    if (millis() - lastOTACheck >= 100) {
        lastOTACheck = millis();
        ArduinoOTA.handle();
    }
    
    // ── Update display app ──────────────────────────────────────
    displayApp.update();
    
    // Small delay to prevent watchdog timeout
    yield();
    delay(1);
}

