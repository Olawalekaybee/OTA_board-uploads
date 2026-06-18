#pragma once

// ═══════════════════════════════════════════════════════════════
//  CYD 2.8" PINOUT CONFIGURATION
// ═══════════════════════════════════════════════════════════════

// --- SPI Display Pins (TFT_eSPI) ---
#define TFT_MOSI    13
#define TFT_MISO    12
#define TFT_SCLK    14
#define TFT_CS      15
#define TFT_DC      2
#define TFT_RST     -1  // Not used
#define TFT_BL      21  // Backlight

// --- I2C Bus (Slave Receiver for ESP32-S3 telemetry) ---
#define I2C_SDA_PIN 22
#define I2C_SCL_PIN 27
#define I2C_SLAVE_ADDRESS 0x12  // CYD listens on this address
#define I2C_FREQUENCY 100000

// ═══════════════════════════════════════════════════════════════
//  TIMING
// ═══════════════════════════════════════════════════════════════
#define RENDER_INTERVAL_MS 2000      // Update display every 2 seconds
#define STALE_WARN_MS 8000           // Warn if no data for 8 seconds

// ═══════════════════════════════════════════════════════════════
//  DEBUGGING
// ═══════════════════════════════════════════════════════════════
#define ENABLE_DEBUG_LOGS 1

// ═══════════════════════════════════════════════════════════════
//  OTA SETTINGS
// ═══════════════════════════════════════════════════════════════
#define OTA_HOSTNAME "agrinode-cyd"
#define OTA_PASSWORD "admin"  // Change this to something secure!
