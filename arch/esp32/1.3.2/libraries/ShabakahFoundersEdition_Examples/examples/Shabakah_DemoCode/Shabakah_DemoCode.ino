/**
 * @file    Shabakah_DemoCode.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    13 August 2023
 * 
 * @brief   This demo utilizes all of the components on the Shabakah (Founders Edition) board.
 *          > Addressable RGB LED
 *          > Two buttons
 *          > Temperature and Humidity Sensor
 *          > Serial output from both native USB and UART 
 *          
 *          In this demo code, the RGB LED will cycle through a range of colors. You can use the two buttons 
 *          (labeled "Boot" and "Button") to adjust the intensity of the LED brightness. The temperature and 
 *          humidity sensor will take readings every second and send them via serial output (both USB and UART) 
 *          to a connected computer. If the humidity reading exceeds 65%, the RGB LED on the Shabakah (Founders Edition) board 
 *          will turn red until the humidity falls below this threshold.
 *          
 *          In this project we rely on open-source libraries. We appreciate the creators of those libraries for 
 *          their contribution to the open-source community! 
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
int LED_Brightness = 50;                    //Controls LED brightness (Range is from 0 to 255)
int Hue = 0;                                //Controls the rotating "base color" 
Adafruit_NeoPixel Shabakah_RGBLED(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);       //Object to control our RGB LED

/* Buttons */
const int Shabakah_Button_Boot = 9;         //Boot Button - on the top right side of the Shabakah board
const int Shabakah_Button = 0;              //Button - on the bottom right side of the Shabakah board

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
  
  /* Initialize UART serial output */
  Serial1.begin(9600, SERIAL_8N1, RX, TX);
  
  /* Initialize Buttons */
  pinMode(Shabakah_Button, INPUT);               //pulled-down and debounce accounted for externally
  pinMode(Shabakah_Button_Boot, INPUT_PULLUP);   //No external pull-up, must be done internally
  
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
  
  /* Initialize RGB LED */
  Shabakah_RGBLED.begin();
  Shabakah_RGBLED.setBrightness(LED_Brightness);
  Shabakah_RGBLED.clear();
  Shabakah_RGBLED.show();
}


void loop()
{
  /* Run every 1 second */
  if(RunTime == 1000)
  {
    /* Output via serial */
    Serial.println("> USB Output: Hello from Shabakah (Founders Edition)!");
    Serial1.println("> UART Output: Hello from Shabakah (Founders Edition)!");

    /* Get current temperature and humidity readings */
    CurrTemp = SHT30_Sensor.GetTemperatureCelsius();
    CurrHumd = SHT30_Sensor.GetHumidity();
    
    Serial.print("Temperature = ");
    Serial.print(CurrTemp);
    Serial.println(" C");
    Serial.print("Humidity = ");
    Serial.print(CurrHumd);
    Serial.println("%\n");

    Serial1.print("Temperature = ");
    Serial1.print(CurrTemp);
    Serial1.println(" C");
    Serial1.print("Humidity = ");
    Serial1.print(CurrHumd);
    Serial1.println("%\n");
    
    RunTime = 0;
  }

  /* Increase or decrease brightness based on button presses */
  //Increase brightness
  if( !digitalRead(Shabakah_Button_Boot) )
  {
    Serial.println("> USB Output: Boot Button pressed!");
    Serial1.println("> UART Output: Boot Button pressed!");
    
    LED_Brightness = LED_Brightness + 1;
    if(LED_Brightness > 255)
    {
      LED_Brightness = 255;
    }
  }
  //Decrease brightness
  if( digitalRead(Shabakah_Button) )
  {
    Serial.println("> USB Output: Button pressed!");
    Serial1.println("> UART Output: Button pressed!");
    LED_Brightness = LED_Brightness - 1;
    if(LED_Brightness < 0)
    {
      LED_Brightness = 0;
    }
  }
    
  /* If humidity is above threshold, change LED color to red */
  if( CurrHumd >= HUMD_THRESHOLD)
  {
    Shabakah_RGBLED.setPixelColor(0, 255, 0, 0);  
  }
  /* else, cycle through LED colors */
  else
  {
    Hue = Hue + 20;         //Slowly cycle the "base color" through the rainbow
    Shabakah_RGBLED.setPixelColor(0, Shabakah_RGBLED.ColorHSV(Hue));
    /* If we exceed the max Hue value, set back to 0: */
    if(Hue > 65536)
    {
      Hue = 0;
    }
  }

  /* Update LED */
  Shabakah_RGBLED.setBrightness(LED_Brightness);
  Shabakah_RGBLED.show();

  /* Increment counter */
  RunTime++;

  delay(1);
}
