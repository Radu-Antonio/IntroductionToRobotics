const int elevatorBlinker = 4;
const int level1Pin = 10;
const int level2Pin = 11;
const int level3Pin = 12;
const int level1Button = 5;
const int level2Button = 6;
const int level3Button = 7;
int currentLevel = 1;
bool isMoving = false;

void setup() {
  int buttons[] = { level1Button, level2Button, level3Button };
  pinMode(elevatorBlinker, OUTPUT);
  pinMode(level1Pin, OUTPUT);
  pinMode(level2Pin, OUTPUT);
  pinMode(level3Pin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(buttons[i], INPUT);
  }
  digitalWrite(level1Pin, HIGH);
  digitalWrite(elevatorBlinker, HIGH);
}

void loop() {
  if (!isMoving) {
    for (button in buttons)
      if (buttonPressed(button))
        if (button != currentLevel) {
          // move to the wanted level
          isMoving = true;
        }
  } else {
    blink(elevatorBlinker);
    
  }
}
