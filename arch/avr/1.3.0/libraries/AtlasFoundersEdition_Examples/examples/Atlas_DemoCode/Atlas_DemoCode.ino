/**
 * @file    Atlas_DemoCode.ino
 * @authors Khalid AlAwadhi, Remal IoT | Mohammed AlSuwaidi, Remal IoT
 * @date    13 August 2023
 * 
 * @brief   This sample demo utilizes all of the components on the Atlas board.
 *          > Addressable RGB LEDs
 *          > Two buttons
 *          > Temperature and Humidity Sensor
 *          > Accelerometer
 *          > MicroSD Card (if plugged in)
 *          
 *          In this sample demo code, one RGB LED will keep changing colors. Based on the 
 *          accelerometer movement you can move the LED right or left. 
 *          The accelerometer readings will also be sent out via serial to the PC.
 *          
 *          The temperature/humidity sensor will take readings every 1 second and store them
 *          on a file on the SD card (if it was plugged in at bootup) called "Readings.txt"
 *          The readings will also be sent via serial out to the PC.
 *          
 *          If the humidity reading exceeds 60%, all the RGB LEDs on the Atlas board will turn 
 *          red until the humidity falls below this threshold.
 *          
 *          Using Buttons 1 and 2 you can change the intensity of the LED brightness
 *          
 *          In this project we rely on open-source libraries. We appreciate the creators of those libraries for 
 *          their contribution to the open-source community! 
 *          
 *          Make sure you select "Atlas" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"
#include <Wire.h>
#include "Remal_SHT3X.h"
#include "LIS3DHTR.h"
#include <SD.h>


/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* RGB LEDs */
const int LEDStrip_Pin = 13;                    //The pin connected to the LED strip
#define LED_TYPE                    WS2812B     //The model of the LED IC
#define COLOR_ORDER                 GRB         //This is based on the LED type
#define NUM_LEDS                    4           //Number of LEDs on the Atlas board
CRGB leds[NUM_LEDS];                            //This array stores current LED color informationto be used by FastLED functions
int Hue = 0;                                    //Controls the rotating "base color" 
int LED_Brightness = 50;                        //Controls LED brightness (Range is from 0 to 255)
int LED_On_Index = 3;                           //This variable controls which LED is on(1-4)

/* Buttons */
const int Atlas_Button_1 = 12;                  //Button 1 - on the left side of the Atlas board
const int Atlas_Button_2 = 10;                  //Button 2 - on the right side of the Atlas board

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;                 //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);              //Create sensor object
float CurrTemp = 0;                             //Stores current temperature readings
float CurrHumd = 0;                             //Stores current humidity readings 
#define HUMD_THRESHOLD              65.0        //The threshold before turning the LEDs red

/* Accelerometer */
const int Accel_Addr = 0x18;                    //The I2C address of the accelerometer
LIS3DHTR<TwoWire> LIS_Accel;                    //Create sensor object
float X_Val = 0;                                //Stores the current X value from the accelerometer
float Y_Val = 0;                                //Stores the current Y value from the accelerometer
float Z_Val = 0;                                //Stores the current Z value from the accelerometer

/* MicroSD Card */
const int SDCard_CS = 7;                        //The SPI chip select pin of the SD card 
bool IsCardInserted = false;                    //This flag is set at startup to decide whether to write data to the MicroSD card or not
File ReadingsTXTFile;                           //Object to interface with the SD filesystem
int ReadingsCount = 1;                          //Stores the number of readings taken and saved to the SD card

/* Loop timing counters */
int RunCount_TempRead = 0;
int RunCount_AccelRead = 0;
int RunCount_HueUpdate = 0;





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

  /* Initialize Buttons 1 and 2 */
  pinMode(Atlas_Button_1, INPUT);               //pulled-down and debounce accounted for externally
  pinMode(Atlas_Button_2, INPUT);               //pulled-down and debounce accounted for externally
  
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

  /* Initialize the Accelerometer */
  LIS_Accel.begin(Accel_Addr);
  LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);

  /* Initialize the MicroSD Card */
  Serial.println("> Initializing MicroSD card...");
  
  if( !SD.begin(SDCard_CS) )
  {
    Serial.println("> MicroSD card initialization failed, is a card inserted?");
    Serial.println("> Also make sure the microSD card is formatted as FAT");
    Serial.println("> No microSD card is found - bootup will continue as normal");
  }
  else
  {
    Serial.println("> MicroSD card found! Temperature and humidity readings will be stored in Readings.txt");
    IsCardInserted = true;
  }

  if(IsCardInserted)
  {
    /* Make sure Readings.txt does not exist */
    SD.remove("Readings.txt");
    
    /* Create Readings.txt file */
    ReadingsTXTFile = SD.open("Readings.txt", FILE_WRITE);
    
    /* Close the file */
    ReadingsTXTFile.close();
  }
}


