/**
 * @file    Shabakah_WifiAP_TempHumd.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    19 July 2023
 * 
 * @brief   This application serves as a straightforward web server, providing real-time temperature and humidity 
 *          data from Shabakah (Founders Edition)'s integrated Temperature/Humidity sensor.
 *          
 *          To access this data, follow these simple steps:
 *            1- Connect to Shabakah's Wi-Fi network (Default name is "ShabakahFE-WiFi-TempHumd" and password is "12345678")
 *            2- Open a web browser and enter Shabakah's IP address, which is 192.168.4.1 without HTTP or HTTPS
 * 
 *          You'll be able to see a webpage with the current temperature and humidity readings, updated every second. 
 *          As an additional feature, the RGB LED on Shabakah blinks each time a client connects and a new reading 
 *          is obtained, signifying that the webpage has been updated.
 *          
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Remal_SHT3X.h"
#include <WiFi.h>
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

/* Temp/Humd sensor */
const int TempHumd_Addr = 0x44;             //The I2C address of the Temp/Humd sensor
SHT3x SHT30_Sensor(TempHumd_Addr);          //Create sensor object
float CurrTemp = 0;                         //Stores current temperature readings
float CurrHumd = 0;                         //Stores current humidity readings 
#define HUMD_THRESHOLD              65.0    //The threshold before turning the LEDs red

/* Wi-Fi */
WiFiServer server(80);                      //Server will run on this port



void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);

  /* Initialize RGB LED */
  Shabakah_RGBLED.begin();
  Shabakah_RGBLED.clear();
  Shabakah_RGBLED.show();

  Serial.println("> Configuring Shabakah (Founders Edition) to be an access point...");

  /* You can replace 'YourSSID' and 'YourPassword' with your own name and password: */
  const char *YourSSID = "ShabakahFE-WiFi-TempHumd";          //Name of your Wi-Fi network
  const char *YourPassword = "12345678";                    //Password for your Wi-Fi network (NOTE: Minimum 8 characters!)
  
  /* Error check: */
  if(strlen(YourPassword) < 8)
  {
    Serial.println("> Wi-Fi password must be at least 8 characters! Please change it in the code and try again!");
    Shabakah_RGBLED.setPixelColor(0, 255, 0, 0);
    Shabakah_RGBLED.show();
    while(1);
  }

  /* Initialize Wi-Fi */
  WiFi.softAP(YourSSID, YourPassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("> Shabakah's IP address: ");
  Serial.println(myIP);
  server.begin();

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
}


void loop()
{
  /* Listen for incoming clients: */
  WiFiClient client = server.available(); 

  /* If a client connects: */
  if (client) 
  {
    Serial.println("> New client");                   //Print a message out the serial port
    String currentLine = "";                          //Create a String to hold incoming data from the client

    /* Set the RGB LED to green to indicate a client is connected: */
    Shabakah_RGBLED.setPixelColor(0, 0, 255, 0);
    Shabakah_RGBLED.show();

    /* Loop while the client's connected: */
    while (client.connected())             
    {
      /* Check if there are any new bytes available from the client: */
      if (client.available())             
      {            
        char c = client.read();                       //Read a byte
        Serial.write(c);                              //Print it out the serial monitor

        /* If the byte is a newline character: */
        if (c == '\n') 
        {                   
          //If the current line is blank, you got two newline characters in a row,
          //that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            //HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            //and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            /* Get current temperature and humidity readings: */
            float temp = SHT30_Sensor.GetTemperatureCelsius();
            float hum = SHT30_Sensor.GetHumidity();

            /* Update web page with current readings: */
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Remal IoT - Shabakah (Founders Edition) Wi-Fi + Sensor Readings Demo</title>");
            client.println("<style>");
            client.println("body {");
            client.println("    font-family: Arial, sans-serif;");
            client.println("    background-color: #f9b432;");
            client.println("    margin: 0;");
            client.println("    padding: 20px;");
            client.println("}");

            client.println("h1, h2 {");
            client.println("    color: black;");
            client.println("    text-align: center;");
            client.println("}");

            client.println(".container {");
            client.println("    max-width: 500px;");
            client.println("    margin: 0 auto;");
            client.println("    background-color: #ffffff;");
            client.println("    padding: 20px;");
            client.println("    border-radius: 5px;");
            client.println("    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);");
            client.println("}");

            client.println("</style>");
            client.println("</head>");

            client.println("<body>");
            client.println("<div class='container'>");
            client.println("<h1>Remal IoT - Shabakah (Founders Edition) Wi-Fi + Sensor Readings Demo</h1>");
            client.println("<h2>This simple webpage showcases the Wi-Fi capabilities of Shabakah (Founders Edition) by continuously displaying the temperature and humidity data obtained from its built-in sensor every second</h2>");
            client.println("<h1> &gt;&gt; Temperature: " + String(temp) + " C</h1>");
            client.println("<h1> &gt;&gt; Humidity: " + String(hum) + " %</h1>");
            client.println("</div>");
            client.println("<meta http-equiv='refresh' content='1'>");              //Refresh the page every 1 second
            client.println("</body>");
            client.println("</html>");
            client.println();

            /* Clear LED */
            Shabakah_RGBLED.setPixelColor(0, 0, 0, 0);
            Shabakah_RGBLED.show();
            break;
          } 
          else 
          {
            currentLine = "";
          }
        }
        else if (c != '\r') 
        {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("> Client disconnected");
  }
}
