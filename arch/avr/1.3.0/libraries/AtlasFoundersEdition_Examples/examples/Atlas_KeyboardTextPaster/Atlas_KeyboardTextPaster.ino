/**
 * @file    Atlas_KeyboardTextPaster.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    13 June 2023
 * 
 * @brief   This example asks the user for two text inputs and assigns each string to 
 *          Button 1 or Button 2, respectively. 
 *          The Atlas board then acts as a keyboard emulator; when either Button 1 or 
 *          Button 2 is pressed, the corresponding text is "typed" into the computer.
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Keyboard.h"


/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* Buttons */
const int Atlas_Button_1 = 12;                  //Button 1 - on the left side of the Atlas board
const int Atlas_Button_2 = 10;                  //Button 2 - on the right side of the Atlas board

/* Texts */
String Button1_Text;
String Button2_Text;


void setup() 
{
  /* Start serial communication with a baudrate of 9600 - so we can read output from the PC */
  Serial.begin(9600);

  while (!Serial)
  {
    //Wait until USB serial monitor is opened
  }
	
  /* Init buttons */
	pinMode(Atlas_Button_1, INPUT);
	pinMode(Atlas_Button_2, INPUT); 
	  
	/* Init keyboard control */
	Keyboard.begin();

  /* Get text from user */
  Serial.println("Enter the text you want to attach to Button 1:");
  while (!Serial.available())
  {
    // Wait until data is available in the serial buffer
  }
  Button1_Text = Serial.readStringUntil('\n');

  Serial.println("and now enter the text you want to attach to Button 2:");
  while (!Serial.available()) 
  {
    // Wait until data is available in the serial buffer
  }
  Button2_Text = Serial.readStringUntil('\n');

  Serial.println("Confirmed! Simply press either of those buttons to paste the text on your PC!");
}


void loop()
{
	/* Emulate keyboard by printing the text to the PC based on button presses */
	if(digitalRead(Atlas_Button_1) == HIGH)
	{
		Keyboard.print(Button1_Text);
	}
	
	if(digitalRead(Atlas_Button_2) == HIGH)
	{
		Keyboard.print(Button2_Text);
	}

	/* A delay so we dont keep pasting too fast */
	delay(150);
}
