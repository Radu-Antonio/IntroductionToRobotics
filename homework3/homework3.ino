const int pinSW = 2;
const int pinY = A0; 
const int pinX = A1; 

const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int segSize = 8;

byte swState = HIGH;
byte lastSwState = HIGH;
const int resetTime = 2000;
unsigned long lastTimePressed = 0;

int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;

byte segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
  // 0    1    2     3      4     5    6      7
};

byte ledState[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define OOB 255 // out of bounds

int currentLed = 7; // dp
// un fel de hashmap care ne zice vecinul unui led intr-o anumita directie
byte neighbour[segSize][4] = {
  {OOB,   6,   5, 1  },       // a
  {  0,   6,   5, OOB},       // b
  {  6,   3,   4, 7  },       // c
  {  6, OOB,   4, 2  },       // d
  {  6,   3, OOB, 2  },       // e
  {  0,   6, OOB, 1  },       // f
  {  0,   3, OOB, OOB},       // g
  {OOB, OOB,   2, OOB},       // dp
};

const int blinkInterval = 200;
unsigned long lastTimeBlinked = 0;
byte blinkState = LOW;

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  digitalWrite(segments[currentLed], HIGH);
}

// in superLOOP citim valorile de pe joystick si le folosim pentru flow ul programului:
// intai afisam led urile care sunt activate, dupa blinkuim led ul curent, si updatam starea 
// ledului curent si pozitia la care suntem, dupa verificam daca cineva tine apasat 2 sec sa stim
// daca dam restart 
void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);
  
  showLeds();
  blink(currentLed);
  updateLedState(currentLed);
  updatePosition(xValue, yValue);
  checkForReset();
}

// aducem jocul in state ul initial (toate led urile off si led curent dp)
void checkForReset() {
  if (millis() - lastTimePressed >= resetTime && swState == LOW) {
    for (int i = 0; i < segSize; i++) {
      ledState[i] = LOW;
    }
    currentLed = 7; // dp
  }
}

// daca am apasat butonul de pe joystick, atunci schimbam starea ledului curent
void updateLedState(byte led) {
  if (swState != lastSwState && swState == LOW) {
    lastTimePressed = millis();
    ledState[led] = !ledState[led];
  }
  lastSwState = swState;
}

// verificam daca joysticku s-a miscat din pozitie de stat intr-o directie.
// daca da vedem in ce directie, si updatam ledul curent cu vecinul din directia 
// unde era joysticku, daca era un vecin valid 
void updatePosition(int x, int y) {
  if (x < minThreshold && !joyMoved) {        // LEFT
    joyMoved = true;
    if (neighbour[currentLed][LEFT] != OOB) {
      currentLed = neighbour[currentLed][LEFT];
    }
  }

  if (y < minThreshold && !joyMoved) {        // DOWN
    joyMoved = true;
    if (neighbour[currentLed][DOWN] != OOB) {
      currentLed = neighbour[currentLed][DOWN];
    }
  }
  
  if (y > maxThreshold && !joyMoved) {        // UP
    joyMoved = true;
    if (neighbour[currentLed][UP] != OOB) {
      currentLed = neighbour[currentLed][UP];
    }
  }
  
  if (x > maxThreshold && !joyMoved) {        // RIGHT
    joyMoved = true;
    if (neighbour[currentLed][RIGHT] != OOB) {
      currentLed = neighbour[currentLed][RIGHT];
    }
  }

  if (minThreshold < x && x < maxThreshold && 
      minThreshold < y && y < maxThreshold) {
    joyMoved = false;
  }
}

void showLeds() {
  for (int i = 0; i < segSize; i++) {
    if (i == currentLed) continue;
    digitalWrite(segments[i], ledState[i]);
  }
}

void blink(byte led) {
  if (millis() - lastTimeBlinked >= blinkInterval) {
    blinkState = !blinkState;
    lastTimeBlinked = millis();
    digitalWrite(segments[led], blinkState);
  }
}