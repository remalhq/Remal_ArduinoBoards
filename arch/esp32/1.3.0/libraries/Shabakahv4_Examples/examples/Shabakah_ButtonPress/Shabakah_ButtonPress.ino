/**
 * @file    Shabakah_ButtonPress.ino
 * @authors Khalid Mansoor AlAwadhi, Remal IoT
 * @date    12 Jan 2024
 * 
 * @brief   This example serves as a demonstration of how to interface with the two buttons on 
 *          the Shabakah board and provides a foundation to build upon, enjoy!
 * 
 *          NOTE: You must open the "Serial Monitor" to run this demo. Click on "Tools -> Serial Monitor"
 *          
 *          Make sure you select "Shabakah v4" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
 
/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* Buttons */
const int Shabakah_Button_1 = 9;          //Button 1 - on the right side of the Shabakah board
const int Shabakah_Button_2 = 10;         //Button 2 - on the left side of the Shabakah board


void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);
  /* Wait for USB Serial */
  while (!Serial)
  {
    //Wait
  }

  /* Initialize Buttons */
  pinMode(Shabakah_Button_1, INPUT_PULLUP);       //No external pull-up, must be done internally
  pinMode(Shabakah_Button_2, INPUT_PULLUP);       //No external pull-up, must be done internally

  Serial.println("> Press button 1 or 2, that's it this is the demo :)");
}


void loop()
{
  if( digitalRead(Shabakah_Button_1) == 0 )
  {
    Serial.println("> Button 1 pressed!");

    // You can do stuff here based on the button press
  }

  if( digitalRead(Shabakah_Button_2) == 0 )
  {
    Serial.println("> Button 2 pressed!");

    // You can do stuff here based on the button press
  }

  /* Loop every 150 ms*/
  delay(150);
}


