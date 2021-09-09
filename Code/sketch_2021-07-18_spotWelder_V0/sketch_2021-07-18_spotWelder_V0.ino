// initilize lcd
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

const int rs =9, en = 6, d4 = 7, d5 = 8, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// declare btn pin
#define PIN_IN1 2
#define PIN_IN2 3
#define BTN_PIN 10
#define FOOT_PIN 11
#define OUT_PIN 12

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

  Serial.begin(9600);
  //prepare the lcd
  lcd.begin(16, 2);
}

void loop() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Spot Welder V1!");
  do {
    btnState = digitalRead(BTN_PIN);
  } while (btnState == 1);
  
    btnState = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ms:");
    lcd.setCursor(4,0);
    lcd.print(encoder->getPosition()*2);
    
  while (btnState == 1){
    // sets the ms on the lcd 
    encoder->tick(); // just call tick() to check the state.
    newPos = encoder->getPosition();
    if (pos != newPos) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ms:");
    lcd.setCursor(4,0);
    lcd.print(encoder->getPosition()*2);
    pos = newPos;
  }
  
   if (digitalRead(FOOT_PIN) == 0){
      digitalWrite(OUT_PIN, HIGH);
      delay(encoder->getPosition()*2);
      digitalWrite(OUT_PIN, LOW);
   }
  }
 }
