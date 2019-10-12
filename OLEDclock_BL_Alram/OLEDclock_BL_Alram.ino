/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int hour=0, minute=0, second=0, day=1, month=1, week=1;
int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
String w[7] = {"Mon  ", "Tue  ", "Wed  ", "Thu  ", "Fri  ", "Sat  ", "Sun  "};
const int inter_time = 1000;
int alarm_hour=12, alarm_minute=0;
bool NL = true;
int i = 0;
RTC_DS1307 RTC;
SoftwareSerial BTserial(2, 3); // RX | TX

void setup() {
  Serial.begin(9600);
  // HC-05 default serial speed for AT mode is 38400
  BTserial.begin(38400);

  Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(WHITE);
//  display.setCursor(0, 0);
  // Display static text
//  display.println("Hello!123");
//  display.display(); 
}

void loop() {
  if(hour == alarm_hour && minute == alarm_minute){
      Serial.println("Alarm!!!!!!");
//      play(melody, noteDurations, sizeof(melody) / sizeof(int));
  }
  
  display.clearDisplay();
  DateTime now = RTC.now(); 
  printTime(now);

  if (BTserial.available()){  
        i = BTserial.read();
        Serial.println(i);
        if(i == 100){  //100: start edit time
          editTime();
        }
        if(i == 200){ //200: set alarm
          setAlarm();
        }
    }
}

void setAlarm(){
  while(i != 101){ //101: end set alarm
//    printTime(); //or display something indicating setAlarm
    if (BTserial.available()){
      i = BTserial.read();
      Serial.println(i);
    }
    if(i == 202){ //202: set alarm time
      int n = 0;
      while(n < 2){
        if (BTserial.available()){
          i = BTserial.read();
          Serial.println(i);
          n++;
        }
        if(n == 1){
          alarm_hour = i;
          Serial.println(alarm_hour);
        }
        else if(n == 2){
          alarm_minute = i;
          Serial.println(alarm_minute);
        }
      }
    }
  }
}

void editTime(){
  printTime(RTC.now());
  while(i != 101){ // 101: end edit time
    if (BTserial.available()){
      i = BTserial.read();
      Serial.println(i);
    }
    if(i == 102){ //102: edit clock time
      int n = 0;
      while(n < 2){
        if (BTserial.available()){
          i = BTserial.read();
          Serial.println(i);
          n++;
        }
        if(n == 1){
          hour = i;
          Serial.println(hour);
        }
        else if(n == 2){
          minute = i;
          Serial.println(minute);
        }
        second = 0;
      }
    }
    else if(i == 103){ //103: edit clock date
      int n = 0;
      while(n < 2){
        if(BTserial.available()){
          i = BTserial.read();
          Serial.println(i);
          n++;
        }
        if(n == 1){
          month = i;
          Serial.println(month);
        }
        else if(n == 2){
          day = i;
          Serial.println(day);
        }
      }
    }
    else if(i == 104){ //104: edit weekday
      if (BTserial.available()){
          i = BTserial.read();
          Serial.println(i);
      }
      delay(300);
      week = i;
      Serial.println(w[week-1]);
    }
  }
}

void printTime(DateTime now){
  display.setCursor(0, 0);
  display.setTextSize(3);
  if(now.hour()<10){
    display.print("0");
    display.print(now.hour());
  }
  else{
    display.print(now.hour()); 
  }
//  display.setTextSize(1);
  display.print(":");
//  display.setTextSize(3);

  if(now.minute()<10){
    display.print("0");
    display.print(now.minute());
  }
  else{
    display.print(now.minute()); 
  }
  display.setTextSize(1);
  display.print(" ");
//  display.setTextSize(3);
  display.setTextSize(2);
  if(now.second()<10){
    display.print("0");
    display.print(now.second());
  }
  else{
    display.print(now.second()); 
  }
  
  display.setCursor(0, 33);
  display.setTextSize(2);
  display.print(now.month());
  display.print("/");
  display.print(now.day());
  display.print(" ");
  display.print(w[week-1]);

  display.display();
}
