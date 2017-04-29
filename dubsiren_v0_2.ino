/**
 * MASEGO SOUNDWORKS - Dub Siren v0.2
 * Erik-Jan van de Wal, MIT License 2017.
 **/

#include <LiquidCrystal.h>;
LiquidCrystal lcd(12, 11, 5, 4, 3 ,2);

// Tone
const  float startHz = 100.0;
const  float endHz = 2000.0;
float freq = 100.0;
float timeSpeed = 50.0;

//Buttons
const int button0 = A0;
const int button1 = A1;
const int button2 = A2;

// Wailing siren
float wailingSirenFreq = freq;
float wailingSirenTimeSpeed = timeSpeed;
const int wailingStartHz = 300;
const int wailingEndHz = 3200;
int wailingStart = wailingStartHz;
int wailingEnd = wailingEndHz;
boolean wailingUp = true;

// Alarm
float alarmFreq = freq;
float alarmTimeSpeed = timeSpeed;

// Laser
float laserFreq = freq;
float laserTimeSpeed = timeSpeed;

// Speaker
const int speakerPin = 7;

// Led
const int ledPin = 9;

// Potmeters
const  float maxPotential = 1023.0;
const int potPinFreq = A5;
const int potPinSpeed = A4;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("ALL GLORY TO JAH");
  delay(1500);

  //  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(button0, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

void loop() {
  boolean buttonState0 = digitalRead(button0);
  boolean buttonState1 = digitalRead(button1);
  boolean buttonState2 = digitalRead(button2);
 
  lcd.setCursor(0, 1);
  getFrequency();
  getTimeSpeed();
  
  if ( buttonState1 == 0 ) {
    alarm(buttonState1);
  }
  if ( buttonState2 == 0 ) {
    laser(buttonState2);
  }
  if ( buttonState0 == 0 ) {
    wailingSiren(buttonState0);
  }

  lcd.setCursor(0, 0);
  lcd.print("MASEGO SOUNDWORKS");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void getFrequency() {
  // calculate frequency
  float analogIn = analogRead(potPinFreq);
  freq = ( (analogIn - 0) / (maxPotential - 0) ) * (endHz - startHz) + startHz;
  // write to lcd
  lcd.setCursor(6, 1);
  lcd.print("f:");
  lcd.setCursor(8, 1);
  lcd.print(round(freq));
}

void getTimeSpeed() {
  // calculate frequency
  float analogIn = analogRead(potPinSpeed);
  timeSpeed = ( (analogIn - 0) / (maxPotential - 0) ) * (25 - 500) + 500;
  // write to lcd
  lcd.setCursor(0, 1);
  lcd.print("s:");
  lcd.setCursor(2, 1);
  lcd.print(round(timeSpeed));
}

void alarm(int buttonState1) {
  if (buttonState1 == 0) {
    analogWrite(ledPin, 255);
    tone(speakerPin, freq, timeSpeed);
    delay(timeSpeed/2);
    tone(speakerPin, round(freq*1.5), timeSpeed);
    delay(timeSpeed/2);
  }
  analogWrite(ledPin, 0);
}

void laser(int buttonState2) {
  if (buttonState2 == 0) { 
    for (int i = 5; i>1; i--) {
      for (int j = 3; j > 0; j--) {
        analogWrite(ledPin, i*25);
        tone(speakerPin, round((j*i)*(freq/4)), timeSpeed);
        delay(timeSpeed/10);
      }
      tone(speakerPin, round((i)*(freq/4)), timeSpeed);
    }
  }
  analogWrite(ledPin, 0);
}

void wailingSiren(int buttonState0) { 
  for (int i = wailingStart; i < (round(freq*2)); i++) {
    // break out of the for when the button is let go
    if ( digitalRead(button0) == 0 && i > 500 ) {
      delay(500);
      break;
    }
    if ( (i%3) == 1 ) {
      getTimeSpeed();
    }
    float analogIn = analogRead(potPinFreq);
    float wfreq = ( (analogIn - 0) / (maxPotential - 0) ) * (endHz - startHz) + startHz;

    // Turn on the led
    analogWrite(ledPin, 255); 
    // Generate tone on frequency == wailingStart
    tone(speakerPin, i, 10);
    delay(timeSpeed/100);
    
    if ( i == (round((freq*2)-1)) ) {
      while(digitalRead(button0) == 1) {
        tone(speakerPin, i, 10)
      }
    }
  }
  analogWrite(ledPin, 0);
}

void wailingSirenUp() {
  for (wailingStart < wailingEnd; wailingStart++;) {
    // break out of the for when the button is let go
    if ( digitalRead(button0) == 1 ) break;

    // Check if we need to reverse the sound
    if ( wailingStart > wailingEnd ) {
      wailingUp = false;
      wailingStart = wailingEndHz;
      wailingEnd = wailingStartHz;
      delay(10);
      break;
    }

    // Turn on the led
    analogWrite(ledPin, 255); 
    // Generate tone on frequency == wailingStart
    tone(speakerPin, wailingStart, 5);
    delay(1);
  }
  analogWrite(ledPin, 0);
}
void wailingSirenDown() {
  for (wailingStart > wailingEnd; wailingStart--;) {
    // break out of the for when the button is let go
    if ( digitalRead(button0) == 1 ) break;

    // Check if we need to reverse the sound
    if ( wailingStart < wailingEnd ) {
      wailingUp = true;
      wailingStart = wailingStartHz;
      wailingEnd = wailingEndHz;
      delay(10);
      break;
    }
    // Turn on the led
    analogWrite(ledPin, 255); 
    // Generate tone on frequency == wailingStart
    tone(speakerPin, wailingStart, 5);
    delay(1);
  }
  analogWrite(ledPin, 0);
}
