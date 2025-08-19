/**
 * @file    Shabakah_MQTT_SimpleSubscriber.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    16 Mar 2025
 * 
 * @brief   This example demonstrates subscribing to the MQTT broker at "mqtt.remal.io" and listening for messages on the topic "remal/ex/simplemqtt".
 *          When a message is received, the Shabakah board processes the payload and updates LED 1 based on the content of the message:
 *          - If the message is "Hello from Shabakah! Button 1 pressed!", LED 1 blinks with a gold color.
 *          - If the message is "Hello from Shabakah! Button 2 pressed!", LED 1 blinks with a blue color.
 *
 *          LED 2 indicates the status of the Wi-Fi and MQTT connection:
 *          - Green: Connection is successful
 *          - Red: Connection has failed
 *
 *          To observe the effects of this example, run the "Shabakah_MQTT_SimplePublisher" example on another Shabakah to publish messages to the same topic.
 *          The subscriber will receive the published messages and update its LEDs accordingly, creating a synchronized visual effect.
 *          Additionally, you can monitor and send the expected messages in real-time using the MQTT Dashboard at https://mqtt.remal.io.
 *          Connect to the broker and subscribe to the topic "remal/ex/simplemqtt" to view and send messages.
 * 
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
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

/* Remal MQTT credentials */
#define REMAL_MQTT_SERVER   "mqtt.remal.io"
#define REMAL_MQTT_PORT     8885
#define MQTT_TOPIC          "remal/ex/simplemqtt"    // Topic to subscribe to

WiFiClient ESP_Client;                    // WiFi Client
PubSubClient ShabakahClient(ESP_Client);  // MQTT Client
String ClientID = "ShabakahClient-" + String(random(1000, 9999));  // Unique Client ID



/*###############################
 * Function Prototypes
 *###############################*/
void WiFi_Connect();
void MQTT_Connect();
void MQTT_Callback(char* topic, byte* payload, unsigned int length);



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

  /* Connect to WiFi */
  WiFi_Connect();
 
  /* Configure MQTT server and callback */
  ShabakahClient.setServer(REMAL_MQTT_SERVER, REMAL_MQTT_PORT);
  ShabakahClient.setCallback(MQTT_Callback);

  /* Connect to MQTT */
  MQTT_Connect();
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
    MQTT_Connect();
  }

  ShabakahClient.loop();

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
void MQTT_Connect()
{
  const int MaxAttempts = 10;
  int ReconnectAttempts = 0;

  while (!ShabakahClient.connected())
  {
    /* Attempt to connect */
    if (ShabakahClient.connect(ClientID.c_str()))
    {
      // Subscribe to the topic
      ShabakahClient.subscribe(MQTT_TOPIC);

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


/*
 * Callback function triggered when an MQTT message is received on the subscribed topic.
 * The function processes the incoming message and updates LED 1 based on the payload content.
 *   - If the payload is "Hello from Shabakah! Button 1 pressed!", LED 1 blinks with a gold color.
 *   - If the payload is "Hello from Shabakah! Button 2 pressed!", LED 1 blinks with a blue color.
 * The received message and topic are also printed to the Serial Monitor for debugging purposes.
 * 
 * Parameter: topic    The MQTT topic on which the message was received
 * Parameter: payload  The message payload as a byte array
 * Parameter: length   The length of the payload in bytes
 */
void MQTT_Callback(char* topic, byte* payload, unsigned int length)
{
  char PayloadData[length+1] = {0};   // Array to store the payload data

  for (int i = 0; i < length; i++) 
  {
    /* Store each character in the payload */
    PayloadData[i] = (char)payload[i];
    PayloadData[i+1] = '\0';        // Null-terminate the string
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(PayloadData);

  if( strcmp(PayloadData, "Hello from Shabakah! Button 1 pressed!") == 0 )
  {
    /* Blink LED 1 - Gold */
    LED_1.setPixelColor(0, 255, 128, 0);  // R=255, G=128, B=0
    LED_1.show();
    delay(500);
    LED_1.clear();
    LED_1.show();
  }
  else if( strcmp(PayloadData, "Hello from Shabakah! Button 2 pressed!") == 0 )
  {
    /* Blink LED 1 - Blue */
    LED_1.setPixelColor(0, 0, 128, 255);  // R=0, G=128, B=255
    LED_1.show();
    delay(500);
    LED_1.clear();
    LED_1.show();
  }
}
