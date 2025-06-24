#include <stdbool.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 16
#define lin 2
#define ende 0x27

LiquidCrystal_I2C lcd(ende,col,lin);

String input = "";
int inputNumber;
bool gameState[9];

int timer = 60;
unsigned long previousMillis = 0; 
const long interval = 1000;

void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(A0));
  gameInit();

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  
  decreaseTimer();

  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');
    inputNumber = input.toInt();
    Serial.println(inputNumber);

    if (inputNumber >= 1 && inputNumber <= 9) {
      action(inputNumber);
      checkState();
    }
  }

  light();
}


void gameInit(){
  for (int i = 0; i < 9; i++) {
    gameState[i] = false;
  }

  for (int i = 0; i < 5; i++) { // 5 movimentos aleatorios para garantir um estado possivel de resolver
    int move = random(0, 9);
    action(move + 1);
  }
}

void action(int inputNumber){
  inputNumber--; // 0 a 8

  gameState[inputNumber] = !gameState[inputNumber];

  if(inputNumber - 3 >= 0) 
    gameState[inputNumber - 3] = !gameState[inputNumber - 3];

  if(inputNumber + 3 <= 8) 
    gameState[inputNumber + 3] = !gameState[inputNumber + 3];

  if(inputNumber % 3 != 0) 
    gameState[inputNumber - 1] = !gameState[inputNumber - 1];

  if(inputNumber % 3 != 2) 
    gameState[inputNumber + 1] = !gameState[inputNumber + 1];
}


void light(){
  for(int i = 0; i < 9; i++){
    if(gameState[i]) digitalWrite(i + 2, HIGH);
    else digitalWrite(i + 2, LOW);
  }
}

void checkState(){
  for (int i = 0; i < 9; i++) {
    if (gameState[i] == true) return;
  }
  while(true) {
    light();
    decreaseTimer();
  }
}

void decreaseTimer(){
  unsigned long currentMillis = millis(); // Pega o tempo atual

  // Decrementa o timer a cada 1 segundo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (timer > 0) {
      timer--;
    }

    // Exibe o tempo restante no LCD
    lcd.clear();
    lcd.setCursor(0, 1); 
    lcd.print("Timer: ");
    lcd.print(timer);

    if (timer == 0) {
      lcd.setCursor(0, 1);
      lcd.print("Game Over!");
      // liga todas as luzes
      for (int i = 0; i < 9; i++) {
        gameState[i] = true;
      }
      light();
      while (true) {

        // aqui eu mando o sinal avisando que perdeu
        
      }
    }
  }
}
