#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

#define EXTERNAL_NUM_INTERRUPTS 22
#define NUM_DIGITAL_PINS        22
#define NUM_ANALOG_INPUTS       6

/*
 * Changed for Shabakah (Founders Edition)
 */
static const uint8_t RGB_LED_PIN = 6;
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT+RGB_LED_PIN;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility
#define LED_BUILTIN LED_BUILTIN
#define RGB_BUILTIN LED_BUILTIN

#define analogInputToDigitalPin(p)  (((p)<NUM_ANALOG_INPUTS)?(analogChannelToDigitalPin(p)):-1)
#define digitalPinToInterrupt(p)    (((p)<NUM_DIGITAL_PINS)?(p):-1)
#define digitalPinHasPWM(p)         (p < EXTERNAL_NUM_INTERRUPTS)

static const uint8_t TX = 21;
static const uint8_t RX = 20;

static const uint8_t SDA = 2;									//Changed for Shabakah (Founders Edition)
static const uint8_t SCL = 8;									//Changed for Shabakah (Founders Edition)



/*
 * Added for Shabakah (Founders Edition)
 */
#define SHBK_BOARD
#define SHBK_FE
#define SHBK_BTN				0
#define SHBK_BTN_BOOT			9
#define SHBK_I2C_TEMP_ADDR		0x44


static const uint8_t SS    = 7;
static const uint8_t MOSI  = 6;
static const uint8_t MISO  = 5;
static const uint8_t SCK   = 4;

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;

#endif /* Pins_Arduino_h */
