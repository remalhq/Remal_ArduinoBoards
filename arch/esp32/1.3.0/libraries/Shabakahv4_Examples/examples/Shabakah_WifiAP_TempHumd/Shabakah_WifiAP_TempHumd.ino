/**
 * @file    Shabakah_WifiAP_TempHumd.ino
 * @authors Khalid Mansoor AlAwadhi, Remal IoT
 * @date    12 Jan 2024
 * 
 * @brief   This application serves as a straightforward web server, providing real-time temperature and humidity 
 *          data from Shabakah's integrated Temperature/Humidity sensor.
 *          
 *          To access this data, follow these simple steps:
 *            1- Connect to Shabakah's Wi-Fi network (Default name is "Shabakah-WiFi-TempHumd" and password is "12345678")
 *            2- Open a web browser and enter Shabakah's IP address, which is 192.168.4.1 without HTTP or HTTPS
 * 
 *          You'll be able to see a webpage with the current temperature and humidity readings, updated every second. 
 *          As an additional feature, the LEDs on Shabakah blink sequentially (LED2 then LED1) each time a client 
 *          connects and a new reading is obtained, signifying that the webpage has been updated.
 *          
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Remal_SHT3X.h"
#include <WiFi.h>
#include "Adafruit_NeoPixel.h"


/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* RGB LEDs (WS2812B) */
const int LED_1_PIN = 1;                    //The pin connected to LED 1 on Shabakah
const int LED_2_PIN = 3;                    //The pin connected to LED 2 on Shabakah
const int NumLEDs = 1;                      //Number of LEDs on each pin
Adafruit_NeoPixel Shabakah_LED_1(NumLEDs, LED_1_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 1
Adafruit_NeoPixel Shabakah_LED_2(NumLEDs, LED_2_PIN, NEO_GRB + NEO_KHZ800);       //Object to control LED 2

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

  /* Initialize RGB LEDs */
  Shabakah_LED_1.begin();
  Shabakah_LED_1.clear();
  Shabakah_LED_1.show();

  Shabakah_LED_2.begin();
  Shabakah_LED_2.clear();
  Shabakah_LED_2.setBrightness(40);
  Shabakah_LED_2.show();

  Serial.println("> Configuring Shabakah to be an access point...");

  /* You can replace 'YourSSID' and 'YourPassword' with your own name and password: */
  const char *YourSSID = "Shabakah-WiFi-TempHumd";          //Name of your Wi-Fi network
  const char *YourPassword = "12345678";                    //Password for your Wi-Fi network (NOTE: Minimum 8 characters!)
  
  /* Error check: */
  if(strlen(YourPassword) < 8)
  {
    Serial.println("> Wi-Fi password must be at least 8 characters! Please change it in the code and try again!");

    /* Set LED 1 to RED to indicate error */
    Shabakah_LED_1.setPixelColor(0, 255, 0, 0);
    Shabakah_LED_1.show();
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

    /* Set LED 1 to RED to indicate error */
    Shabakah_LED_1.setPixelColor(0, 255, 0, 0);
    Shabakah_LED_1.show();
    while(1)
    {
      //Wait forever
    }
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

    /* Set LED 2 to GREEN to indicate a client is connected: */
    Shabakah_LED_1.setPixelColor(0, 0, 0, 0);
    Shabakah_LED_1.show();
    Shabakah_LED_2.setPixelColor(0, 0, 255, 0);
    Shabakah_LED_2.show();


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
            client.println("<title>Remal IoT - Shabakah Wi-Fi + Sensor Readings Demo</title>");
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
            client.println("<h1>Remal IoT - Shabakah Wi-Fi + Sensor Readings Demo</h1>");
            client.println("<h2>This simple webpage showcases the Wi-Fi capabilities of Shabakah by continuously displaying the temperature and humidity data obtained from its built-in sensor every second</h2>");
            client.println("<h1> &gt;&gt; Temperature: " + String(temp) + " C</h1>");
            client.println("<h1> &gt;&gt; Humidity: " + String(hum) + " %</h1>");
            client.println("</div>");
            client.println("<meta http-equiv='refresh' content='1'>");              //Refresh the page every 1 second
            client.println("</body>");
            client.println("</html>");
            client.println();

            /* Now clear LED 2 and set LED 1 GREEN*/
            Shabakah_LED_2.setPixelColor(0, 0, 0, 0);
            Shabakah_LED_2.show();
            Shabakah_LED_1.setPixelColor(0, 0, 255, 0);
            Shabakah_LED_1.show();
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

  /* Clear LEDs */
  Shabakah_LED_1.setPixelColor(0, 0, 0, 0);
  Shabakah_LED_2.setPixelColor(0, 0, 0, 0);
  Shabakah_LED_1.show();
  Shabakah_LED_2.show();
}
