#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum MorseState {
  IDLE, PULSE_HIGH, PULSE_LOW, PAUSE
};

struct MorseElement {
  int duration;  // in ms
  bool isHigh;
};

MorseElement redMorse[] = {
  // R: .-.
  {200, true}, {400, false},  
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {1000, false},
  // E: .
  {200, true}, {400, false},
  {1000, false},
  // D: -..
  {200, true}, {1000, false}, 
  {200, true}, {400, false},
  {200, true}, {400, false},
  {2000, false}
};

MorseElement greenMorse[] = {
  // G: --.
  {200, true}, {1000, false}, 
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {2000, false},

  // R: .-.  
  {200, true}, {400, false},
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {2000, false},

  // E: .
  {200, true}, {400, false},
  {2000, false},
  // E: .
  {200, true}, {400, false},
  {2000, false},

  // N: -.
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {2000, false},
};

MorseElement blueMorse[] = {
  // B: -...
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {200, true}, {400, false},
  {200, true}, {400, false},
  {2000, false},

  // L: .-..
  {200, true}, {400, false},
  {200, true}, {1000, false},
  {200, true}, {400, false},
  {200, true}, {400, false},
  {2000, false},

  // U: ..-
  {200, true}, {400, false}, 
  {200, true}, {400, false},
  {200, true}, {1000, false},
  {2000, false},

  // E: .
  {200, true}, {400, false},
  {2000, false},   
};

const int redMorseLength = sizeof(redMorse) / sizeof(MorseElement);
const int greenMorseLength = sizeof(greenMorse) / sizeof(MorseElement);
const int blueMorseLength = sizeof(blueMorse) / sizeof(MorseElement);


int morseIndex = 0;
unsigned long lastMorseTime = 0;
bool morseActive = true;
bool gameOn = true;
int morsePin = A5;

const int win_pin = A0;
const int lose_pin = 7;
const int reset_pin = 3;

const int red_wire = 13;
const int green_wire = 11;
const int blue_wire = 9;

int correct_wire;

void handleMorse() {
  static unsigned long previousMillis = 0;
  static int index = 0;
  MorseElement* sequence;
  int length;
  if (correct_wire == red_wire){ 
    sequence = redMorse;
    length = redMorseLength;
  }
  else if (correct_wire == green_wire) {
    sequence = greenMorse;
    length = greenMorseLength;
  } 
  else if (correct_wire == blue_wire) {
    sequence = blueMorse;
    length = blueMorseLength;
  }
  if (!morseActive) return;

  if (millis() - previousMillis >= sequence[index].duration) {
    previousMillis = millis();
    digitalWrite(morsePin, sequence[index].isHigh ? HIGH : LOW);
    index++;
    if (index >= length) {
      index = 0;
      digitalWrite(morsePin, LOW);
    }
  }
}


void pickRandomWire(){
  correct_wire = 9 + random(0, 3) * 2; 
}


void setup() {
  randomSeed(analogRead(2)); 
  Serial.begin(9600);
  pinMode(red_wire, INPUT);
  pinMode(green_wire, INPUT);
  pinMode(blue_wire, INPUT);
  pinMode(reset_pin, INPUT);
  pinMode(lose_pin, OUTPUT);
  pinMode(win_pin, OUTPUT);
  pinMode(morsePin, OUTPUT);
  pickRandomWire();
}

void loop() {
  if(gameOn == true){  
    handleMorse();
    int sensor_red = digitalRead(red_wire);
    int sensor_green = digitalRead(green_wire);
    int sensor_blue = digitalRead(blue_wire);

    if (sensor_red == LOW){
      if(correct_wire == red_wire) digitalWrite(win_pin, HIGH);
      else digitalWrite(lose_pin, HIGH);
      gameOn = false;
    }
    else if (sensor_green == LOW){
      if(correct_wire == green_wire) digitalWrite(win_pin, HIGH);
      else digitalWrite(lose_pin, HIGH);
      gameOn = false;
    }
    else if (sensor_blue == LOW){
      if(correct_wire == blue_wire) digitalWrite(win_pin, HIGH);
      else digitalWrite(lose_pin, HIGH);
      gameOn = false;
    }
    else {
      digitalWrite(win_pin, LOW);
      digitalWrite(lose_pin, LOW);
    }
  }
  else{
    if(digitalRead(reset_pin) == HIGH) {
      gameOn = true;
      pickRandomWire();
      morseActive = true;
      morseIndex = 0;
      lastMorseTime = millis();
      delay(2000);
    } 
  }
  delay(100);
}


