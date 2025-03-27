/**
 * @file    Shabakah_MQTT_SimplePublisher.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    11 Mar 2025
 * 
 * @brief   This example demonstrates connecting to the MQTT broker at "mqtt.remal.io" and publishing messages to the topic "remal/simplemqtt/example" 
 *          based on button presses. LED 1 blinks with a color that depends on the button pressed, providing visual feedback for the button action.
 *          LED 2 indicates the status of the Wi-Fi and MQTT connection:
 *          - Green: Connection is successful
 *          - Red: Connection has failed
 *
 *          To observe the effects of this example, run the "Shabakah_MQTT_SimpleSubscriber" example on another Shabakah to subscribe to the same topic.
 *          The subscriber will receive the published messages and update its LEDs accordingly, creating a synchronized visual effect.
 *          Additionally, you can monitor the messages in real-time using the MQTT Dashboard at https://mqtt.remal.io.
 *          Connect to the broker and subscribe to the topic "remal/simplemqtt/example" to view the messages.
 *          
 *          Ensure "Shabakah v4" is selected under "Tools -> Boards -> Remal IoT Boards (ESP32)" 
 *          and the correct port is selected under "Tools -> Port". 
 */
#include <WiFi.h>               // Wi-Fi Library
#include "Adafruit_NeoPixel.h"  // NeoPixel Library for controlling RGB LEDs
#include <PubSubClient.h>       // MQTT Library for publishing/subscribing to topics
 

/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* WiFi credentials - Change this! */
#define WIFI_SSID      "Your_WiFi_SSID"     // Change to your Wi-Fi SSID
#define WIFI_PASS      "Your_WiFi_Password" // Change to your Wi-Fi Password

/* RGB LEDs (WS2812B) */
const int LED_1_PIN = 1; // LED 1 - Middle of Shabakah
const int LED_2_PIN = 3; // LED 2 - Bottom left of Shabakah
const int NumLEDs = 1;   // Number of LEDs per pin
Adafruit_NeoPixel LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);
 
/* Buttons */
const int Button_1 = 9;  // Right-side button
const int Button_2 = 10; // Left-side button

/* Remal MQTT credentials */
#define REMAL_MQTT_SERVER   "mqtt.remal.io"
#define REMAL_MQTT_PORT     8885
#define PUBLISH_TOPIC       "remal/simplemqtt/example"    // Topic to publish messages to

WiFiClient ESP_Client;                    // WiFi Client
PubSubClient ShabakahClient(ESP_Client);  // MQTT Client
String ClientID = "ShabakahClient-" + String(random(1000, 9999));  // Unique Client ID



/*###############################
 * Function Prototypes
 *###############################*/
void WiFi_Connect();
void MQTT_Reconnect();



/* Setup Function */
void setup()
{
  Serial.begin(9600);         // Initialize serial communication for debugging
  Serial.setTxTimeoutMs(0);   // Avoid waiting if the Serial Monitor is not connected 
 
  /* Initialize LEDs */
  LED_1.begin();              // Initialize LED 1
  LED_2.begin();              // Initialize LED 2
  LED_1.clear();              // Clear LED 1
  LED_2.clear();              // Clear LED 2
  LED_1.setBrightness(100);   // Set LED 1 brightness to 100
  LED_2.setBrightness(20);    // Set LED 2 brightness to 20
  LED_1.show();               // Show LED 1
  LED_2.show();               // Show LED 2
 
  /* Initialize Buttons 
   * Internal Pullup Configuration - Button closes to GND when pressed
   */
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(Button_2, INPUT_PULLUP);
 
  /* Connect to WiFi */
  WiFi_Connect();
 
  /* Configure MQTT */
  ShabakahClient.setServer(REMAL_MQTT_SERVER, REMAL_MQTT_PORT);
  
  /* Connect to MQTT */
  MQTT_Reconnect();
}

 
/* Main Loop */
void loop()
{
  /* Ensure WiFi Connection */
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi disconnected x_x, attempting reconnection...");
    WiFi_Connect();
  }
 
  /* Ensure MQTT Connection */
  if (!ShabakahClient.connected())
  {
    Serial.println("MQTT disconnected x_x, attempting reconnection...");
    MQTT_Reconnect();
  }

  ShabakahClient.loop();    // Maintain MQTT connection
 
  /* Check which button is pressed and publish message to MQTT */
  if (digitalRead(Button_1) == LOW) 
  {
    Serial.println("Button 1 pressed, sent MQTT message!");
    ShabakahClient.publish(PUBLISH_TOPIC, "Hello from Shabakah! Button 1 pressed!");
 
    /* Blink LED 1 - Gold */
    LED_1.setPixelColor(0, 255, 128, 0);  // R=255, G=128, B=0
    LED_1.show();
    delay(500);
    LED_1.clear();
    LED_1.show();
  }
  else if (digitalRead(Button_2) == LOW) 
  {
    Serial.println("Button 2 pressed, sent MQTT message!");
    ShabakahClient.publish(PUBLISH_TOPIC, "Hello from Shabakah! Button 2 pressed!");
 
    /* Blink LED 1 - Blue */
    LED_1.setPixelColor(0, 0, 128, 255);  // R=0, G=128, B=255
    LED_1.show();
    delay(500);
    LED_1.clear();
    LED_1.show();
  }
  
  delay(1);
}
 


