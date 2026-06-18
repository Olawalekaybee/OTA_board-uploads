#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

// ═══════════════════════════════════════════════════════════════
//  OTA BOOT ANIMATION STATES
// ═══════════════════════════════════════════════════════════════
enum OTAState {
    OTA_INITIALIZING,           // Scanning network
    OTA_CONNECTING_SERVER,      // Connecting to update server
    OTA_DOWNLOADING,            // Downloading firmware
    OTA_FLASHING,               // Writing to flash
    OTA_VERIFYING,              // Verifying checksum
    OTA_COMPLETE,               // Success
    OTA_ERROR                   // Failed
};

class OTABootAnimation
{
public:
    // Initialize with TFT_eSPI reference
    OTABootAnimation(TFT_eSPI& tft);

    // Call this from setup() to start the boot sequence
    void begin();

    // Call this from loop() to update animation frames
    // Returns true if animation is still running
    bool update();

    // Transition to next state
    void setState(OTAState newState);

    // Set progress (0-100) for download/flash states
    void setProgress(uint8_t percent);

    // Set error message
    void setError(const char* message);

    // Check if animation is done
    bool isComplete() const { return _state == OTA_COMPLETE || _state == OTA_ERROR; }

    // Get current state
    OTAState getState() const { return _state; }

private:
    TFT_eSPI& tft;

    // ── Animation state ────────────────────────────────────────
    OTAState _state = OTA_INITIALIZING;
    OTAState _lastState = (OTAState)-1;      // detect transitions
    
    uint8_t  _progress = 0;                  // 0-100 for progress states
    uint32_t _frameTimer = 0;                // animation frame timer
    uint8_t  _frameCount = 0;                // cycles for spinner/pulse
    
    const char* _errorMsg = "";

    // ── State timings ──────────────────────────────────────────
    static constexpr uint32_t FRAME_INTERVAL = 50;    // 50 ms per frame = ~20 fps

    // ── Draw zones (absolute pixel positions) ──────────────────
    static constexpr int ZONE_HDR_Y    = 0;
    static constexpr int ZONE_HDR_H    = 50;
    static constexpr int ZONE_ANIM_Y   = 50;
    static constexpr int ZONE_ANIM_H   = 70;
    static constexpr int ZONE_PROG_Y   = 120;
    static constexpr int ZONE_PROG_H   = 40;
    static constexpr int ZONE_STAT_Y   = 160;
    static constexpr int ZONE_STAT_H   = 80;

    // ── Colour palette ─────────────────────────────────────────
    static constexpr uint16_t C_BG         = 0x0844;   // Navy
    static constexpr uint16_t C_HDR        = 0x0887;   // Header navy
    static constexpr uint16_t C_ACCENT     = 0x07FF;   // Cyan
    static constexpr uint16_t C_WHITE      = 0xFFFF;
    static constexpr uint16_t C_GREEN      = 0x07E0;
    static constexpr uint16_t C_GREY       = 0x7BEF;
    static constexpr uint16_t C_ORANGE     = 0xFD20;
    static constexpr uint16_t C_RED        = 0xF800;

    // ── Private draw methods ───────────────────────────────────
    void drawHeader();
    void drawAnimationZone();
    void drawProgressBar();
    void drawStatusZone();

    // Animation helpers
    void drawSpinner(int x, int y, int size, uint8_t frame);
    void drawPulsingCircle(int x, int y, uint8_t frame);
    void drawWaveform(int x, int y, int w, int h, uint8_t frame);
    void drawCheckmark(int x, int y, uint16_t color);
    void drawXmark(int x, int y, uint16_t color);
};
