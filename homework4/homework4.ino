const int latchPin = 11; 
const int clockPin = 10; 
const int dataPin = 12; 

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int pauseButtonPin = 3;
const int resetButtonPin = 2;
const int lapButtonPin = 8;

#define COUNT_MODE 0
#define PAUSE_MODE 1
#define LAP_MODE 2

const byte button[] = {pauseButtonPin, resetButtonPin, lapButtonPin};
byte buttonState[] = {HIGH, HIGH, HIGH};
byte buttonReading[] = {HIGH, HIGH, HIGH};
byte lastButtonReading[] = {HIGH, HIGH, HIGH};
unsigned long lastTimePressedButton[] = {0, 0, 0};
const int buttonDelay = 60;

int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4;
const int encodingstime = 10;

byte byteEncodings[encodingstime] = {
  //A B C D E F G DP
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};

unsigned long delayCount = 50; 
unsigned long time = 0; 
unsigned long lastTime = 0;
unsigned long timeBeforePause = 0;

int currentMode = PAUSE_MODE;
const int maxNumOfLaps = 5;
int savedLaps[maxNumOfLaps] = {0, 0, 0, 0, 0};
int lapIndex = 0;
int lapIndexShow = 0;
int numOfSavedLaps = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(pauseButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
}

// avem 3 moduri:
// counting - in care se intampla cronometarea si putem salva lap-uri
// pause - in care putem relua cronometrul sau sa il punem pe pauza, si sa dam reset la timp
// lap - in care putem vedea timpurile salvate, sau putem da reset si le stergem pe toate

void loop() {
  switch (currentMode) {
    case COUNT_MODE: 
      writeTime(time);
      time = timeBeforePause + (millis() - lastTime) / 100;

      // daca cronometez si apas pe pauza trec in pause mode
      if (isButtonPressed(0)) { // PAUSE BUTTON
        currentMode = PAUSE_MODE;
        timeBeforePause = time;
      } 
      
      // daca cronometrez si apas pe lap button, salvez timpul curent
      if (isButtonPressed(2)) { // LAP BUTTON
        numOfSavedLaps++;
        savedLaps[lapIndex] = time;
        lapIndex = (lapIndex + 1) % maxNumOfLaps;
      }
      break;

    case PAUSE_MODE:
      writeTime(time);

      // daca sunt in pauza si apasa pe butonu de pauza, reincep cronometrarea
      if (isButtonPressed(0)) { // PAUSE BUTTON
        currentMode = COUNT_MODE;
        lastTime = millis();
      } 

      // daca sunt in pauza si apas pe reset, timpul devine 0
      if (isButtonPressed(1)) { // RESET BUTTON
        time = 0;
        timeBeforePause = 0;
      }

      // daca sunt in pauza si apas pe lap, trec in lap mode
      if (isButtonPressed(2) && time == 0) { // LAP BUTTON
        currentMode = LAP_MODE;
        if (numOfSavedLaps >= maxNumOfLaps) {
          lapIndexShow = lapIndex;
        }
        time = savedLaps[lapIndexShow];
      }
      break;

    case LAP_MODE:
      writeTime(time);

      // daca sunt in lap mode si apas pe lap button, trec circular la urmatorul timp salvat
      if (isButtonPressed(2)) { // LAP BUTTON
        if (numOfSavedLaps >= maxNumOfLaps) {
          lapIndexShow = (lapIndexShow + 1) % maxNumOfLaps;
        } else {
          lapIndexShow = (lapIndexShow + 1) % lapIndex;
        }
        time = savedLaps[lapIndexShow];
      }

      // daca sunt in lap si apas pe butonul de reset, sterg toate lap-urile
      if (isButtonPressed(1)) { // RESET BUTTON
        for (int i = 0; i < maxNumOfLaps; i++) {
          savedLaps[i] = 0;
        }
        numOfSavedLaps = 0;
        lapIndex = 0;
        lapIndexShow = 0;
        time = 0;
        currentMode = PAUSE_MODE;
      }
      break;
  }
}

// vedem daca este apasat al idx-lea buton 
bool isButtonPressed(byte idx) { 
  buttonReading[idx] = digitalRead(button[idx]);

  if (buttonReading[idx] != lastButtonReading[idx]) {
    lastTimePressedButton[idx] = millis();
  }

  if (millis() - lastTimePressedButton[idx] > buttonDelay) {
    if (buttonState[idx] != lastButtonReading[idx]) {
      buttonState[idx] = lastButtonReading[idx];

      if (buttonState[idx] == LOW) {
        lastButtonReading[idx] = buttonReading[idx];
        return true;
      }
    }
  }
  lastButtonReading[idx] = buttonReading[idx];
  return false;
}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displaytime) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displaytime], LOW);
}

// afisam timpul time in secunde pe 4 digit 7 seg display
void writeTime(unsigned long time) {
  int currentDigit = 0;
  for (int currentDisplay = 3; currentDisplay >= 0; currentDisplay--) {
    currentDigit = time % 10;
    time /= 10;

    activateDisplay(currentDisplay);
    if (currentDisplay == 2) { // suntem la display 3 deci vrem si dp ON
      writeReg(byteEncodings[currentDigit] | 1);
    } else { // daca nu afisam cifra normal
      writeReg(byteEncodings[currentDigit]);
    }
    writeReg(B00000000);
  }
}
