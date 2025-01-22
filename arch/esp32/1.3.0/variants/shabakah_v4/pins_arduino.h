#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

/*
 * Shabakah v4 Specific (Anything added or changed): 
 */
#define SHBK_BOARD
#define SHBK_V4
#define SHBK_BTN_1              9
#define SHBK_BTN_2              10   
#define SHBK_I2C_TEMP_ADDR		0x44

static const uint8_t SDA = 2;
static const uint8_t SCL = 8;

#define PIN_RGB_LED             1       //Main LED on board

/*
 * Common with ESP32C3 (Unchanged values): 
 */
// BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
#define BUILTIN_LED LED_BUILTIN  // backward compatibility
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN
// RGB_BUILTIN and RGB_BRIGHTNESS can be used in new Arduino API rgbLedWrite()
#define RGB_BUILTIN    LED_BUILTIN
#define RGB_BRIGHTNESS 50

static const uint8_t TX = 21;
static const uint8_t RX = 20;

static const uint8_t SS = 7;
static const uint8_t MOSI = 6;
static const uint8_t MISO = 5;
static const uint8_t SCK = 4;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;

#endif /* Pins_Arduino_h */
