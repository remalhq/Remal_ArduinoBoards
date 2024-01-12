/**
 * @file    Nabd_DemoCode.ino
 * @author  Khalid AlAwadhi, Remal IoT
 * @date    16 December 2022
 * 
 * @brief   This is a very simple example code showcasing Nabd's LED blinking in 
 *          a heartbeat pattern alongside serial output every 1.2 seconds
 *        
 *          Make sure you select "Nabd (Founders Edition)" under "Tools -> Boards -> Remal IoT Boards (AVR)"
 *          and the correct port under "Tools -> Port" 
 */


#define HEARTBEAT_PULSE_MS      115       //Changing this value controls the rate of the heartbeat pattern, it's in milliseconds


void setup()
{
  /* Initialize the LED, set it as an output */
  pinMode(LED_BUILTIN, OUTPUT);

  /* Start serial communication with a baudrate of 115200 - so we can read output from the PC */
  Serial.begin(115200);
}

void loop()
{
  /* Output message - You should see the TX LED blink which indicates this message being sent to the PC */
  Serial.println("> Nabd Founders Edition (v3.1) is alive! Hello, World!");

  /* Blink LED in heartbeat pattern */
  digitalWrite(LED_BUILTIN, HIGH);
  delay(HEARTBEAT_PULSE_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(HEARTBEAT_PULSE_MS);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(HEARTBEAT_PULSE_MS);
  digitalWrite(LED_BUILTIN, LOW);

  /* Wait 1200 milliseconds and start again from the top */
  delay(1200);  
}
