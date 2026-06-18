#include "OTABootAnimation.h"

// ═══════════════════════════════════════════════════════════════
//  CONSTRUCTOR
// ═══════════════════════════════════════════════════════════════
OTABootAnimation::OTABootAnimation(TFT_eSPI& tft_ref)
    : tft(tft_ref)
{
}

// ═══════════════════════════════════════════════════════════════
//  begin()  — called from setup() to initialize display
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::begin()
{
    tft.fillScreen(C_BG);
    tft.setTextDatum(TL_DATUM);

    _state = OTA_INITIALIZING;
    _lastState = (OTAState)-1;
    _frameTimer = millis();
    _frameCount = 0;

    drawHeader();
    drawAnimationZone();
    drawProgressBar();
    drawStatusZone();

    Serial.println("[OTA Anim] Boot animation started");
}

// ═══════════════════════════════════════════════════════════════
//  update()  — call from loop() every ~50 ms
//
//  Returns true if animation is still running (not complete/error)
// ═══════════════════════════════════════════════════════════════
bool OTABootAnimation::update()
{
    uint32_t now = millis();

    // ── Frame timer ───────────────────────────────────────────
    if (now - _frameTimer < FRAME_INTERVAL)
    {
        return !isComplete();
    }

    _frameTimer = now;
    _frameCount++;

    // ── Mode transition (clear zone, draw new state) ──────────
    if (_state != _lastState)
    {
        _lastState = _state;
        _frameCount = 0;

        // Clear animation and progress zones
        tft.fillRect(0, ZONE_ANIM_Y, 320, ZONE_ANIM_H + ZONE_PROG_H, C_BG);
    }

    drawAnimationZone();
    drawProgressBar();
    drawStatusZone();

    return !isComplete();
}

// ═══════════════════════════════════════════════════════════════
//  setState() / setProgress() / setError()
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::setState(OTAState newState)
{
    _state = newState;
}

void OTABootAnimation::setProgress(uint8_t percent)
{
    _progress = constrain(percent, 0, 100);
}

void OTABootAnimation::setError(const char* message)
{
    _state = OTA_ERROR;
    _errorMsg = message;
}

// ═══════════════════════════════════════════════════════════════
//  drawHeader()
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawHeader()
{
    tft.fillRect(0, ZONE_HDR_Y, 320, ZONE_HDR_H, C_HDR);
    tft.drawFastHLine(0, ZONE_HDR_H - 1, 320, C_ACCENT);

    tft.setTextColor(C_ACCENT, C_HDR);
    tft.setTextFont(4);
    tft.setCursor(60, 8);
    tft.print("AgriNode");

    tft.setTextFont(1);
    tft.setTextColor(C_GREY, C_HDR);
    tft.setCursor(70, 32);
    tft.print("Firmware Update");
}

// ═══════════════════════════════════════════════════════════════
//  drawAnimationZone()  — displays animated spinner/pulse
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawAnimationZone()
{
    int cx = 160;
    int cy = ZONE_ANIM_Y + 35;

    // Clear zone
    tft.fillRect(0, ZONE_ANIM_Y, 320, ZONE_ANIM_H, C_BG);

    // ── Draw based on current state ───────────────────────────
    switch (_state)
    {
        case OTA_INITIALIZING:
            drawSpinner(cx, cy, 30, _frameCount);
            tft.setTextColor(C_WHITE, C_BG);
            tft.setTextFont(2);
            tft.setCursor(110, ZONE_ANIM_Y + 55);
            tft.print("Initializing");
            break;

        case OTA_CONNECTING_SERVER:
            drawPulsingCircle(cx, cy, _frameCount);
            tft.setTextColor(C_ACCENT, C_BG);
            tft.setTextFont(2);
            tft.setCursor(90, ZONE_ANIM_Y + 55);
            tft.print("Connecting");
            break;

        case OTA_DOWNLOADING:
        case OTA_FLASHING:
            drawWaveform(40, ZONE_ANIM_Y + 10, 240, 40, _frameCount);
            break;

        case OTA_VERIFYING:
            drawSpinner(cx, cy, 25, _frameCount);
            tft.setTextColor(C_GREEN, C_BG);
            tft.setTextFont(2);
            tft.setCursor(100, ZONE_ANIM_Y + 55);
            tft.print("Verifying");
            break;

        case OTA_COMPLETE:
            drawCheckmark(cx, cy, C_GREEN);
            tft.setTextColor(C_GREEN, C_BG);
            tft.setTextFont(2);
            tft.setCursor(105, ZONE_ANIM_Y + 55);
            tft.print("Complete!");
            break;

        case OTA_ERROR:
            drawXmark(cx, cy, C_RED);
            tft.setTextColor(C_RED, C_BG);
            tft.setTextFont(2);
            tft.setCursor(115, ZONE_ANIM_Y + 55);
            tft.print("Failed");
            break;
    }
}

