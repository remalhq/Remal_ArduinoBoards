/**
 * @file    Atlas_Accelerometer_Readings.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    11 June 2023
 * 
 * @brief   This example initializes the LIS3DHTR accelerometer sensor. If the initialization is successful, it sets the LED 
 *          strip to green; otherwise, it sets it to red. After successful initialization, the program continuously 
 *          polls the LIS3DHTR sensor to get the current X, Y, and Z readings. It then prints these readings via the 
 *          serial interface. To view the values, you can use the Serial Monitor. Alternatively, you can utilize 
 *          the Serial Plotter for a more visual representation of the current values.
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"
#include <Wire.h>
#include "LIS3DHTR.h"


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

/* Accelerometer */
const int Accel_Addr = 0x18;                    //The I2C address of the accelerometer
LIS3DHTR<TwoWire> LIS_Accel;                    //Create sensor object
float X_Val = 0;                                //Stores the current X value from the accelerometer
float Y_Val = 0;                                //Stores the current Y value from the accelerometer
float Z_Val = 0;                                //Stores the current Z value from the accelerometer




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

  /* Initialize the Accelerometer */
  LIS_Accel.begin(Accel_Addr);
  delay(100);
  if(!LIS_Accel)
  {
    Serial.println("> Error: Could not initialize SHT30 Accelerometer!");
    ErrorLoop();
  }
  
  /* 
   * Uncomment the settings you want: 
   */
  //Range:
  LIS_Accel.setFullScaleRange(LIS3DHTR_RANGE_2G);
  //LIS_Accel.setFullScaleRange(LIS3DHTR_RANGE_4G);
  //LIS_Accel.setFullScaleRange(LIS3DHTR_RANGE_8G);
  //LIS_Accel.setFullScaleRange(LIS3DHTR_RANGE_16G);
  
  //Data rate:
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_1HZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_10HZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
  LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_100HZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_200HZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_1_6KHZ);
  //LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_5KHZ);
  
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  FastLED.show();
}


void loop()
{
  X_Val = LIS_Accel.getAccelerationX();     //Get the current X value from the accelerometer
  Y_Val = LIS_Accel.getAccelerationY();     //Get the current Y value from the accelerometer
  Z_Val = LIS_Accel.getAccelerationZ();     //Get the current Z value from the accelerometer

  /* Output via serial */
  Serial.print("X:");
  Serial.print(X_Val);
  Serial.print(", ");
  Serial.print("Y:");
  Serial.print(Y_Val);
  Serial.print(", ");
  Serial.print("Z:");
  Serial.println(Z_Val);
  
  delay(50);
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