#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h> // liquid crystal display with i2c backpack (in progress)
#include <Wire.h> //i2c communication
#include <Servo.h> 
#include "HX711.h" // load cell using a hx711 amplifier
#include "settings.h" 
#include "config.h" 

HX711 scale; // load cell amp
Servo servo_test; //initialize a servo object
int servo_angle = 0;
long oldTime = 0;
String chipID;
String serverURL = SERVER_URL;
long currentMillis = 0;
LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x3F for a 16 chars and 2 line display

/*   
 *   connect to internet
 */
void connectToDefault() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(BACKUP_SSID, BACKUP_PASSWORD);

  int timer = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    timer += 500;
    if (timer > 10000)
      break;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

/*   
 *   first time setup
 */
void setup()
{
  // essentials
  Serial.begin(115200); // starts the serial monitor
  servo_test.attach(SERVO_PIN); // attach the signal pin of servo to d1 of arduino
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // initiate scale with the correct pins
  
  // set pins for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFiManager wifiManager;
  chipID = generateChipID();
  String configSSID = String(CONFIG_SSID) + "_" + chipID;
  
  connectToDefault(); 
  wifiManager.autoConnect(configSSID.c_str());

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hey, je ziet er leuk uit vandaag!");
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("Nee grapje, ga naar je vuilnisbak :)");
    lcd.setCursor(0, 2);
  // print message
  lcd.print("Hey, je ziet er leuk uit vandaag!");
  // set cursor to first column, second row
  lcd.setCursor(0,3);
  lcd.print("Nee grapje, ga naar je vuilnisbak :)");
  
  

 

  //HTTPClient http;
  //http.begin(serverURL + "add_device.php?device_id=" + chipID);
  //uint16_t httpCode = http.GET();
  //http.end();
}

/* 
 *  default loop
 */ 
void loop()
{
 //Serial.println(getWeight());
 //delay(1000);

  currentMillis = millis();
  //Every requestDelay, send a request to the server
  if (currentMillis > oldTime + REQUEST_DELAY)
  {
    Serial.println("");
    Serial.println("/ Sending Diaganostics /");
    sendSensorData();
    oldTime = currentMillis;
  }
}

void sendSensorData() {
  Serial.print("Weight: "); Serial.println(getWeight());
  Serial.print("Height: "); Serial.println(getUssDistance());
  Serial.print("IR Distance: "); Serial.println(getIrDistance());
}

/* 
 *  check infrared sensor if somthing is near
 */
long getIrDistance() {
  // get distance value max 1024
  return analogRead(IR_PIN);
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
      // distance in cm
      return distance;
  }
}

/* 
 *  get weight of the current depository
 */
long getWeight() {
  if (scale.is_ready()) {
    long reading = scale.read();
    return reading;
  } else {
    Serial.println("HX711 not found.");
    return 0;
  }
}

/* 
 *  open can, wait for trash
 */
void openTrashCan() {
  for(servo_angle = 0; servo_angle < 180; servo_angle += 1)    // command to move from 0 degrees to 180 degrees 
  {      
    Serial.println(servo_angle);                            
    servo_test.write(servo_angle);                 //command to rotate the servo to the specified angle
    delay(50);                       
  } 
}

/* 
 *  close can
 */
void closeTrashCan() {
  for(servo_angle = 0; servo_angle < 180; servo_angle += 1)    // command to move from 0 degrees to 180 degrees 
  {      
    Serial.println(servo_angle);                            
    servo_test.write(servo_angle);                 //command to rotate the servo to the specified angle
    delay(50);                       
  } 
}

/* 
 *  generate chip id 
 */
String generateChipID()
{
  String chipIDString = String(ESP.getChipId() & 0xffff, HEX);

  chipIDString.toUpperCase();
  while (chipIDString.length() < 4)
    chipIDString = String("0") + chipIDString;

  return chipIDString;
}
