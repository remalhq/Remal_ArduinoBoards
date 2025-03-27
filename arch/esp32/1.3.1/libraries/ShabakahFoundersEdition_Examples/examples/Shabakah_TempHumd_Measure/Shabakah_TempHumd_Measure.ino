/**
 * @file    Shabakah_TempHumd_Measure.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    13 Aug 2023
 * 
 * @brief   This example initializes the SHT30 sensor. If the initialization is successful, it sets the LED 
 *          strip to green; otherwise, it sets it to red. After successful initialization, the program continuously 
 *          polls the SHT30 sensor every 500 ms to retrieve the current temperature and humidity readings. It then 
 *          prints these readings via the serial interface. To view the values, you can use the Serial Monitor (Found under "Tools")
 *          Alternatively, you can utilize the Serial Plotter for a more visual representation of the current values.
 *          
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Remal_SHT3X.h"
#include "Adafruit_NeoPixel.h"



/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_PIN = 6;                      //The pin connected to the RGB LED on Shabakah
#define NUM_LEDS                    1       //Number of LEDs
Adafruit_NeoPixel Shabakah_RGBLED(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);       //Object to control our RGB LED

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;             //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);          //Create sensor object
float CurrTemp = 0;                         //Stores current temperature readings
float CurrHumd = 0;                         //Stores current humidity readings 



void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);
  
  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_RGBLED.begin();
  Shabakah_RGBLED.clear();
  Shabakah_RGBLED.show();

  /* Initialize the Temp/Humd sensor */
  SHT30_Sensor.Initialize();
  
  /* Set the repeatability mode to high (e_high)
   * The repeatability mode determines the measurement accuracy and energy consumption.
   * Available options are e_low, e_medium, and e_high. */
  SHT30_Sensor.SetRepeatability(e_high);
  
  if( !SHT30_Sensor.IsConnected() )
  {
    Serial.println("> Error: Could not initialize SHT30 temperature and humidity sensor!");
    Shabakah_RGBLED.setPixelColor(0, 255, 0, 0);        //Set LED to red
    Shabakah_RGBLED.show();
    while(1)
    {
      //Wait forever
    }
  }

  Shabakah_RGBLED.setPixelColor(0, 0, 255, 0);        //Set LED to green
  Shabakah_RGBLED.show();
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
