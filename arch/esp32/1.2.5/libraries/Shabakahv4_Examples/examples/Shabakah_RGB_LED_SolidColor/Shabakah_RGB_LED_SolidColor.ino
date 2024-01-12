/**
 * @file    Shabakah_RGB_LED_SolidColor.ino
 * @authors Khalid Mansoor AlAwadhi, Remal IoT
 * @date    12 Jan 2023
 * 
 * @brief   In this example, you can easily select the color to display on the LEDs by setting 
 *          individual red, green, and blue values. Additionally, you can adjust the brightness.
 *
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Adafruit_NeoPixel.h"


/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_1_PIN = 1;                    //The pin connected to LED 1 on Shabakah
const int LED_2_PIN = 3;                    //The pin connected to LED 2 on Shabakah
const int NumLEDs = 1;                      //Number of LEDs on each pin
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2


void setup()
{
  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();
  Shabakah_LED_2.begin();
  Shabakah_LED_2.clear();
  Shabakah_LED_2.show();

  /* Set the LED colors and brightness below.
     You can use this website to quickly find the color you want to display: https://htmlcolorcodes.com/color-names/
  */
  int LED_1_Red_Value = 255;              //Range is from 0 to 255
  int LED_1_Green_Value = 69;             //Range is from 0 to 255
  int LED_1_Blue_Value = 0;               //Range is from 0 to 255
  int LED_1_Brightness = 100;             //Range is from 0 to 255

  int LED_2_Red_Value = 148;              //Range is from 0 to 255
  int LED_2_Green_Value = 0;              //Range is from 0 to 255
  int LED_2_Blue_Value = 211;             //Range is from 0 to 255
  int LED_2_Brightness = 50;              //Range is from 0 to 255

  /* Now apply the settings to the LEDs: */
  Shabakah_LED_1.setPixelColor(0, LED_1_Red_Value, LED_1_Green_Value, LED_1_Blue_Value);
  Shabakah_LED_1.setBrightness(LED_1_Brightness);
  
  Shabakah_LED_2.setPixelColor(0, LED_2_Red_Value, LED_2_Green_Value, LED_2_Blue_Value);
  Shabakah_LED_2.setBrightness(LED_2_Brightness);

  /* Shabakah_LED_X.show() must be called to display and reflect any changes made to the RGB LEDs */
  Shabakah_LED_1.show();
  Shabakah_LED_2.show();
}


void loop()
{
  //Nothing to do, set the color you want to see in setup() above!
}
