#define RED_LED 2
#define GREEN_LED 3
#define BLUE_LED 4
#define YELLOW_LED 5

#define RED_BUTTON 8
#define GREEN_BUTTON 9
#define BLUE_BUTTON 10
#define YELLOW_BUTTON 11

// Define a tone for each color (optional, requires a buzzer on pin 12)
// #define TONE_RED 262
// #define TONE_GREEN 294
// #define TONE_BLUE 330
// #define TONE_YELLOW 349
// #define BUZZER_PIN 12

int ledPins[] = {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED};
int buttonPins[] = {RED_BUTTON, GREEN_BUTTON, BLUE_BUTTON, YELLOW_BUTTON};
//int tones[] = {TONE_RED, TONE_GREEN, TONE_BLUE, TONE_YELLOW};

#define MAX_LEVEL 5
int sequence[MAX_LEVEL];
int currentLevel = 0;
int stepToPlay = 0;

enum GameState {
  START_GAME,
  COMPUTER_TURN,
  PLAYER_TURN,
  GAME_OVER
};

GameState gameState = START_GAME;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(A0));
  
  //pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  switch (gameState) {
    case START_GAME:
      if (isAnyButtonPressed()) {
        delay(500);
        newGame();
      }
      break;

    case COMPUTER_TURN:
      showSequence();
      gameState = PLAYER_TURN;
      stepToPlay = 0;
      break;

    case PLAYER_TURN:
      {
        int buttonPressed = readPlayerInput();
        if (buttonPressed != -1) {
          if (buttonPressed == sequence[stepToPlay]) {
            lightUp(buttonPressed, 150);
            stepToPlay++;
            if (stepToPlay >= currentLevel) {
              delay(500);
              nextLevel();
            }
          } else {
            gameOver();
          }
        }
      }
      break;

    case GAME_OVER:
      if (isAnyButtonPressed()) {
        delay(500);
        newGame();
      }
      break;
  }
}

void newGame() {
  currentLevel = 0;
  nextLevel();
}

void nextLevel() {
  currentLevel++;
  if (currentLevel > MAX_LEVEL) {
      winGameAnimation();
      gameState = GAME_OVER;
      return;
  }

  sequence[currentLevel - 1] = random(0, 4);

  gameState = COMPUTER_TURN;
}

void showSequence() {
  delay(500);
  for (int i = 0; i < currentLevel; i++) {
    int colorIndex = sequence[i];
    lightUp(colorIndex, 500);
    delay(150);
  }
}

int readPlayerInput() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

bool isAnyButtonPressed() {
  return readPlayerInput() != -1;
}

void gameOver() {
  for (int i = 0; i < 3; i++) {
    lightUpAll(200);
    delay(200);
  }
  //tone(BUZZER_PIN, 150, 1000);
  gameState = GAME_OVER;
}

void lightUp(int colorIndex, int duration) {
  digitalWrite(ledPins[colorIndex], HIGH);
  tone(BUZZER_PIN, tones[colorIndex], duration);
  delay(duration);
  digitalWrite(ledPins[colorIndex], LOW);
}

void lightUpAll(int duration) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(duration);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void winGameAnimation() {
    for (int i = 0; i < 5; i++) {
        for(int j=0; j<4; j++){
            lightUp(j, 50);
        }
    }
}