/**
 * @file    Atlas_TempHumd_Measure.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    13 August 2023
 * 
 * @brief   This example initializes the SHT30 sensor. If the initialization is successful, it sets the LED 
 *          strip to green; otherwise, it sets it to red. After successful initialization, the program continuously 
 *          polls the SHT30 sensor every 500 ms to retrieve the current temperature and humidity readings. It then 
 *          prints these readings via the serial interface. To view the values, you can use the Serial Monitor. 
 *          Alternatively, you can utilize the Serial Plotter for a more visual representation of the current values.
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"
#include <Wire.h>
#include "Remal_SHT3X.h"


/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* RGB LEDs */
const int LEDStrip_Pin = 13;                    //The pin connected to the LED strip
#define LED_TYPE                    WS2812B     //The model of the LED IC
#define COLOR_ORDER                 GRB         //This is based on the LED type
#define NUM_LEDS                    4           //Number of LEDs on the Atlas board
CRGB leds[NUM_LEDS];                            //This array stores current LED color informationto be used by FastLED functions
int LED_Brightness = 50;                        //Controls LED brightness (Range is from 0 to 255)

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;                 //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);              //Create sensor object
float CurrTemp = 0;                             //Stores current temperature readings
float CurrHumd = 0;                             //Stores current humidity readings 



void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);
  
  /* Initialize the LEDs, using the FastLED library */
  FastLED.addLeds<LED_TYPE, LEDStrip_Pin, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(0);
  /* Start all LEDs off */
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  FastLED.setBrightness(LED_Brightness);

  /* Initialize the SHT30 Temp/Humd sensor */
  SHT30_Sensor.Initialize();
  
  /* Set the repeatability mode to high (e_high)
   * The repeatability mode determines the measurement accuracy and energy consumption.
   * Available options are e_low, e_medium, and e_high. */
  SHT30_Sensor.SetRepeatability(e_high);
  
  if( !SHT30_Sensor.IsConnected() )
  {
    Serial.println("> Error: Could not initialize SHT30 temperature and humidity sensor!");
    ErrorLoop();
  }
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  FastLED.show();
}


void loop()
{
  /* Get current temperature and humidity readings */
  CurrTemp = SHT30_Sensor.GetTemperatureCelsius();
  CurrHumd = SHT30_Sensor.GetHumidity();

  /* Output via serial */
  Serial.print("Temperature_C:");
  Serial.print(CurrTemp);
  Serial.print(", ");
  Serial.print("Humidity_%:");
  Serial.println(CurrHumd);

  delay(500);
}


/*
 * This function is called in case of a fatal error. It serves to visually indicate 
 * the error condition by cycling through red LEDs on the Atlas board
 */
void ErrorLoop()
{
  static int CycleNum = 0;
  while(1)
  {
    /* Set LEDs on Atlas Board to be red */
    if(CycleNum == 0)
    {
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Red;
      leds[3] = CRGB::Red;
      CycleNum++;
    }
    else if(CycleNum == 1)
    {
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Red;
      leds[3] = CRGB::Red;
      CycleNum++;
    }
    else if(CycleNum == 2)
    {
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Red;
      CycleNum++;
    }
    else if(CycleNum == 3)
    {
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Red;
      leds[3] = CRGB::Black;
      CycleNum = 0;
    }    
    FastLED.show();
    delay(100);
  }
}