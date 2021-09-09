// Spot Welder V1
// By nobski

#include <LiquidCrystal.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

const int rs =7, en = 9, d4 = 8, d5 = 11, d6 = 12, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// declare btn pin
#define PIN_IN1 2
#define PIN_IN2 3
#define BTN_PIN 4
#define FOOT_PIN 5
#define OUT_PIN 6

// declare
int btnState = 0;
static int pos = 0;
int newPos = 0;
int footState = 0;
RotaryEncoder *encoder = nullptr;

void checkPosition() {
  encoder->tick(); // just call tick() to check the state.
}

void setup() {
  // set the interrupt
  pinMode(BTN_PIN, INPUT);
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, RISING);

  // initilize pins 
  pinMode(PIN_IN1, INPUT_PULLUP);
  pinMode(PIN_IN2, INPUT_PULLUP);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(FOOT_PIN, INPUT_PULLUP);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  //Serial.begin(9600);
  //prepare the lcd
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Spot Welder V1!");






  
}

void loop() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Spot Welder V1!");
  lcd.setCursor(0,1);
  lcd.print("Safe");
  delay(500);
  
  do {
    btnState = digitalRead(BTN_PIN);
  } while (btnState == 1);

    btnState = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ms:");
    lcd.setCursor(4,0);
    lcd.print(encoder->getPosition()*2);
    lcd.setCursor(0,1);
    lcd.print("Ready");
    delay(500);
    
  while (btnState == 1){
    
    // sets the ms on the lcd 
    encoder->tick(); // just call tick() to check the state.
    newPos = encoder->getPosition();
    if (encoder->getPosition() < 1){
      encoder->setPosition(1);
    }
    if (pos != newPos) {
    lcd.setCursor(0,0);
    lcd.print("ms:");
    lcd.setCursor(4,0);
    lcd.print(encoder->getPosition()*2);
    pos = newPos;
  }

  if (digitalRead(BTN_PIN) == 0){ // controls the safe mode
    btnState = 0;
  } else {
    
    if (digitalRead(FOOT_PIN) == 0 && footState == 0){ // Welds!
      footState = 1;
      digitalWrite(OUT_PIN, HIGH);
      delay(encoder->getPosition()*2);
      digitalWrite(OUT_PIN, LOW);
      lcd.setCursor(0,1);
      lcd.print("Welded!");
      
    } else if (digitalRead(FOOT_PIN) == 1 && footState == 1) { // safe time to take foot off pedal
      lcd.setCursor(0,1);
      lcd.print("Welded");
      footState = 0;
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("Ready  ");
    }
   }
  }
 }
