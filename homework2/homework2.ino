const int buttonPin1 = 10;
const int buttonPin2 = 9;
const int buttonPin3 = 8;

const int buttons[] = {buttonPin1, buttonPin2, buttonPin3};

const int level1Pin = 7;
const int level2Pin = 6;
const int level3Pin = 5;

const int levels[] = {level1Pin, level2Pin, level3Pin};

const int blinkerPin = 4;
const int blinkDelay = 200;
int lastBlink = 0;
byte blinkerState = HIGH;

byte buttonState[] = {HIGH, HIGH, HIGH};
byte ledState[] = {LOW, LOW, LOW};
byte buttonReading[] = {HIGH, HIGH, HIGH};
byte lastButtonReading[] = {HIGH, HIGH, HIGH};

const int debounceDelay = 60;
unsigned long int lastDebounce[] = {0, 0, 0};

byte i = 0;
byte currentLevel = 0;
byte nextLevel = 0;
bool isMoving = false;

const int levelMovingDelay = 1000;
int lastMovingTime = 0;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(levels[i], OUTPUT);
  }
  pinMode(blinkerPin, OUTPUT);
  ledState[currentLevel] = HIGH;
  digitalWrite(levels[currentLevel], HIGH);
}

// in superLOOP daca nu ne miscam verificam butoanele in cazul in care cineva cheama liftul
// daca se intample sa-l apese liftul se pune in miscare, iar cat timp este in miscare nu mai verificam butoanele
// ci mutam liftul de la nivelul curent la nivelul butonului apasat
void loop() {
  blink();
  if (!isMoving) {
    for (int i = 0; i < 3; i++) {
      checkButton(i);
    }
  } else {
    moveElevator(currentLevel, nextLevel);
  }
}

void blink() {
  if (!isMoving) {
    blinkerState = HIGH;
  } else if (millis() - lastBlink >= blinkDelay) {
    lastBlink = millis();
    blinkerState = !blinkerState;
  }
  digitalWrite(blinkerPin, blinkerState);
}

// verfica pentru un buton dat daca state ul s-a schimbat. daca da pornim un timer si daca
// dupa o perioada de 50 ms state ul citit e diferit de state ul butonului inseamna ca butonul si-a schimat starea
// si atunci verificam daca s-a apasat pe buton si daca nu s a apasat la nivelul curent, atunci tinem minte nivelul 
// destinatie si punem liftul in miscare
void checkButton(int idx) {
  buttonReading[idx] = digitalRead(buttons[idx]);

  if (buttonReading[idx] != lastButtonReading[idx]) {
    lastDebounce[idx] = millis();
  }

  if (millis() - lastDebounce[idx] >= debounceDelay) {
    if (buttonReading[idx] != buttonState[idx]) {
      buttonState[idx] = buttonReading[idx];
      if (buttonState[idx] == LOW && currentLevel != idx) {
        nextLevel = idx;
        isMoving = true;
        lastMovingTime = millis();
      }
    }
  }
  digitalWrite(levels[idx], ledState[idx]);
  lastButtonReading[idx] = buttonReading[idx];
}

// o data la un interval de o secunde mutam liftul cu un nivel, daca nivelul destinatie
// este mai sus, ne ducem cu un nivel in sus, daca e in jos, ne ducem un nivel in jos
// cand nivelul curent == nivelul destinatie, am ajuns :)
void moveElevator(byte &currentLevel, byte nextLevel) {
  if (millis() - lastMovingTime >= levelMovingDelay) {
    lastMovingTime = millis();

    if (nextLevel - currentLevel > 0) { // urcam
      ledState[currentLevel] = LOW;
      digitalWrite(levels[currentLevel], LOW);
      currentLevel++;
      ledState[currentLevel] = HIGH;
      digitalWrite(levels[currentLevel], HIGH);
    
    } else if (nextLevel - currentLevel < 0) { // coboram
      ledState[currentLevel] = LOW;
      digitalWrite(levels[currentLevel], LOW);
      currentLevel--;
      ledState[currentLevel] = HIGH;
      digitalWrite(levels[currentLevel], HIGH);
    
    } else { // am ajuns
      isMoving = false;
      return;
    }
  }
}