void loop()
{
  /* Every 1 second: */
  if(RunCount_TempRead == 100)
  {
    Serial.println("> Atlas Founders Edition (v2) is alive! Hello, World!");

    /* Get current temperature and humidity readings */
    CurrTemp = SHT30_Sensor.GetTemperatureCelsius();
    CurrHumd = SHT30_Sensor.GetHumidity();

    /* Output via serial */
    Serial.print("> Temperature = ");
    Serial.print(CurrTemp);
    Serial.println(" C");
    Serial.print("> Humidity = ");
    Serial.print(CurrHumd);
    Serial.println("%\n");

    /* Store reading on SD card if it was found at bootup */
    if(IsCardInserted)
    {
      /* Open the file */
      ReadingsTXTFile = SD.open("Readings.txt", FILE_WRITE);
      
      /* If the file was opened successfully */
      if(ReadingsTXTFile)
      {
        ReadingsTXTFile.print("---- Reading #");
        ReadingsTXTFile.print(ReadingsCount);
        ReadingsTXTFile.println("----");
        ReadingsTXTFile.print("> Temperature = ");
        ReadingsTXTFile.print(CurrTemp);
        ReadingsTXTFile.println(" C");
        ReadingsTXTFile.print("> Humidity = ");
        ReadingsTXTFile.print(CurrHumd);
        ReadingsTXTFile.println("%\n");
        ReadingsCount++;
        
        /* Close the file */
        ReadingsTXTFile.close();
      }
      else
      {
        Serial.println("> MicroSD Card Error: Could not open Readings.txt");
      }
    }

    RunCount_TempRead = 0;    //Clear counter
  }

  /* Every 20ms: */
  if(RunCount_HueUpdate == 2)
  { 
    Hue++;                    //Slowly cycle the "base color" through the rainbow
    if(Hue > 255)
    {
      Hue = 0;                //Hue cannot exceed 255
    }
    RunCount_HueUpdate = 0;   //Clear counter
  }

  /* Every 100 ms: */
  if(RunCount_AccelRead == 10)
  { 
    X_Val = LIS_Accel.getAccelerationX();     //Get the current X value from the accelerometer
    Y_Val = LIS_Accel.getAccelerationY();     //Get the current Y value from the accelerometer
    Z_Val = LIS_Accel.getAccelerationZ();     //Get the current Z value from the accelerometer

    /* Output via serial */
    Serial.print("> X = ");
    Serial.print(X_Val);
    Serial.print('\t');
    Serial.print("Y = ");
    Serial.print(Y_Val);
    Serial.print('\t');
    Serial.print("Z = ");
    Serial.println(Z_Val);

    /* Decide which LED to turn on based on the X value */
    if( X_Val > 0 && LED_On_Index < 4)
    {
      LED_On_Index++;
    }
    else if ( X_Val < 0 && LED_On_Index > 1)
    {
      LED_On_Index--;
    }

    RunCount_AccelRead = 0;     //Clear counter
  }

  /* Increase or decrease brightness based on button presses */
  //Increase brightness
  if( digitalRead(Atlas_Button_2) )
  {
    Serial.println("> Button 2 pressed!");
    
    LED_Brightness = LED_Brightness + 1;
    if(LED_Brightness > 255)
    {
      LED_Brightness = 255;       //LED brightness cannot exceed 255
    }
  }
  //Decrease brightness
  if( digitalRead(Atlas_Button_1) )
  {
    Serial.println("> Button 1 pressed!");
    LED_Brightness = LED_Brightness - 1;
    if(LED_Brightness < 0)
    {
      LED_Brightness = 0;         //LED brightness cannot be less than 0
    }
  }
  
  /* Update the LEDs */
  FastLED.setBrightness(LED_Brightness);
  SetLEDColors();
  FastLED.show();                 //Send the 'leds' array out to the actual LED strip

  /* Loop every 10 ms*/
  delay(10);
  RunCount_TempRead++;
  RunCount_AccelRead++;
  RunCount_HueUpdate++;
}



/*
 * This funtion handles setting the LED colors based on things happening on the Atlas board
 */
void SetLEDColors() 
{
  /* FastLED's built-in rainbow generator */
  fill_rainbow(leds, NUM_LEDS, Hue, 7);
  
  /* Only turn on the wanted LED */
  switch(LED_On_Index)
  {
    //Only LED 1 on:
    case 1:
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      break;
        
    //Only LED 2 on:
    case 2:
      leds[0] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      break;
      
    //Only LED 3 on:
    case 3:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[3] = CRGB::Black;
      break;
      
    //Only LED 4 on:
    case 4:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      break;
  }

  /* Check if the humidity is above the threshold, 
   *  if so turn all LEDs on and set them to RED */
  if( CurrHumd >= HUMD_THRESHOLD)
  {
    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;
    leds[2] = CRGB::Red;
    leds[3] = CRGB::Red;
  }
}
