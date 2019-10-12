//weekday still have some small problem
#define NOTE_    0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// 然後準備歌曲的資料，底下先用個短短的範例。
int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4,
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_G3,
  NOTE_A3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_C4,
};

int noteDurations[] = {
  6, 16, 8, 8, 8, 8, 4,
  6, 16, 8, 8, 8, 8, 4,
  6, 16, 8, 8, 8, 8, 4,
  6, 16, 8, 8, 8, 8, 4,
};

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
    //printTime();

    if(hour == alarm_hour && minute == alarm_minute){
      Serial.println("Alarm!!!!!!");
      play(melody, noteDurations, sizeof(melody) / sizeof(int));
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
    //printTime();
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
    //printTime();
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
   printTime();
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

void play(int *melody, int *noteDurations, int num){
  for(int note = 0; note < num; note++){
    int noteDuration = 3000 / noteDurations[note];
    tone(10, melody[note], noteDuration);

    delay(noteDuration * 1.30);
  }
}
