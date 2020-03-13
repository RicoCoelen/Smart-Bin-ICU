// servo includes for opening
#include <Servo.h>
Servo servo_test;        //initialize a servo object for the connected servo
int Pin_D1 = 5;
int angle = 0;  

// infra red sensor
int Value_IR_A0 = 0; // variable to store the values from sensor(initially zero)
int Pin_A0 = 0; // infrared analog sensor pin

// liquid crystal display
#include <LiquidCrystal.h>

/*   
 *   first time setup
 */
void setup()
{
  // essentials
  Serial.begin(115200); // starts the serial monitor
  // initiate servo
  servo_test.attach(Pin_D1); // attach the signal pin of servo to d1 of arduino
  // lcd test
  //lcd.begin(16,1);
  //lcd.print("hello, world!");
}

/* 
 *  default loop
 */ 
void loop()
{
  int beefy = detectObject();
  Serial.println(beefy);
//  if (beefy > 800) {
//    openTrashCan();
//  }
//  else {
//    closeTrashCan();
//  }
}

/* 
 *  check infrared sensor if somthing is near
 */
int detectObject () {
  // get distance value max 1024
  Value_IR_A0 = analogRead(Pin_A0);
  return Value_IR_A0;
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
    delay(15);                       
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
    delay(15);                       
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
