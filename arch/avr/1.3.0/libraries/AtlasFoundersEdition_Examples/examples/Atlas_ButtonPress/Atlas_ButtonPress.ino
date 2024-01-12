/**
 * @file    Atlas_ButtonPress.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    13 June 2023
 * 
 * @brief   This example serves as a demonstration of how to interface with the two buttons on 
 *          the Atlas board and provides a foundation to build upon, enjoy!
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
 
/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* Buttons */
const int Atlas_Button_1 = 12;                  //Button 1 - on the left side of the Atlas board
const int Atlas_Button_2 = 10;                  //Button 2 - on the right side of the Atlas board


void setup()
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);
  /* Wait for USB Serial */
  while (!Serial)
  {
    //Wait
  }

  /* Initialize Buttons 1 and 2 */
  pinMode(Atlas_Button_1, INPUT);               //pulled-down and debounce accounted for externally
  pinMode(Atlas_Button_2, INPUT);               //pulled-down and debounce accounted for externally

  Serial.println("> Press button 1 or 2, that's it this is the demo :)");
}


void loop()
{
  if( digitalRead(Atlas_Button_1) )
  {
    Serial.println("> Button 1 pressed!");

    // You can do stuff here based on the button press
  }

  if( digitalRead(Atlas_Button_2) )
  {
    Serial.println("> Button 2 pressed!");

    // You can do stuff here based on the button press
  }

  /* Loop every 150 ms*/
  delay(150);
}


