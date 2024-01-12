/**
 * @file    Shabakah_SerialOutput_UART_USB.ino
 * @authors Khalid Mansoor AlAwadhi, Remal IoT
 * @date    26 March 2023
 * 
 * @brief   This example simply shows how to use the native USB port and the hardware UART port on Shabakah (Founders Edition)
 *          
 *          Make sure you select "Shabakah (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (ESP32)"
 *          and the correct port under "Tools -> Port" 
 */


void setup()
{
    /* This is how you initialize to use native USB as serial output
     *  !! Make sure you set "Tools -> USB CDC on Boot" to "Enabled" !!
     * Additionally, while using native USB serial out there is no need to set the baudrate as it's automatically detected: */
	Serial.begin();
	while(!Serial) 
	{
		/* You can wait until serial is ready when using native USB serial comm,
		 * which means this statement will loop until you connect to Shabakah (Founders Edition)'s USB port,
		 * so make sure the hardware switch is set to USB or you'll be stuck here!
		 *
		 * You can also just remove this while loop. It's only here for reference
		 */
	}
	Serial.println("> Serial USB: Hello from Shabakah (Founders Edition)!");
	
	/* This is how you initialize to use hardware UART as serial output
	 * You need to explicitly define your parameters for UART communicaiton */
	 Serial1.begin(115200, SERIAL_8N1, RX, TX);
	 Serial1.println("> Serial1 UART: Hello from Shabakah (Founders Edition)!");
}

void loop()
{
    Serial.println("> Serial USB: Hello from Shabakah (Founders Edition)!");
    Serial1.println("> Serial1 UART: Hello from Shabakah (Founders Edition)!");
    delay(1000);
}
