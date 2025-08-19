/**
 * @file    Shabakah_Bluetooth_LEDControl.ino
 * @author  Mohammed Hani Ahmed, Remal <mhani@remal.io>
 * @date    19-08-2025
 * 
 * @brief   This example demonstrates how to control Shabakah's LEDs via Bluetooth Low Energy (BLE) communication.
 *          The program creates a BLE server named "Shabakah" that receives color commands from connected devices.
 *        
 *          This example will:
 *            - LED 2 shows connection status: red when disconnected, green when connected to a BLE device
 *            - LED 1 changes color based on received commands: "RED", "GREEN", "BLUE" or "WHITE" (case-insensitive)
 *            - LED 1 turns off when the command "OFF" is received
 *            - Responds with confirmation messages for each command received
 * 
 *          To send commands to the Shabakah board, you can use one of the following apps:
 *            - (iOS - Paid) BLE Terminal HM-10: https://apps.apple.com/us/app/ble-terminal-hm-10/id1398703795
 *            - (iOS - Free) Bluefruit Connect: https://apps.apple.com/us/app/bluefruit-connect/id830125974
 *            - (Android - Free) Serial Bluetooth Terminal: https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en&gl=US
 *
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal Boards (ESP32)"
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
int LED_Brightness = 50;                    //Controls LED brightness (Range is from 0 to 255)
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2

/* Bluetooth */
BLESerial BT_Device;


void setup()
{
  /* Initialize the Serial Monitor */
  Serial.begin(115200);

  /* Initialize the RGB to be off, using the NeoPixel library */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.setBrightness(LED_Brightness);
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();
  
  Shabakah_LED_2.begin();
  Shabakah_LED_2.setBrightness(LED_Brightness);
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
    /* If we are connected, turn on the LED 2 to GREEN */
    Shabakah_LED_2.setPixelColor(0, Shabakah_LED_2.Color(0, 255, 0));
    Shabakah_LED_2.show();

    /* Check if we have any data available to read */
    while( BT_Device.Data_Available() > 0 )
    { 
      /* Get the data */
      String Data = BT_Device.Get_Data();
      Serial.print("Data received: ");
      Serial.println(Data);

      /* Convert the data to upper case */
      Data.toUpperCase();

      /* Trim the data to remove any leading/trailing whitespace */
      Data.trim();
 
      /* Based on the data received, change the color of LED 1 */
      if(Data == "RED")
      {
        Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(255, 0, 0));
        BT_Device.Send_Data("LED 1 set to red! ^_^\r\n");
      }
      else if(Data == "GREEN")
      {
        Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(0, 255, 0));
        BT_Device.Send_Data("LED 1 set to green! ^_^\r\n");
      }
      else if(Data == "BLUE")
      {
        Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(0, 0, 255));
        BT_Device.Send_Data("LED 1 set to blue! ^_^\r\n");
      }
      else if (Data == "WHITE")
      {
        Shabakah_LED_1.setPixelColor(0, Shabakah_LED_1.Color(255, 255, 255));
        BT_Device.Send_Data("LED 1 set to white! ^_^\r\n");
      }
      else if(Data == "OFF")
      {
        Shabakah_LED_1.clear(); // Turn off LED 1
        BT_Device.Send_Data("LED 1 turned off! ^_^\r\n");
      }
      else
      {
        /* Respond with color not recognized */
        BT_Device.Send_Data("Color not recognized! x_x\r\n");
      }
      Shabakah_LED_1.show();    // Update LED 1
      delay(1);                 // Small delay to allow the LED to update

      /* Clear LED 2 */
      Shabakah_LED_2.clear();
      Shabakah_LED_2.show();
    }
  }
  else
  {
    /* If we are not connected, turn on LED 2 to RED */
    Shabakah_LED_2.setPixelColor(0, Shabakah_LED_2.Color(255, 0, 0));
    Shabakah_LED_2.show();
  }

  delay(1);
}
