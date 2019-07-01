#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN 3

LiquidCrystal_I2C	lcd(0x27,2,1,0,4,5,6,7);

const int buttonPinIncrease = 11;    // the number of the pushbutton pin
const int buttonPinDecrease = 12;
const int firstCoilPin = 9;
const int secondCoilPin = 10;


int buttonState;             // the current reading from the input pin
int buttonState2;
int lastButtonState = LOW;   // the previous reading from the input pin
int lastButtonState2 = LOW;
int rpmValue = 1500;
int value = 100;
int phase = 1;


long revTime=40000;
long lastRevTime = 0; 
long currentTime;
long quaterRev = revTime/4;
long tenDegRev = revTime/36;

void setup() {
  pinMode(buttonPinIncrease, INPUT);
  pinMode(buttonPinDecrease, INPUT);

  pinMode(firstCoilPin, OUTPUT);
  pinMode(secondCoilPin, OUTPUT);

  revTime = 1000000*60;
  revTime = revTime/(rpmValue);
  quaterRev = revTime/4;
  tenDegRev = revTime/36;
  
 lcd.begin (16,2);
 lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(HIGH);

lcd.home ();
lcd.print(rpmValue );
lcd.print(" rpm");
}

void loop() {

  currentTime = micros();
 
 switch (phase)
 {
  case 1:
  if ( currentTime-(lastRevTime+tenDegRev)>= 0){
      digitalWrite(firstCoilPin, LOW);
      phase=2;
  }
  break;
  
  case 2:
  if ( currentTime-(lastRevTime+quaterRev)>= 0){
      digitalWrite(secondCoilPin, HIGH);
      phase=3;
  }
  break;
  
  case 3: 
  if (currentTime-(lastRevTime+quaterRev+ tenDegRev)>= 0){
      digitalWrite(secondCoilPin, LOW);
      phase=4;
  }
  break;
  
    case 4:
 
    //button(buttonPinIncrease, &lastButtonState, &buttonState,  value, &rpmValue, &revTime, &quaterRev, &tenDegRev, buttonPinDecrease, &lastButtonState2, &buttonState2,  -value);     
    
    buttonState = digitalRead(buttonPinIncrease);
   buttonState2 = digitalRead(buttonPinDecrease);
  
  if (buttonState == HIGH && lastButtonState ==LOW) {
        rpmValue = rpmValue + value;
        revTime = 1000000*60;
        revTime = revTime/(rpmValue);
        quaterRev = revTime/4;
        tenDegRev = revTime/24;
        lcd.clear();
        //lcd.setCursor(0, 0);
        lcd.print(rpmValue);
        lcd.print(" rpm");
      }
      
      if (buttonState2 == HIGH && lastButtonState2 ==LOW) {
        rpmValue = rpmValue - value;
        revTime = 1000000*60;
        revTime = revTime/(rpmValue);
        quaterRev = revTime/4;
        tenDegRev = revTime/24;
        lcd.clear();
        //lcd.setCursor(0, 0);
        lcd.print(rpmValue);
        lcd.print(" rpm");
      }

  lastButtonState = buttonState;
  lastButtonState2 = buttonState2;
    
    
    if (rpmValue <100) rpmValue=100;
    if (rpmValue >15000) rpmValue=15000;
    
    
    
    phase=5;
 break;
  
  case 5:
  if ( currentTime-(lastRevTime+revTime)>= 0){
    
      //Serial.print("phase: 4");
      //Serial.print("; New Rev ");
      phase=1;
      lastRevTime = micros();
      digitalWrite(firstCoilPin, HIGH);
  }
   break;
 }
}

/*
void button (int buttonPin, int *lastButtonState, int *buttonState,  int value, int *rpmValue,  long *revTime, long *quaterRev, long *tenDegRev, int buttonPin2, int *lastButtonState2, int *buttonState2, int value2){
  
   *buttonState = digitalRead(buttonPin);
   *buttonState2 = digitalRead(buttonPin2);
  
  if (*buttonState == HIGH && *lastButtonState ==LOW) {
        *rpmValue = *rpmValue + value;
        *revTime = 1000000*60;
        *revTime = *revTime/(*rpmValue);
        *quaterRev = *revTime/4;
        *tenDegRev = *revTime/24;
        lcd.clear();
        //lcd.setCursor(0, 0);
        lcd.print(*rpmValue);
        lcd.print(" rpm");
      }
      
      if (*buttonState2 == HIGH && *lastButtonState2 ==LOW) {
        *rpmValue = *rpmValue + value2;
        *revTime = 1000000*60;
        *revTime = *revTime/(*rpmValue);
        *quaterRev = *revTime/4;
        *tenDegRev = *revTime/24;
        lcd.clear();
        //lcd.setCursor(0, 0);
        lcd.print(*rpmValue);
        lcd.print(" rpm");
      }

  *lastButtonState = *buttonState;
  *lastButtonState2 = *buttonState2;
  
}
*/
