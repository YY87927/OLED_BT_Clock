#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int hour=0, minute=0, second=0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(8, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  printTime();
  delay(1000);
  addTime();
}

void printTime(){
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
  }
}