// ═══════════════════════════════════════════════════════════════
//  drawProgressBar()
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawProgressBar()
{
    int y = ZONE_PROG_Y;
    int x = 30;
    int w = 260;
    int h = 16;

    // Clear zone
    tft.fillRect(0, y, 320, ZONE_PROG_H, C_BG);

    // Only show progress bar for states that have meaningful progress
    if (_state != OTA_DOWNLOADING && _state != OTA_FLASHING)
    {
        return;
    }

    // Track background
    tft.fillRoundRect(x, y + 4, w, h, 3, 0x1124);   // very dark grey

    // Filled portion
    int fillW = (_progress * w) / 100;
    if (fillW > 0)
    {
        tft.fillRoundRect(x, y + 4, fillW, h, 3, C_ACCENT);
    }

    // Border
    tft.drawRoundRect(x, y + 4, w, h, 3, C_ACCENT);

    // Percentage text
    tft.setTextColor(C_WHITE, C_BG);
    tft.setTextFont(2);
    tft.setCursor(x + w + 10, y + 5);
    tft.printf("%3d%%", _progress);
}

// ═══════════════════════════════════════════════════════════════
//  drawStatusZone()  — displays current status message
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawStatusZone()
{
    int y = ZONE_STAT_Y;

    // Clear
    tft.fillRect(0, y, 320, ZONE_STAT_H, C_BG);

    // Top border
    tft.drawFastHLine(0, y, 320, 0x18E5);

    tft.setTextFont(1);
    tft.setTextSize(1);

    // ── Status message ────────────────────────────────────────
    const char* statusMsg = "";
    uint16_t statusColor = C_GREY;

    switch (_state)
    {
        case OTA_INITIALIZING:
            statusMsg = "Scanning WiFi network...";
            statusColor = C_GREY;
            break;
        case OTA_CONNECTING_SERVER:
            statusMsg = "Contacting update server...";
            statusColor = C_ACCENT;
            break;
        case OTA_DOWNLOADING:
            statusMsg = "Downloading firmware package...";
            statusColor = C_ACCENT;
            break;
        case OTA_FLASHING:
            statusMsg = "Writing to flash memory...";
            statusColor = C_ORANGE;
            break;
        case OTA_VERIFYING:
            statusMsg = "Verifying firmware integrity...";
            statusColor = C_GREEN;
            break;
        case OTA_COMPLETE:
            statusMsg = "Update successful! Rebooting...";
            statusColor = C_GREEN;
            break;
        case OTA_ERROR:
            statusMsg = "Update failed!";
            statusColor = C_RED;
            break;
    }

    tft.setTextColor(statusColor, C_BG);
    tft.setCursor(15, y + 8);
    tft.print(statusMsg);

    // ── Details line ──────────────────────────────────────────
    if (_state == OTA_DOWNLOADING || _state == OTA_FLASHING)
    {
        tft.setTextColor(C_GREY, C_BG);
        tft.setCursor(15, y + 25);
        tft.print("Progress: ");
        tft.setTextColor(C_WHITE, C_BG);
        tft.printf("%d%%", _progress);
    }
    else if (_state == OTA_ERROR)
    {
        tft.setTextColor(C_RED, C_BG);
        tft.setCursor(15, y + 25);
        if (_errorMsg)
        {
            tft.print(_errorMsg);
        }
        else
        {
            tft.print("Check WiFi connection");
        }

        tft.setTextColor(C_GREY, C_BG);
        tft.setCursor(15, y + 40);
        tft.print("Rebooting in 10 seconds...");
    }
    else if (_state == OTA_COMPLETE)
    {
        tft.setTextColor(C_GREEN, C_BG);
        tft.setCursor(15, y + 25);
        tft.print("Device will reboot shortly");
    }
}

