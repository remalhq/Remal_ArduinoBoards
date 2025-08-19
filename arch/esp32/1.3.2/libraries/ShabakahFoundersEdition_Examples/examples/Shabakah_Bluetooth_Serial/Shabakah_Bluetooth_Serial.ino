/**
 * @file    Shabakah_Bluetooth_Serial.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    27 July 2023
 * 
 * @brief   This example shows how to use the Remal BLE (Bluetooth Low Energy) Serial library to send and receive data over Bluetooth LE, it 
 *          is meant to be used with the Shabakah (Founders Edition) board (ESP32-C3)
 *          
 *          This example will:
 *            - Display any data received from the connected Bluetooth device on the Serial Monitor
 *            - Every 1 second, a message will be sent to the connected device
 *            - RGB LED on Shabakah (Founders Edition) will be red when not connected to a device, and green when connected to a device
 *          
 *          To send or receive data from the Shabakah board, you can one of the following apps:
 *            - (iOS - Paid) BLE Terminal HM-10: https://apps.apple.com/us/app/ble-terminal-hm-10/id1398703795
 *            - (iOS - Free) Bluefruit Connect: https://apps.apple.com/us/app/bluefruit-connect/id830125974
 *            - (Android - Free) Serial Bluetooth Terminal: https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en&gl=US
 *
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Remal_BLE_Serial.h"
#include "Adafruit_NeoPixel.h"



/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_PIN = 6;                      //The pin connected to the RGB LED on Shabakah
#define NUM_LEDS                    1       //Number of LEDs
Adafruit_NeoPixel Shabakah_RGBLED(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);       //Object to control our RGB LED

/* Bluetooth */
BLESerial BT_Device;

/* Loop timing counters */
int Counter_HelloMsg = 0;                   //Used to run certain actions at a predetermined interval



void setup()
{
  /* Initialize the Serial Monitor */
  Serial.begin(115200);

  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_RGBLED.begin();
  Shabakah_RGBLED.clear();
  Shabakah_RGBLED.show();

  /* Initialize the Bluetooth */
  BT_Device.Init("Shabakah (Founders Edition)");
}


void loop()
{
  /* Check if we are connected to a device */
  if( BT_Device.IsConnected() )
  {
    /* If we are connected, turn on the RGB LED to GREEN */
    Shabakah_RGBLED.setPixelColor(0, Shabakah_RGBLED.Color(0, 255, 0));
    Shabakah_RGBLED.show();

    /* Check if we have any data available to read */
    while( BT_Device.Data_Available() > 0 )
    {
      /* Get the data */
      String Data = BT_Device.Get_Data();
      Serial.print("Data received: ");
      Serial.println(Data);
    }
  }
  else
  {
    /* If we are not connected, turn on the RGB LED to RED */
    Shabakah_RGBLED.setPixelColor(0, Shabakah_RGBLED.Color(255, 0, 0));
    Shabakah_RGBLED.show();
  }

  if(Counter_HelloMsg >= 1000)
  {
    /* Send data to the connected device */
    BT_Device.Send_Data("Hello, World! This is a message from Shabakah (Founders Edition) sent via Bluetooth!");
    Counter_HelloMsg = 0;
  }

  Counter_HelloMsg++;
  delay(1);
}



