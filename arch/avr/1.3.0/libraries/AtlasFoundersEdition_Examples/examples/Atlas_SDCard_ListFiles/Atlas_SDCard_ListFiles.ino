/**
 * @file    Atlas_SDCard_ListFiles.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    11 June 2023
 * 
 * @brief   This example project uses the "SdFat" library, which is more capable than the default Arduino "SD" library. 
 *          Upon startup, the project checks for a microSD card. If none is found, the LED strip will turn red and continue 
 *          to retry until a card is inserted. If a microSD card is detected, the LEDs will switch to green and a text file 
 *          named 'Hello' will be written to the SD card. Additionally, if the user presses either button 1 or button 2, all 
 *          directories and files on the microSD card will be printed via serial.
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"
#include "SdFat.h"
#include "sdios.h"


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

/* MicroSD Card + SdFat library */
const int SDCard_CS = 7;                        //The SPI chip select pin of the SD card 
#define SPI_CLOCK               SD_SCK_MHZ(8)   //SPI clock speed to use, in our case 8MHz
#define SD_CONFIG SdSpiConfig(SDCard_CS, DEDICATED_SPI, SPI_CLOCK)   //Settings for interfacing with the SD card
SdFs SDCard;                                    //SDCard object
FsFile file;                                    //Filesystem object
FsFile root;                                    //Filesystem object  

/* Buttons */
const int Atlas_Button_1 = 12;                  //Button 1 - on the left side of the Atlas board
const int Atlas_Button_2 = 10;                  //Button 2 - on the right side of the Atlas board



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

  while (!Serial)
  {
    //Wait until USB serial monitor is opened
  }

  /* Initialize the MicroSD Card using SdFat library */
  Serial.println("> Initializing MicroSD card...");
  
  if( !SDCard.begin(SD_CONFIG) )
  {
    Serial.println("> MicroSD card initialization failed, is a card inserted?");
    Serial.println("> If yes, try reinserting it and reset the Atlas board.");
    Serial.println("> If initializing keeps failing, try formatting the SD card using");
    Serial.println("  the 'SdFormatter' example found under 'File -> Examples -> SdFat'");
    
    /* Set LEDs on Atlas Board to be red */
    ErrorLoop();
  }

  Serial.println("> MicroSD card found!");
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  FastLED.show();

  /* Create Hello.txt file */
  if (!file.open("Hello.txt", O_WRONLY | O_CREAT))
  {
    Serial.println("> Error: Failed to create Hello.txt");
  }
  else
  {
    file.write("Hello from the Atlas board!");
    Serial.println("> Created Hello.txt file on card");
  }
  file.close();

  Serial.println("> Press button 1 or 2 to list all directories and files on the SD card");
}


void loop()
{
  if( digitalRead(Atlas_Button_1) || digitalRead(Atlas_Button_2) )
  {
    Serial.println("> Button 1/2 pressed - listing all directories and files:");

    /* List all directories and files on the SD card */
    ListAllFiles();
    Serial.println("-----------");
  }

  /* Loop every 100 ms*/
  delay(100);
}


/**
 * @brief Lists all directories and files found on the SD card in a tree structure with indentation
 * 
 * 
 * @param
 *        None
 *
 * @return
 *        None
 */
void ListAllFiles()
{
    int rootFileCount = 0;
    if (!root.open("/"))
    {
      Serial.println("> Error: Could not open root");
      return;
    }
    while (file.openNext(&root, O_RDONLY)) 
    {
      if (!file.isHidden()) 
      {
        rootFileCount++;
      }
      file.close();
      if (rootFileCount > 10)
      {
        Serial.println("> Error: Too many files in root. Must be less than 10");
        return;
      }
    }

    SDCard.ls("/", LS_R);
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
