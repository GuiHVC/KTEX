#define RED_LED 2
#define GREEN_LED 3
#define YELLOW_LED 4

#define RED_BUTTON 5
#define GREEN_BUTTON 6
#define YELLOW_BUTTON 7

#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740

#define NOTE_B3  247
#define NOTE_AS3 233 // A#3 / Bb3
#define NOTE_A3  220
#define NOTE_GS3 208 // G#3 / Ab3

// Define a tone for each color (optional, requires a buzzer on pin 12)
#define TONE_RED 262
#define TONE_GREEN 294
#define TONE_YELLOW 349
#define BUZZER_PIN 12

int ledPins[] = {RED_LED, GREEN_LED, YELLOW_LED};
int buttonPins[] = {RED_BUTTON, GREEN_BUTTON, YELLOW_BUTTON};
int tones[] = {TONE_RED, TONE_GREEN, TONE_YELLOW};

#define MAX_LEVEL 5
int sequence[MAX_LEVEL];

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  pinMode(RED_BUTTON, INPUT);
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);

  randomSeed(analogRead(A0));
  Serial.begin(9600);
  
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop(){
  setPins();

  generateGame();

  waitStart();
}

void setPins(){
  for(int i = 2; i < 8; i++){
    digitalWrite(i, HIGH);
  }
  Serial.println("Pins set");
}

void playGame(){
  lightUpAll(1000);

  int roundCount = 0;
  int playerTurn = 1;
  bool buttonPress = false;
  int userInput = 0;
  bool lost = false;
  int target;

  delay(1000);

  for(int currentLevel = 1; (currentLevel - 1) <= MAX_LEVEL; currentLevel++){
    roundCount++;
    playerTurn = 1;
    buttonPress = false;
    userInput = 0;

    for(int j = 1; j != currentLevel; j++){
      lightUp(sequence[j - 1], 1000);
      delay(500);
    }

    while(playerTurn < currentLevel){
      target = sequence[playerTurn - 1];
      Serial.print("Expecting ");
      Serial.println(target);

      while(buttonPress == false){
        for (int i = 0; i < 3; i++) {
          if (digitalRead(buttonPins[i]) == LOW) {
            Serial.print("Apertei ");
            Serial.println(i);
            buttonPress = true;
            userInput = i;
          }
        }
        if(buttonPress == true){
          lightUp(userInput, 500);
          delay(500);
          if(target == userInput){
            playerTurn++;
          }
          else{
            lost = true;
            gameOver(false);
            return;
          }
        }
      }
      buttonPress = false;
    }
  }
  if(lost == false){
    Serial.println("Victory");
    gameOver(true);
  }
}

void gameOver(bool win){
  if(win){
    Serial.println("you win");
    // lightUpAll(1000);
    // delay(500);
    // lightUpAll(1000);
    int melody[] = {
      NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, 
      NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
      NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
      NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
      NOTE_F5, NOTE_FS5
    };

    int noteDurations[] = {
      150, 150, 150, 600,
      150, 150, 300, 300,
      150, 150, 300, 300,
      150, 150, 150, 150,
      300, 600
    };
    for(int i = 0; i < 18; i++){
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      lightUp(i%3, noteDurations[i]);
    }
  }
  else{
    Serial.println("you lost");

    int melody[] = { NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS3 };
    int noteDurations[] = { 250, 250, 250, 500 };

    for(int i = 0; i < 4; i++){
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      lightUpAll(noteDurations[i]);
    }
  }
}

void generateGame(){
  for(int i = 0; i < MAX_LEVEL; i++){
    sequence[i] = random (0, 3);
  }
  Serial.println("Sequence Generated");
  for(int i = 0; i < MAX_LEVEL; i++){
    Serial.print(sequence[i]);
    Serial.println(" ");
  }
}

void waitStart(){
  while(1 == 1){
    for (int i = 0; i < 3; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        Serial.println("Starting");
        Serial.println(i);
        generateGame();
        playGame();
      }
    }
  }
}

void lightUp(int colorIndex, int duration) {
  digitalWrite(ledPins[colorIndex], LOW);
  tone(BUZZER_PIN, tones[colorIndex], duration);
  delay(duration);
  digitalWrite(ledPins[colorIndex], HIGH);
}

void lightUpAll(int duration) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  delay(duration);
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}
