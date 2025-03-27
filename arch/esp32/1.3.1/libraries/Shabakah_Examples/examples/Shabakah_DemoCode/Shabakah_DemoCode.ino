/**
 * @file    Shabakah_DemoCode.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    12 Jan 2024
 * 
 * @brief   This demo utilizes all of the components on the Shabakah board.
 *          > Addressable RGB LEDs 1 and 2
 *          > Buttons 1 and 2
 *          > SHT30 Temperature and Humidity Sensor
 *          > Serial output via USB
 * 
 *          In this demo code, the RGB LEDs will cycle through a range of colors. You can use
 *          buttons 1 and 2 to adjust the intensity of the LEDs brightness. The temperature and 
 *          humidity sensor will take readings every second and send them via serial output to a 
 *          connected computer. If the humidity reading exceeds the threshold defined by HUMD_THRESHOLD
 *          the LEDs on Shabakah will turn red until the humidity falls below the threshold again.
 *          
 *          In this project we rely on open-source libraries. We appreciate the creators of those libraries for 
 *          their contribution to the open-source community! 
 *          
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
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
int Hue = 0;                                //Controls the rotating "base color" 
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2

/* Buttons */
const int Shabakah_Button_1 = 9;            //Button 1 - right side of the Shabakah board
const int Shabakah_Button_2 = 10;           //Button 2 - left side of the Shabakah board

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;             //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);          //Create sensor object
float CurrTemp = 0;                         //Stores current temperature readings
float CurrHumd = 0;                         //Stores current humidity readings 
#define HUMD_THRESHOLD              65.0    //The threshold before turning the LEDs red

/* Loop timing counters */
int RunTime = 0;                            //Used to run certain actions at a predetermined interval




void setup()
{
  /* Initialize USB serial output */
  Serial.begin();
  Serial.setTxTimeoutMs(0);     //This is used to avoid waiting if the USB is not connected 
  
  /* Initialize Buttons */
  pinMode(Shabakah_Button_1, INPUT_PULLUP);       //No external pull-up, must be done internally
  pinMode(Shabakah_Button_2, INPUT_PULLUP);       //No external pull-up, must be done internally

  /* Initialize the Temp/Humd sensor */
  SHT30_Sensor.Initialize();
  
  /* Set the repeatability mode to high (e_high)
   * The repeatability mode determines the measurement accuracy and energy consumption.
   * Available options are e_low, e_medium, and e_high. */
  SHT30_Sensor.SetRepeatability(e_high);
  
  if( !SHT30_Sensor.IsConnected() )
  {
    Serial.println("> Error: Could not initialize SHT30 temperature and humidity sensor!");
  }
  
  /* Initialize our LEDs */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.setBrightness(LED_Brightness);
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();

  Shabakah_LED_2.begin();
  Shabakah_LED_2.setBrightness(LED_Brightness);
  Shabakah_LED_2.clear();
  Shabakah_LED_2.show();
}


void loop()
{
  /* Run every 1 second */
  if(RunTime == 1000)
  {
    /* Output via serial */
    Serial.println("> Hello from Shabakah!");

    /* Get current temperature and humidity readings */
    CurrTemp = SHT30_Sensor.GetTemperatureCelsius();
    CurrHumd = SHT30_Sensor.GetHumidity();
    
    Serial.print("Temperature = ");
    Serial.print(CurrTemp);
    Serial.println(" C");
    Serial.print("Humidity = ");
    Serial.print(CurrHumd);
    Serial.println("%\n");
    
    RunTime = 0;
  }

  /* Increase or decrease brightness based on button presses */
  //Increase brightness
  if( digitalRead(Shabakah_Button_1) == 0 )
  {
    Serial.println("> Button 1 pressed!");
    delay(10);   //Wait a bit to not increase too fast

    LED_Brightness = LED_Brightness + 1;
    if(LED_Brightness > 255)
    {
      LED_Brightness = 255;
    }
  }
  //Decrease brightness
  if( digitalRead(Shabakah_Button_2) == 0 )
  {
    Serial.println("> Button 2 pressed!");
    delay(10);   //Wait a bit to not decrease too fast

    LED_Brightness = LED_Brightness - 1;
    if(LED_Brightness < 0)
    {
      LED_Brightness = 0;
    }
  }
    
  /* If humidity is above threshold, change LEDs color to red */
  if( CurrHumd >= HUMD_THRESHOLD)
  {
    Shabakah_LED_1.setPixelColor(0, 255, 0, 0);
    Shabakah_LED_2.setPixelColor(0, 255, 0, 0);  
  }
  /* else, cycle through LED colors */
  else
  {
    Hue = Hue + 20;         //Slowly cycle the "base color" through the rainbow
    Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.ColorHSV(Hue));
    Shabakah_LED_2.setPixelColor(0, Shabakah_LED_2.ColorHSV(Hue));

    /* If we exceed the max Hue value, set back to 0: */
    if(Hue > 65536)
    {
      Hue = 0;
    }
  }

  /* Update LEDs */
  Shabakah_LED_1.setBrightness(LED_Brightness);
  Shabakah_LED_2.setBrightness(LED_Brightness);
  Shabakah_LED_1.show();
  Shabakah_LED_2.show();

  /* Increment counter */
  RunTime++;

  delay(1);
}
