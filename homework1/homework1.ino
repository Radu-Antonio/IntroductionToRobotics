const int potRedPin = A0;
const int potGreenPin = A1;
const int potBluePin = A2;

const int ledRedPin = 9;
const int ledGreenPin = 10;
const int ledBluePin = 11;

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;


void setup() {
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  pinMode(potRedPin, INPUT);
  pinMode(potGreenPin, INPUT);  
  pinMode(potBluePin, INPUT); 
}

// citim de la un pin analog conectat la un potentiometru, convertim valoarea voltajului in range-ul 0-255
// pt ca vrem culori, dupa care scriem la un pin de output care e conectat la o culoare 
void updateColor(int inPin, int outPin, int& potValue) {
  potValue = analogRead(inPin);
  int newPotValue = map(potValue, 0, 1023, 0, 255); 
  analogWrite(outPin, newPotValue);
}

void loop() {
  updateColor(potRedPin, ledRedPin, redPotValue);
  updateColor(potGreenPin, ledGreenPin, greenPotValue);
  updateColor(potBluePin, ledBluePin, bluePotValue);
}
