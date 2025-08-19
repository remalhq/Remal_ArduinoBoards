/**
 * @file    Shabakah_ButtonPress.ino
 * @authors Khalid Mansoor AlAwadhi, Remal <khalid@remal.io>
 * @date    18 July 2023
 * 
 * @brief   This example serves as a demonstration of how to interface with the two buttons on 
 *          the Shabakah (Founders Edition) board and provides a foundation to build upon, enjoy!
 * 
 *          NOTE: You must open the "Serial Monitor" to run this demo. Click on "Tools -> Serial Monitor"
 *          
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */
 
/*#############################################
 * Shabakah Board Defines and Global Variables 
 *#############################################*/
/* Buttons */
const int Shabakah_Button_Boot = 9;         //Boot Button - on the top right side of the Shabakah board
const int Shabakah_Button = 0;              //Button - on the bottom right side of the Shabakah board


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
  pinMode(Shabakah_Button, INPUT);               //pulled-down and debounce accounted for externally
  pinMode(Shabakah_Button_Boot, INPUT_PULLUP);   //No external pull-up, must be done internally

  Serial.println("> Press Boot button or the lower button, that's it this is the demo :)");
}


void loop()
{
  if( !digitalRead(Shabakah_Button_Boot))         //Notice the "!" before the digitalRead() function
  {
    Serial.println("> Boot button pressed!");

    // You can do stuff here based on the button press
  }

  if( digitalRead(Shabakah_Button) )
  {
    Serial.println("> Lower button pressed!");

    // You can do stuff here based on the button press
  }

  /* Loop every 150 ms*/
  delay(150);
}


