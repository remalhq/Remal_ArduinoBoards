/**
 * @file    Atlas_RGB_LED_StripDemo.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    11 June 2023
 * 
 * @brief   This example loops through different animations on the RGB LED strip. 
 *          Every few seconds, the animation will change and cycle through 4 available animations. 
 *          This example is based on FastLED's 'DemoReel100' code.
 *          The available animations are:
 *              1. Rainbow
 *              2. Glittering rainbow
 *              3. Sweeping LED trail
 *              4. Pulsing
 * 
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "FastLED.h"


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
int AnimationIndex = 0;                         //Sets the current running animation on the RGB LED strip

/* Loop timing counters */
int RunCount_ChangeAnimation = 0;
int RunCount_HueUpdate = 0;



void setup()
{
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
}


void loop()
{
  /* Every 20ms, update the LED hue: */
  if(RunCount_HueUpdate == 2)
  { 
    Hue++;                            //Slowly cycle the "base color" through the rainbow
    if(Hue > 255)
    {
      Hue = 0;                        //Hue cannot exceed 255
    }
    RunCount_HueUpdate = 0;           //Clear counter
  }

  /* Every ~10 seconds, change the animation: */
  if(RunCount_ChangeAnimation == 1000)
  { 
    AnimationIndex++;
    if(AnimationIndex > 3)
    {
      AnimationIndex = 0;
    }
    RunCount_ChangeAnimation = 0;     //Clear counter
  }
  
  /* Update the LEDs */
  FastLED.setBrightness(LED_Brightness);
  Set_LED_Animation();
  FastLED.show();                     //Send the 'leds' array out to the actual LED strip

  /* Loop every 10 ms */
  delay(10);
  RunCount_ChangeAnimation++;
  RunCount_HueUpdate++;
}


/* 
 * This function controls which animation plays on the Atlas board LED strip based 
 * on the global variable AnimationIndex
 */
void Set_LED_Animation() 
{
  int pos = 0;

  switch(AnimationIndex)
  {
    /* Rainbow */
    case 0:
      fill_rainbow(leds, NUM_LEDS, Hue, 7);
      break;
    
    /* Glittering rainbow */
    case 1:
      // 
      fadeToBlackBy(leds, NUM_LEDS, 10);
      pos = random16(NUM_LEDS);
      leds[pos] += CHSV( Hue + random8(64), 200, 255);
      break;
    
    /* Sweeping LED trail */
    case 2:
      fadeToBlackBy(leds, NUM_LEDS, 20);
      pos = beatsin16( 13, 0, NUM_LEDS-1 );
      leds[pos] += CHSV( Hue, 255, 192);
      break;
    
    /* Pulsing */
    case 3:
      uint8_t Pulse = 40;
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( Pulse, 64, 255);
      for( int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = ColorFromPalette(palette, Hue+(i*2), beat-Hue+(i*10));
      }
      break;
  }
}


