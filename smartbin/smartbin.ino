/*   
 *   servo 
 */
#include <Servo.h>
Servo servo_test;        //initialize a servo object for the connected servo
const int Pin_D0 = 16;
int angle = 0;

/*   
 *   load cell using a hx711 amplifier (circuit wiring)
 */
#include "HX711.h"
HX711 scale;
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 14;

/*   
 *   ultrasonic sensor
 */
const unsigned int TRIG_PIN=13;
const unsigned int ECHO_PIN=15;

/*   
 *   infra red sensor
 */
#define ir A0
long Value_IR_A0 = 0;

/*   
 *   liquid crystal display with i2c backpack (in progress)
 */
#include <LiquidCrystal_I2C.h>

/*   
 *   i2c communication
 */
#include <Wire.h>

/*   
 *   variables
 */
 bool runOnce = true;

/*   
 *   first time setup
 */
void setup()
{
  // essentials
  Serial.begin(115200); // starts the serial monitor
  //servo_test.attach(Pin_D0); // attach the signal pin of servo to d1 of arduino
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // initiate scale with the correct pins
  // set pins for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

/* 
 *  default loop
 */ 
void loop()
{
 
}

/* 
 *  check infrared sensor if somthing is near
 */
long getIrDistance() {
  // get distance value max 1024
  Value_IR_A0 = analogRead(ir);
  return analogRead(ir);
}

/* 
 *  use ultrasonic sensor to get height or fullness 
 */
long getUssDistance() {
  // send pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // calculate distance using speed of sound
  const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
  int distance= duration/29/2;
  
  if(duration==0){
   Serial.println("Warning: no pulse from sensor");
   return 0;
  } 
  else{
      Serial.print("distance to nearest object:");
      Serial.print(distance);
      Serial.println(" cm");
      return distance;
  }
}

/* 
 *  get weight of the current depository
 */
long getWeight() {
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    return reading;
  } else {
    Serial.println("HX711 not found.");
    return 0;
  }
}


/* 
 *  open can, wait for trash
 */
void catchTrash() {

}

/* 
 *  open can, wait for trash
 */
void openTrashCan() {
  for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {      
    Serial.println(angle);                            
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(50);                       
  } 
}

/* 
 *  function
 */
void closeTrashCan() {
  for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {      
    Serial.println(angle);                            
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(50);                       
  } 
}

/* 
 *  check the height of the pile using using infrared sensor or ultrasonic sensor
 */
void checkHeight() {

}

/* 
 *  check the weight of the pile using a load sensor
 */
void checkWeight() {
  
}

/* 
 *  display nessecary information on lcd display // still difficult
 */
void displayLCD() {
 
}

/* 
 *  reset trash weight and height
 */
void resetSettings() {

}
