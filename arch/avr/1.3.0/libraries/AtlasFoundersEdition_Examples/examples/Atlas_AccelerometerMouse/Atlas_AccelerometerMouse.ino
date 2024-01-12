/**
 * @file    Atlas_AccelerometerMouse.ino
 * @authors Khalid AlAwadhi, Remal IoT
 * @date    16 April 2023
 * 
 * @brief   This example shows how to use the Atlas board Accelerometer to control the mouse
 * 			    movement on your computer. The X and Y axis of the accelerometer are used to
 * 			    control the X and Y axis of the mouse. The left and right buttons are used to
 * 			    perform left and right clicks.
 *          
 *          Make sure you select "Atlas (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */
#include "Mouse.h"
#include "LIS3DHTR.h"

/*#############################################
 * Atlas Board Defines and Global Variables 
 *#############################################*/
/* Buttons */
const int Atlas_Button_1 = 12;                  //Button 1 - on the left side of the Atlas board
const int Atlas_Button_2 = 10;                  //Button 2 - on the right side of the Atlas board

/* Accelerometer */
const int Accel_Addr = 0x18;            //The I2C address of the accelerometer
LIS3DHTR<TwoWire> LIS_Accel;            //Accelerometer object

/*
 * You can try changing the values below and see how they affect your Atlas board mouse
 */
int Range = 10;				                  //Output range of X or Y movement; affects movement speed
int ResponseDelay = 10;		              //Response delay of the mouse, in ms




void setup() 
{
	/* Init buttons */
	pinMode(Atlas_Button_1, INPUT);    //Button 1 - Left click
	pinMode(Atlas_Button_2, INPUT);    //Button 2 - Right click
	  
	/* Init mouse control */
	Mouse.begin();
	
	/* Init accel */
	LIS_Accel.begin(Accel_Addr);
	while(!LIS_Accel)
	{
		delay(500);
		if (LIS_Accel)
		{
			break;
		}
	}
	delay(100);
	LIS_Accel.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
}

void loop()
{
	/* Check if we have a left or right mouse clicks */
	if(digitalRead(Atlas_Button_1) == HIGH)
	{
		Mouse.press(MOUSE_LEFT);
	}
	else
	{
		Mouse.release(MOUSE_LEFT);
	}
	
	if(digitalRead(Atlas_Button_2) == HIGH)
	{
		Mouse.press(MOUSE_RIGHT);
	}
	else
	{
		Mouse.release(MOUSE_RIGHT);
	}
	
	/* Get X, Y values: */
	float X_Value = LIS_Accel.getAccelerationX() * Range;
	float Y_Value = -1 * LIS_Accel.getAccelerationY() * Range;
	
	Mouse.move(X_Value, Y_Value, 0);
	
	//A delay so the mouse doesn't move too fast:
	delay(ResponseDelay);
}
