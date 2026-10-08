#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#include <WiFi.h>
#include "time.h"
#include "TOTP.h"

#include <ESP32Servo.h>

const byte ROWS = 4; /* four rows */
const byte COLS = 4; /* four columns */
/* define the symbols on the buttons of the keypads */
char hexaKeys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};



byte rowPins[ROWS] = {14, 27, 26, 25};  /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {33, 32, 23, 19}; /* connect to the column pinouts of the keypad */

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

String num = "";
String code = "";

const char* ssid     = ""; /* add name of wifi network here */
const char* password = ""; /* add wifi password here*/

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

uint8_t hmacKey[] = {0x68, 0x61, 0x70, 0x70, 0x79, 0x62, 0x64, 0x61, 0x79, 0x79};
TOTP totp = TOTP(hmacKey, 10);

Servo myservo;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  lcd.setCursor(0, 0);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConnected to WiFi!");

  // Configure NTP
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting for NTP time sync...");

  time_t now;
  while (true) {
    now = time(NULL);
    if (now > 100000) {   // time is valid
      break;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nTime synced!");
  Serial.println(ctime(&now));


  myservo.attach(13);

  myservo.write(90); 
}

void getCode(){
  time_t now = time(NULL);
  //Serial.printf("%lld\n", (long long)now);
  code = totp.getCode(now);
  //Serial.println(code);
}



void loop() {
  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  getCode();
  //Serial.println(code);

  if (customKey){
    Serial.println(customKey);
  }
  if(customKey == '1'){
    num = num + "1";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '2'){
    num = num + "2";
    lcd.clear();
    lcd.print(num); 
  }
  if(customKey == '3'){
    num = num + "3";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '4'){
    num = num + "4";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '5'){
    num = num + "5";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '6'){
    num = num + "6";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '7'){
    num = num + "7";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '8'){
    num = num + "8";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '9'){
    num = num + "9";
    lcd.clear();
    lcd.print(num);
  }
  if(customKey == '0'){
    num = num + "0";
    lcd.clear();
    lcd.print(num);
  }
  if(num == code) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pin correct");
    num = "";
    myservo.write(180);
  }
  if(customKey == 'C'){
    lcd.clear();
    lcd.setCursor(0,0);
    num = "";
  }
  if(customKey == 'A'){
    myservo.write(90);
    lcd.clear();
    lcd.setCursor(0,0);
    num = "Locked";
    lcd.print(num);
  }
  
}
