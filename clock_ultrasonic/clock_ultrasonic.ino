#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int hour=0, minute=0, second=0, day=1, month=1, week=1;
const int trig = 10;
const int echo = 11;
const int inter_time = 1000;
int time = 0, mode=0;
const int button = 2, up = 3, down = 4;
boolean change = false;
int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char w[7][5] = {{"Mon  "}, {"Tue  "}, {"Wed  "}, 
                {"Thu  "}, {"Fri  "}, {"Sat  "}, {"Sun  "}};

void setup() {
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
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);

  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), changeMode, RISING);
}

void loop() {
  if(change){
    delay(500);
    change=false;
    mode+=1;
    mode%=2;
    lcd.clear();
  }

  
  //Serial.print(mode);
  if(mode==0){
    printTime();
    if(digitalRead(up)==1 && digitalRead(down)==1){
      Serial.print("$$$");
      editTime();
    }
  }
  else if(mode==1){
    float duration, distance;
    digitalWrite(trig, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trig, LOW);
    duration = pulseIn (echo, HIGH);
    distance = (duration/2)/29;
    Serial.print("Data:");
    Serial.print (time/1000);
    Serial.print(", d = ");
    Serial.print(distance);
    Serial.println(" cm");
    time = time + inter_time;
    delay(inter_time);
    printDistance(distance);
  }

  
}

ISR(TIMER1_COMPA_vect){  //change the 0 to 1 for timer1 and 2 for timer2
   //interrupt commands here
   addTime();
}
void editTime(){
  //disable timer 1  
  //TIMSK1 |= (0 << OCIE1A);
  //edit first or second digit //day&month might have two digit
  delay(500);
  lcd.cursor();
  int editCursor = 0, timeChangeFlag = 0;
  while(digitalRead(up)==0 || digitalRead(down)==0){
//    printTime();
    if(digitalRead(button) == 1){
      delay(800);
      editCursor++;
      editCursor%=10;
    }
    switch(editCursor){
      case 0:
        Serial.print("0");
        timeChangeFlag=1;
        lcd.setCursor(0, 0);
        if(digitalRead(up)==1){
          hour+=10;
        }
        else if(digitalRead(down==1)){
          hour-=10;
        }
        break;
      case 1:
        Serial.print("1");
        timeChangeFlag=1;
        lcd.setCursor(1, 0);
        if(digitalRead(up)==1){
          hour++;
        }
        else if(digitalRead(down==1)){
          hour--;
        }
        break;
      case 2:
        Serial.print("2");
        timeChangeFlag=1;
        lcd.setCursor(3, 0);
        if(digitalRead(up)==1){
          minute+=10;
        }
        else if(digitalRead(down)==1){
          minute-=10;
        }
        break;
      case 3:
        Serial.print("3");
        timeChangeFlag=1;
        lcd.setCursor(4, 0);
        if(digitalRead(up)==1){
          minute++;
        }
        else if(digitalRead(down)==1){
          minute--;
        }
        break;
      case 4:
        Serial.print("4");
        timeChangeFlag=1;
        lcd.setCursor(6, 0);
        if(digitalRead(up)==1){
          second+=10;
        }
        else if(digitalRead(down)==1){
          second-=10;
        }
        break;
      case 5:
        Serial.print("5");
        timeChangeFlag=1;
        lcd.setCursor(7, 0);
        if(digitalRead(up)==1){
          second++;
        }
        else if(digitalRead(down)==1){
          second--;
        }
        break;
      case 6:
        Serial.print("6");
        timeChangeFlag=1;
        lcd.setCursor(0, 1);
        if(digitalRead(up)==1){
          month++;
        }
        else if(digitalRead(down)==1){
          month--;
        }
        break;
      case 7:
        Serial.print("7");
        timeChangeFlag=1;
        lcd.setCursor(2, 1);
        if(digitalRead(up)==1){
          day++;
        }
        else if(digitalRead(down)==1){
          day--;
        }
        break;
      case 8:
        Serial.print("8");
        timeChangeFlag=1;
        if(digitalRead(up)==1){
          week++;
        }
        else if(digitalRead(down)==1){
          week--;
        }
        break;
    }
    if(timeChangeFlag==1){
//      Serial.print("123");
      printTime();
      timeChangeFlag=0;
    }
  }
}


void changeMode(){
  //timer:mode0, ultrasonic:mode1, bluetooth:mode2
  change=true;
  delay(500);
}

void printDistance(float distance){
  lcd.setCursor(0, 0);
  lcd.print("dis=    ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print("cm    ");
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
