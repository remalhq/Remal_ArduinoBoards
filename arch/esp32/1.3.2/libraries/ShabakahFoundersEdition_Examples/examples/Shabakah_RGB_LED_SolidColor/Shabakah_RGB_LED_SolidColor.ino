/**
 * @file    Shabakah_RGB_LED_SolidColor.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    18 July 2023
 * 
 * @brief   In this example, you can easily select the color to display on the LED by setting 
 *          individual red, green, and blue values. Additionally, you can adjust the brightness.
 *
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Adafruit_NeoPixel.h"


/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_PIN = 6;                      //The pin connected to the RGB LED on Shabakah
#define NUM_LEDS                    1       //Number of LEDs
Adafruit_NeoPixel Shabakah_RGBLED(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);       //Object to control our RGB LED


void setup()
{
  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_RGBLED.begin();
  Shabakah_RGBLED.clear();
  Shabakah_RGBLED.show();

  /* Set the LED colors and brightness below.
     You can use this website to quickly find the color you want to display: https://htmlcolorcodes.com/color-names/
  */
  int Red_Value = 255;              //Range is from 0 to 255
  int Green_Value = 69;             //Range is from 0 to 255
  int Blue_Value = 0;               //Range is from 0 to 255
  int LED_Brightness = 100;         //Range is from 0 to 255

  /* Now apply the settings to the LED: */
  Shabakah_RGBLED.setPixelColor(0, Red_Value, Green_Value, Blue_Value);
  Shabakah_RGBLED.setBrightness(LED_Brightness);

  /* Shabakah_RGBLED.show() must be called to display and reflect any changes made to the RGB LED */
  Shabakah_RGBLED.show();
}


void loop()
{
  //Nothing to do, set the color you want to see in setup() above!
}



