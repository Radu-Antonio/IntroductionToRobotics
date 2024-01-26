const int redPin = 10;
const int greenPin = 9;
const int bluePin = 8;

const int buttonPin1 = 13;
const int buttonPin2 = 12;
const int buttonPin3 = 11;

const int buttons[] = {buttonPin1, buttonPin2, buttonPin3};

byte buttonState[] = {HIGH, HIGH, HIGH};
byte buttonReading[] = {HIGH, HIGH, HIGH};
byte lastButtonReading[] = {HIGH, HIGH, HIGH};

const int debounceDelay = 60;
unsigned long int lastDebounce[] = {0, 0, 0};

const int trigPin = 6;
const int echoPin = 5;

const int audioPin = 3;
int sound = 350;
const int soundHit = 50;

float dist = 0;
bool alarmGoBrr = false;

const int passcode[] = {0, 0, 2, 1};
int idx = 0;
const int passCodeLength = 4;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(audioPin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop() {
  if (!alarmGoBrr) { // daca alarma nu i pornita, asteptam pana o declanseaza cineva (vine prea aproape de ea)
    updateLed();
    dist = getDistance();
    if (dist < 30) { // prea aproape, pornim alarma
      alarmGoBrr = true;
    }
  } else {
    if (idx == passCodeLength) { // daca am bagat parola corecta, revenim in starea intitiala
      updateLed();
      tone(audioPin, 450, 600);
      delay(600);
      alarmGoBrr = false;
      idx = 0;
      sound = 350;
    } else { // daca nu facem galagie si procesam butoanele pentru a vedea daca parola corecta este introdusa
      makeNoise();
      updateLed();
      for (int i = 0; i < 3; i++) {
        if (buttonPressed(i)) {
          if (passcode[idx] == i) 
            idx++;
          else {
            sound -= soundHit;
            idx = 0;
          }
        }
      }
    }
  }
}

void makeNoise() {
  tone(audioPin, sound, 100);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void updateLed() {
  if (!alarmGoBrr) {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
  } else {
    if (idx == passCodeLength) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
    } else {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    }
  }
}

bool buttonPressed(int idx) {
  bool isPressed = false;
  buttonReading[idx] = digitalRead(buttons[idx]);

  if (buttonReading[idx] != lastButtonReading[idx]) {
    lastDebounce[idx] = millis();
  }

  if (millis() - lastDebounce[idx] >= debounceDelay) {
    if (buttonReading[idx] != buttonState[idx]) {
      buttonState[idx] = buttonReading[idx];
      if (buttonState[idx] == LOW) {
        isPressed = true;
      }
    }
  }
  lastButtonReading[idx] = buttonReading[idx];
  return isPressed;
}