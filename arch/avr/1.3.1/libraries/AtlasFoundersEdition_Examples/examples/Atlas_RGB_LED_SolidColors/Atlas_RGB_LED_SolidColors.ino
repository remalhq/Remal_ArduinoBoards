/**
 * @file    Atlas_RGB_LED_SolidColors.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    11 June 2023
 * 
 * @brief   In this example, you can easily select the colors to display on each LED and adjust the brightness.
 *          Please note that you must use colors defined by the FastLED library.
 *
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"


/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* RGB LEDs */
const int LEDStrip_Pin = 13;                    //The pin connected to the LED strip
#define LED_TYPE                    WS2812B     //The model of the LED IC
#define COLOR_ORDER                 GRB         //This is based on the LED type
#define NUM_LEDS                    4           //Number of LEDs on the Atlas board
CRGB leds[NUM_LEDS];                            //This array stores current LED color informationto be used by FastLED functions



void setup()
{
  /* Initialize the LEDs to be off, using the FastLED library */
  FastLED.addLeds<LED_TYPE, LEDStrip_Pin, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(0);
  /* Start all LEDs off */
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

  /* Set the LED colors here, you must use names defined by the FastLED library: */
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Blue;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::DarkOrange;

  /* Set the brightness here, range is from 0 to 255: */
  FastLED.setBrightness(100);

  /* FastLED.show() must be called to display and reflect any changes made to the LED strip */
  FastLED.show();
}


void loop()
{
  //Nothing to do, set the colors you want to see in setup() above!
}



