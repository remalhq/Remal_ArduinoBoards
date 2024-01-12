/**
 * @file    Shabakah_Bluetooth_Serial.ino
 * @authors Khalid Mansoor AlAwadhi, Remal IoT
 * @date    12 Jan 2023
 * 
 * @brief   This example shows how to use the Remal BLE (Bluetooth Low Energy) Serial library to send and receive data over Bluetooth LE, it 
 *          is meant to be used with the Shabakah board (ESP32-C3)
 *          
 *          This example will:
 *            - Display any data received from the connected Bluetooth device on the Serial Monitor
 *            - Every 1 second, a message will be sent to the connected device
 *            - LED 1 on Shabakah will be red when not connected to a device, and green when connected to a device
 *            - LED 2 on Shabakah will be blue when receiving data from a connected device
 *          
 *          To send or receive data from the Shabakah board, you can one of the following apps:
 * 				    - BLE Terminal HM-10 (iOS): https://apps.apple.com/us/app/ble-terminal-hm-10/id1398703795
 * 				    - Serial Bluetooth Terminal (Android): https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en&gl=US
 *
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Remal_BLE_Serial.h"
#include "Adafruit_NeoPixel.h"



/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LED (WS2812B) */
const int LED_1_PIN = 1;                    //The pin connected to RGB LED 1 on Shabakah
const int LED_2_PIN = 3;                    //The pin connected to RGB LED 2 on Shabakah
const int NumLEDs = 1;                      //Number of LEDs on each pin
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2

/* Bluetooth */
BLESerial BT_Device;

/* Loop timing counters */
int Counter_HelloMsg = 0;                   //Used to run certain actions at a predetermined interval



void setup()
{
  /* Initialize the Serial Monitor */
  Serial.begin(115200);

  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();
  
  Shabakah_LED_2.begin();
  Shabakah_LED_2.clear();
  Shabakah_LED_2.show();

  /* Initialize the Bluetooth */
  BT_Device.Init("Shabakah");
}


void loop()
{
  /* Check if we are connected to a device */
  if( BT_Device.IsConnected() )
  {
    /* If we are connected, turn on the LED 1 to GREEN */
    Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(0, 255, 0));
    Shabakah_LED_1.show();

    /* Check if we have any data available to read */
    while( BT_Device.Data_Available() > 0 )
    {
      /* While getting data turn LED 2 BLUE */
      Shabakah_LED_2.setPixelColor(0, Shabakah_LED_2.Color(0, 0, 255));
      Shabakah_LED_2.show();
      
      /* Get the data */
      String Data = BT_Device.Get_Data();
      Serial.print("Data received: ");
      Serial.println(Data);

      /* Clear LED 2 */
      Shabakah_LED_2.clear();
      Shabakah_LED_2.show();
    }
  }
  else
  {
    /* If we are not connected, turn on LED 1 to RED */
    Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(255, 0, 0));
    Shabakah_LED_1.show();
  }

  if(Counter_HelloMsg >= 1000)
  {
    /* Send data to the connected device */
    BT_Device.Send_Data("Hello, World! This is a message from Shabakah sent via Bluetooth!\r\n");
    Counter_HelloMsg = 0;
  }

  Counter_HelloMsg++;
  delay(1);
}