/**
 * @file    Shabakah_TempHumd_Measure.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    12 Jan 2024
 * 
 * @brief   This example initializes the SHT30 sensor. If the initialization is successful, it sets 
 *          LED 1 to green; otherwise, it sets it to red. After successful initialization, the program 
 *          continuously polls the SHT30 sensor every 500 ms to retrieve the current temperature and humidity 
 *          readings. It then prints these readings via the serial. To view the values, you can use the 
 *          Serial Monitor (Found under "Tools") Alternatively, you can utilize the Serial Plotter for a 
 *          more visual representation of the current values.
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
const int NumLEDs = 1;                      //Number of LEDs on each pin
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;             //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);          //Create sensor object
float CurrTemp = 0;                         //Stores current temperature readings
float CurrHumd = 0;                         //Stores current humidity readings 



void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);
  
  /* Initialize LED 1 to be off */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();

  /* Initialize the Temp/Humd sensor */
  SHT30_Sensor.Initialize();
  
  /* Set the repeatability mode to high (e_high)
   * The repeatability mode determines the measurement accuracy and energy consumption.
   * Available options are e_low, e_medium, and e_high. */
  SHT30_Sensor.SetRepeatability(e_high);
  
  if( !SHT30_Sensor.IsConnected() )
  {
    Serial.println("> Error: Could not initialize SHT30 temperature and humidity sensor!");

    /* If sensor fails to connect, turn LED 1 RED */
    Shabakah_LED_1.setPixelColor(0, 255, 0, 0);
    Shabakah_LED_1.show();
    while(1)
    {
      //Wait forever
    }
  }

  /* If sensor all good, turn LED 1 GREEN */
  Shabakah_LED_1.setPixelColor(0, 0, 255, 0);
  Shabakah_LED_1.show();
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