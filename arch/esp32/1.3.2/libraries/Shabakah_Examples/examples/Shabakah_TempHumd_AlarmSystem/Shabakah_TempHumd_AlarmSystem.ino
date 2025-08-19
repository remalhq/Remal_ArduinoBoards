/**
 * @file    Shabakah_TempHumd_AlarmSystem.ino
 * @author  Mohammed Hani Ahmed, Remal <mhani@remal.io>
 * @date    18-08-2025
 * 
 * @brief   This example creates a temperature and humidity alarm system using the SHT30 sensor. 
 *          The program continuously monitors temperature and humidity readings every second. 
 *          When temperature exceeds 30°C or humidity exceeds 65%, both LEDs turn red to indicate 
 *          an alarm condition. When readings are within normal range, both LEDs display green. 
 *          Temperature and humidity values are also printed to the Serial Monitor for monitoring.
 *        
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal Boards (ESP32)"
 *          and the correct port under "Tools -> Port"
 */
#include "Remal_SHT3X.h"
#include "Adafruit_NeoPixel.h"

/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_1_PIN = 1;                    //The pin connected to LED 1 on Shabakah
const int LED_2_PIN = 3;                    //The pin connected to LED 2 on Shabakah
const int NumLEDs = 1;                      //Number of LEDs on each pin
int LED_Brightness = 50;                    //Controls LED brightness (Range is from 0 to 255)
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;             //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);          //Create sensor object
float CurrTemp = 0;                         //Stores current temperature readings
float CurrHumd = 0;                         //Stores current humidity readings 
#define TEMP_THRESHOLD 30.0                 //The temperature threshold before turning the LEDs red
#define HUMD_THRESHOLD 65.0                 //The humidity threshold before turning the LEDs red

void setup(){
  // Start serial communication with a baudrate of 9600 - so we can read output from the PC
  Serial.begin(9600);

  // Initialize the LEDs to be off, using the NeoPixel library
  Shabakah_LED_1.begin();
  Shabakah_LED_1.setBrightness(LED_Brightness);
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();
  Shabakah_LED_2.begin();
  Shabakah_LED_2.setBrightness(LED_Brightness);
  Shabakah_LED_2.clear();
  Shabakah_LED_2.show();

  // Initialize the Temp/Humd sensor
  SHT30_Sensor.Initialize();

  /* Set the repeatability mode to high (e_high)
   * The repeatability mode determines the measurement accuracy and energy consumption.
   * Available options are e_low, e_medium, and e_high. */
  SHT30_Sensor.SetRepeatability(e_high);

  while( !SHT30_Sensor.IsConnected() )
  {
    Serial.println("> Error: Could not initialize SHT30 temperature and humidity sensor!");
    delay(1000);
  }
}

void loop()
{
  // Get current temperature and humidity readings
  CurrTemp = SHT30_Sensor.GetTemperatureCelsius();
  CurrHumd = SHT30_Sensor.GetHumidity();

  // Output via serial
  Serial.print("Temperature = ");
  Serial.print(CurrTemp);
  Serial.print(" C, ");
  Serial.print("Humidity = ");
  Serial.print(CurrHumd);
  Serial.println(" %");

  // If temperature or humidity is above threshold, change LEDs color to red
  if(CurrTemp >= TEMP_THRESHOLD || CurrHumd >= HUMD_THRESHOLD)
  {
    Shabakah_LED_1.setPixelColor(0, 255, 0, 0);
    Shabakah_LED_2.setPixelColor(0, 255, 0, 0); 
  }
  // Otherwise, change LEDs color to green
  else
  {
    Shabakah_LED_1.setPixelColor(0, 0, 255, 0);
    Shabakah_LED_2.setPixelColor(0, 0, 255, 0);
  }

  // Update LEDs
  Shabakah_LED_1.show();
  delay(1);               // Add a small delay to allow the LEDs to update
  Shabakah_LED_2.show();

  delay(1000);            // Repeat every second
}