// ═══════════════════════════════════════════════════════════════
//  drawSpinner()  — rotating spinner animation
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawSpinner(int x, int y, int size, uint8_t frame)
{
    frame = frame % 12;   // 12 rotation steps
    float angle = (frame * 30.0) * 3.14159 / 180.0;

    // Draw outer circle (static)
    tft.drawCircle(x, y, size + 5, C_GREY);

    // Draw 4 rotating dots
    for (int i = 0; i < 4; i++)
    {
        float a = angle + (i * 3.14159 / 2.0);
        int px = x + (int)((size + 8) * cos(a));
        int py = y + (int)((size + 8) * sin(a));
        tft.fillCircle(px, py, 3, C_ACCENT);
    }

    // Center circle
    tft.fillCircle(x, y, 4, C_ACCENT);
}

// ═══════════════════════════════════════════════════════════════
//  drawPulsingCircle()  — breathing pulse effect
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawPulsingCircle(int x, int y, uint8_t frame)
{
    frame = frame % 20;   // 20-frame cycle

    // Pulse from small to large and back
    int minR = 15;
    int maxR = 35;
    int r;

    if (frame < 10)
    {
        // Expand
        r = minR + (frame * (maxR - minR) / 10);
    }
    else
    {
        // Contract
        r = maxR - ((frame - 10) * (maxR - minR) / 10);
    }

    // Outer pulsing circle
    tft.drawCircle(x, y, r, C_ACCENT);
    tft.drawCircle(x, y, r - 1, C_ACCENT);

    // Inner circle
    tft.fillCircle(x, y, 8, C_ACCENT);
}

// ═══════════════════════════════════════════════════════════════
//  drawWaveform()  — animated equalizer-style bars
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawWaveform(int x, int y, int w, int h, uint8_t frame)
{
    int barW = 8;
    int barSpacing = 12;
    int numBars = w / barSpacing;

    frame = frame % 16;   // 16-frame animation cycle

    for (int i = 0; i < numBars; i++)
    {
        int barX = x + (i * barSpacing);
        
        // Staggered wave effect
        int phase = (frame + i) % 16;
        int barH;

        if (phase < 8)
        {
            barH = (phase * h / 2) / 8;
        }
        else
        {
            barH = ((16 - phase) * h / 2) / 8;
        }

        barH = constrain(barH, 2, h);

        // Draw bar at bottom of zone, animated height
        int barY = y + h - barH;
        tft.fillRect(barX, barY, barW - 2, barH, C_ACCENT);
    }

    // Label
    tft.setTextColor(C_WHITE, C_BG);
    tft.setTextFont(1);
    tft.setTextSize(1);
    tft.setCursor(x + 10, y + h + 5);
    
    if (_state == OTA_DOWNLOADING)
        tft.print("Downloading...");
    else
        tft.print("Flashing...");
}

// ═══════════════════════════════════════════════════════════════
//  drawCheckmark()  — success indicator
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawCheckmark(int x, int y, uint16_t color)
{
    int size = 40;

    // Draw circle background
    tft.drawCircle(x, y, size + 5, color);
    tft.fillCircle(x, y, size, C_BG);
    tft.drawCircle(x, y, size, color);

    // Draw checkmark
    int left_x = x - 15;
    int left_y = y + 5;
    int mid_x = x - 5;
    int mid_y = y + 15;
    int right_x = x + 15;
    int right_y = y - 8;

    tft.drawLine(left_x, left_y, mid_x, mid_y, color);
    tft.drawLine(mid_x, mid_y, right_x, right_y, color);
    tft.drawLine(left_x + 1, left_y + 1, mid_x + 1, mid_y + 1, color);
    tft.drawLine(mid_x + 1, mid_y + 1, right_x + 1, right_y + 1, color);
}

// ═══════════════════════════════════════════════════════════════
//  drawXmark()  — error indicator
// ═══════════════════════════════════════════════════════════════
void OTABootAnimation::drawXmark(int x, int y, uint16_t color)
{
    int size = 40;

    // Draw circle background
    tft.drawCircle(x, y, size + 5, color);
    tft.fillCircle(x, y, size, C_BG);
    tft.drawCircle(x, y, size, color);

    // Draw X
    int offset = 15;
    tft.drawLine(x - offset, y - offset, x + offset, y + offset, color);
    tft.drawLine(x + offset, y - offset, x - offset, y + offset, color);
    tft.drawLine(x - offset + 1, y - offset + 1, x + offset + 1, y + offset + 1, color);
    tft.drawLine(x + offset + 1, y - offset + 1, x - offset + 1, y + offset + 1, color);
}