/* 
 * This function establishes WiFi connection. 
 * LED 2 indicates the connection status:
 *  - Green: Connection successful
 *  - Red: Connection failed
 * Soft-resets board after a certain number of failed connection attempts
 */
void WiFi_Connect()
{
  Serial.println("Connecting to WiFi SSID: " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  const int MaxAttempts = 10;
  int ReconnectAttempts = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    LED_2.setPixelColor(0, 255, 0, 0); // Red LED while connecting
    LED_2.show();
    delay(500);
    LED_2.clear();
    LED_2.show();
    delay(500);

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected to WiFi! ^_^");
      LED_2.setPixelColor(0, 0, 255, 0); // Green LED
      LED_2.show();
      return;
    }
    else
    {
      ReconnectAttempts++;

      if(ReconnectAttempts >= MaxAttempts)
      {
        Serial.println("\nFailed to connect to " + String(WIFI_SSID) + " after " + String(MaxAttempts) + " attempts x_x.. Please check your Wi-Fi credentials. Restarting Shabakah in 5 seconds...");
        while (1)
        {
          int Loop = 0;
          while (Loop < 5)
          {
            LED_1.setPixelColor(0, 255, 0, 0);
            LED_2.setPixelColor(0, 255, 0, 0);
            LED_1.show();
            LED_2.show();
            delay(500);
            LED_1.clear();
            LED_2.clear();
            LED_1.show();
            LED_2.show();
            delay(500);
            Loop++;
          }
          esp_restart();    // Soft-restart Shabakah
        }
      }
    }
  }
}


/* 
 * This function establishes MQTT connection. 
 * LED 2 indicates the connection status:
 *  - Green: Connection successful
 *  - Red: Connection failed
 * Soft-resets board after a certain number of failed connection attempts
 */
void MQTT_Reconnect()
{
  const int MaxAttempts = 10;
  int ReconnectAttempts = 0;

  while (!ShabakahClient.connected())
  {
    /* Attempt to connect */
    if (ShabakahClient.connect(ClientID.c_str()))
    {
      Serial.println("Connected to MQTT server! ^_^");
      LED_2.setPixelColor(0, 0, 255, 0); // Green LED
      LED_2.show();
      return;
    }
    else
    {
      ReconnectAttempts++;

      Serial.println("Failed to connect to MQTT server x_x, retrying in 1 second...");
      LED_2.setPixelColor(0, 255, 0, 0); // Red LED
      LED_2.show();
      delay(1000);

      if(ReconnectAttempts >= MaxAttempts)
      {
        Serial.println("Failed to connect to MQTT server after " + String(MaxAttempts) + " attempts x_x.. Restarting Shabakah in 5 seconds..");
        while (1)
        {
          int Loop = 0;
          while (Loop < 5)
          {
            LED_1.setPixelColor(0, 255, 0, 0);
            LED_2.setPixelColor(0, 255, 0, 0);
            LED_1.show();
            LED_2.show();
            delay(500);
            LED_1.clear();
            LED_2.clear();
            LED_1.show();
            LED_2.show();
            delay(500);
            Loop++;
          }
          esp_restart();    // Soft-restart Shabakah
        }
      }
    }
  }
}
