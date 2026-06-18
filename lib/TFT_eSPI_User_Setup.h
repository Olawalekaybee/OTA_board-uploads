// ═══════════════════════════════════════════════════════════════
//  TFT_eSPI USER SETUP - CYD 2.8" ILI9341 (320x240)
//
//  LOCATION: lib/TFT_eSPI/User_Setup.h
//  (PlatformIO will automatically find and use this)
//
//  This file configures TFT_eSPI for your CYD display.
// ═══════════════════════════════════════════════════════════════

#ifndef USER_SETUP_H
#define USER_SETUP_H

#define USER_SETUP_LOADED

// ═══════════════════════════════════════════════════════════════
//  DISPLAY DRIVER
// ═══════════════════════════════════════════════════════════════
#define ILI9341_DRIVER

// ═══════════════════════════════════════════════════════════════
//  INTERFACE - SPI (4-wire)
// ═══════════════════════════════════════════════════════════════
#define TFT_SPI_HOST     VSPI_HOST

// CYD PIN CONFIGURATION
// ┌─────────────────────────┐
// │   CYD 2.8"              │
// │   ILI9341 SPI pins      │
// ├─────────────────────────┤
#define TFT_MOSI    13  // SDI (Serial Data In)
#define TFT_MISO    12  // SDO (Serial Data Out) - can be unconnected
#define TFT_SCLK    14  // SCK (Serial Clock)
#define TFT_CS      15  // CS  (Chip Select)
#define TFT_DC      2   // DC  (Data/Command)
#define TFT_RST     -1  // RST (Reset) - not used (pull-up to VCC)
#define TFT_BL      21  // BL  (Backlight)
#define TFT_BACKLIGHT_ON 1  // Set to 1 if backlight on = HIGH, 0 if on = LOW

// ═══════════════════════════════════════════════════════════════
//  DISPLAY ORIENTATION
// ═══════════════════════════════════════════════════════════════
#define TFT_WIDTH   240
#define TFT_HEIGHT  320
#define TFT_ROTATION 1      // 0 = portrait, 1 = landscape, 2 = portrait flipped, 3 = landscape flipped
// For this project use rotation = 1 (landscape 320×240)

// ═══════════════════════════════════════════════════════════════
//  FONT LOADING
// ═══════════════════════════════════════════════════════════════
// Only load fonts you actually use to save Flash space
#define LOAD_GLCD       // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2      // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4      // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6      // Font 6. Large 48 pixel high font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7      // Font 7. 7 segment 48 pixel high font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8      // Font 8. Large 75 pixel high font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_RLE_FONT   // Enable free fonts

// ═══════════════════════════════════════════════════════════════
//  SPI SPEED
// ═══════════════════════════════════════════════════════════════
#define SPI_FREQUENCY  27000000  // 27 MHz - safe for ILI9341 (max ~50 MHz)
#define SPI_READ_FREQUENCY 5000000  // 5 MHz - slower for reads

// ═══════════════════════════════════════════════════════════════
//  COLOUR MODE (DO NOT CHANGE)
// ═══════════════════════════════════════════════════════════════
#define TFT_BL_ON HIGH  // Backlight pin HIGH = ON

// ═══════════════════════════════════════════════════════════════
//  DEBUGGING (Set to 0 for production)
// ═══════════════════════════════════════════════════════════════
#define TFT_VERBOSE 0

#endif  // USER_SETUP_H
