#include <stdbool.h>

String input = "";
int inputNumber;
bool gameState[9];


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
}

void loop() {
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
    delay(1000);
    for (int j = 0; j < 9; j++) {
      gameState[j] = !gameState[j];
    }
  }
}
