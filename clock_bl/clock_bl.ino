// Basic Bluetooth sketch HC-05_AT_MODE_01
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to commincation mode when first powered on you will need to manually enter AT mode
// The default baud rate for AT mode is 38400
// See www.martyncurrey.com for details

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.

const int rs = 13, en = 12, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int hour=0, minute=0, second=0, day=1, month=1, week=1;
int alarm_hour=12, alarm_minute=0;
const int inter_time = 1000;
int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char w[7][5] = {{"Mon  "}, {"Tue  "}, {"Wed  "}, 
                {"Thu  "}, {"Fri  "}, {"Sat  "}, {"Sun  "}};
                
char c = ' ';
int i = 0;
bool NL = true;
 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    // HC-05 default serial speed for AT mode is 38400
    BTserial.begin(38400);  

    cli();
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  //OCR1A = 100;
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();

  // put your setup code here, to run once:
  lcd.begin(8, 2);
  Serial.begin(9600);
    
}
 
void loop()
{
    printTime();

    if(hour == alarm_hour && minute == alarm_minute){
      Serial.println("Alarm!!!!!!");
    }
    
    // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
//        c = BTserial.read();
//        Serial.write(c);
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
    printTime();
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
  while(i != 101){ // 101: end edit time
    printTime();
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

ISR(TIMER1_COMPA_vect){  //change the 0 to 1 for timer1 and 2 for timer2
   //interrupt commands here
   addTime();
}

void addTime(){
  second++;
  if(second>=60){
    second=0;
    minute++;
  }
  if(minute>=60){
    minute=0;
    hour++;
  }
  if(hour>=24){
    hour=0;
    day++;
    week++;
  }
  if(week>7){
    week=1;
  }
  if(day>m[month-1]){
    day=1;
    month++;
  }
  if(month>12){
    month=1;
  }
}

void printTime(){
  lcd.setCursor(0, 0);
  if(hour<10){
    lcd.print("0");
    lcd.print(hour);
  }
  else{
    lcd.print(hour); 
  }
  lcd.print(":");

  if(minute<10){
    lcd.print("0");
    lcd.print(minute);
  }
  else{
    lcd.print(minute); 
  }
  lcd.print(":");

  if(second<10){
    lcd.print("0");
    lcd.print(second);
  }
  else{
    lcd.print(second); 
  }
  lcd.setCursor(0, 1);
  lcd.print(month);
  lcd.print("/");
  lcd.print(day);
  lcd.print(" ");
  lcd.print(w[week-1]);
}
