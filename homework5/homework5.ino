#include <EEPROM.h>

#define mainMenu 0
#define settings 1
#define resetData 2
#define systStatus 3
#define ledControl 4

void printMainMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control\n");
}

void printSensorSettings() {
  Serial.println("1. Sensor Settings");
  Serial.println("  1. Sensors Sampling Interval");
  Serial.println("  2. Ultrasonic Alert Threshold");
  Serial.println("  3. LDR Alert Threshold");
  Serial.println("  4. Back\n");
}

void printResetLogData() {
  Serial.println("2. Reset Logger Data");
  Serial.println("  1. Yes");
  Serial.println("  2. No\n");
}

void printSystemStatus() {
  Serial.println("3. System Status");
  Serial.println("  1. Current Sensor Readings");
  Serial.println("  2. Current Sensor Settings");
  Serial.println("  3. Display Logged Data");
  Serial.println("  4. Back\n");
}

void printRgbControl() {
  Serial.println("4. RGB LED Control");
  Serial.println("  1. Manual Color Control");
  Serial.println("  2. LED: Toggle Automatic ON/OFF");
  Serial.println("  3. Back\n");
}

// lista de functii de printari
void (*selectMenuState[])() = { printMainMenu, printSensorSettings, printResetLogData, printSystemStatus, printRgbControl };
byte maxNumOfOptions[] = { 4, 4, 2, 4, 3 };
byte menuState = mainMenu;

const int photocellPin = A0; 
const int trigPin = 9;
const int echoPin = 10;
const int bluePin = 5;
const int redPin = 6;
const int greenPin = 7;

// adresele din eeprom din care citim setarile senzorilor
int adrresses[] = {0, 2, 4, 6};
int sampleInterval, minDist, minLight;
bool automaticMode;

bool isColorOn[] = {0, 1, 0};
int photoSensorVal = 0;
int distSensorVal = 0;
unsigned long lastTimeRead = 0;

const int maxNumOfReadings = 10;
int ultraSonicReadings[maxNumOfReadings];
int photoReadings[maxNumOfReadings];
int currentIndex = 0;

// setam pinii si citim din memorie setarile sezorilor
void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(photocellPin, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  EEPROM.get(adrresses[0], sampleInterval); 
  EEPROM.get(adrresses[1], minDist); 
  EEPROM.get(adrresses[2], minLight); 
  EEPROM.get(adrresses[3], automaticMode); 
  
  Serial.begin(9600);
  printMainMenu();
}

// cand citim ceva alegem o optiune pe baza a ce am citit si mereu citim senzorii
void loop() {
  if (Serial.available()) {
    int choice = Serial.parseInt();
    chooseOption(choice);
  }
  readSensors(0);
  blink();
}

// ledul se activeaza cand pragul minim este depasit de oricare din senzori
void blink() {
  bool isOverLimit = getDistance() < minDist || getPhotocellValue() < minLight;
  if (automaticMode) {
    digitalWrite(redPin, isOverLimit);
    digitalWrite(greenPin, !isOverLimit);
  } else if (isOverLimit) {
    digitalWrite(redPin, isColorOn[0]);
    digitalWrite(greenPin, isColorOn[1]);
    digitalWrite(bluePin, isColorOn[2]);
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
}

// o functie care citeste valorile senzorilor la un interval dat, si 
// updateaza LOG ul cu citirile 
void readSensors(bool show) {
  if (millis() - lastTimeRead > sampleInterval) {
    distSensorVal = getDistance();
    photoSensorVal = getPhotocellValue();

    if (show) {
      Serial.print("=============================\n");
      Serial.print("UltraSonicSensor reading: ");
      Serial.println(distSensorVal);
      Serial.print("Photosensor reading: ");
      Serial.println(photoSensorVal);
    }

    if (currentIndex < maxNumOfReadings) {
      ultraSonicReadings[currentIndex] = distSensorVal;
      photoReadings[currentIndex] = photoSensorVal;
      currentIndex++;
    } else {
      for (int i = 0; i < maxNumOfReadings - 1; i++) {
        ultraSonicReadings[i] = ultraSonicReadings[i+1];
        photoReadings[i] = photoReadings[i+1];
      }
      ultraSonicReadings[maxNumOfReadings - 1] = distSensorVal;
      photoReadings[maxNumOfReadings - 1] = photoSensorVal;
    }
    lastTimeRead = millis();
  }
}

// citim din photosenzor
int getPhotocellValue() {
  return analogRead(photocellPin);
}

// citim din ultrasonic senzor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void chooseOption(int choice) {
  // daca optiunea aleasa este invalida afisam din nou meniul/submeniul corespunzator
  if (!(0 < choice && choice <= maxNumOfOptions[menuState])) {
    Serial.println("The selected option is not available, please try again:\n");
    selectMenuState[menuState]();
    return;
  } 

  // in functie de ce meniu/submeniu suntem si opiunea citita trebuie sa facem 
  // cate o alta actiune
  switch (menuState) {
    // daca sunt in main meniu in functie de ce numar aleg ma duc in submeniul corespunzator
    case mainMenu:
      menuState = choice;
      break;

    // daca sunt in settings updatez valoarea unei setari ale senzorilor
    case settings:
      if (choice == 1)      
        setSampleInterval();
      else if (choice == 2) 
        setUltrasonicSensorThreshold();
      else if (choice == 3) 
        setPhotoSensorThreshold();
      else
        menuState = mainMenu;
      break;

    // daca sunt in optiunea de reset LOG DATA si apas 1 dau un mesaj de confirmare
    // daca nu apas 1 ma intorc la meniul principal
    case resetData:
      if (choice == 1) {
        Serial.println("Are you sure? Y / N");
        while (Serial.available() <= 0) {} 
        if (Serial.available() > 0 && 'Y' == (char)Serial.read()) {
          currentIndex = 0;
          Serial.println("data reset complete\n");
        }
      } 
      menuState = mainMenu;
      break;

    // daca sunt in system status in functie de optiune afisez
    // citirile / setarile curente ale senzorilor
    case systStatus:
      if (choice == 1) 
        showSensorsReadings();
      else if (choice == 2)
        showSensorsSettings();
      else if (choice == 3)
        showLoggedData();
      else
        menuState = mainMenu;
      break;

    // daca sunt in led control atunci in functie de optiune schimb modul automat
    // sau schimb culorile custom ale led ului
    case ledControl:
      if (choice == 1)
        setRgbValues();
      else if (choice == 2)
        toggleAutomaticMode();
      else
        menuState = mainMenu;
      break;
  }
  selectMenuState[menuState]();
}

void setSampleInterval() {
  Serial.println("Select a value (1 - 10) for the sampling interval:");
  while (Serial.available() <= 0) {} 

  if (Serial.available() > 0) {
    int interval = Serial.parseInt();
    if (!(0 < interval && interval <= 10)) {
      Serial.println("That's not a valid value.");
      return;
    }
    sampleInterval = 1000 * interval;
    EEPROM.put(adrresses[0], sampleInterval);
    Serial.print("Interval set to ");
    Serial.print(interval);
    Serial.println(" seconds.\n");
  }
};

void setUltrasonicSensorThreshold() {
  Serial.println("Select a value for the ultrasonic sensor (1 - 100) cm:");
  while (Serial.available() <= 0) {} 

  if (Serial.available() > 0) {
    int val = Serial.parseInt();
    
    if (!(0 < val && val <= 100)) {
      Serial.println("That's not a valid value.");
      return;
    }
    minDist = val;
    EEPROM.put(adrresses[1], minDist);
    Serial.print("Ultrasonic sensor minim Threshold set to ");
    Serial.print(val);
    Serial.println(" cm.\n");
  }
};

void setPhotoSensorThreshold() {
  Serial.println("Select a value for the photosensor (50 - 900):");
  while (Serial.available() <= 0) {} 

  if (Serial.available() > 0) {
    int val = Serial.parseInt();
    
    if (!(50 < val && val <= 900)) {
      Serial.println("That's not a valid value.");
      return;
    }
    minLight = val;
    EEPROM.put(adrresses[2], minLight);
    Serial.print("Photosensor minim Threshold set to ");
    Serial.print(val);
    Serial.println(".\n");
  }
};

void showSensorsReadings() {
  Serial.println("Showing current sensor readings.");
  Serial.println("To exit press any key, then ENTER");
  while (Serial.available() <= 0) {
    readSensors(1);
    blink();
  } 
  Serial.parseInt();
}

void showSensorsSettings() {
  Serial.println("Sensors settings:");
  Serial.print("Sample rate: ");
  Serial.print(sampleInterval / 1000);
  Serial.println(" seconds");
  Serial.print("Minimum threshold value for Ultrasonic sensor: ");
  Serial.print(minDist);
  Serial.println(" cm");
  Serial.print("Minimum threshold value for Photo sensor: ");
  Serial.println(minLight);
  Serial.println();
}

void showLoggedData() {
  Serial.print("Showing last ");
  Serial.print(currentIndex);
  Serial.println(" measurements for ultrasonic sensor:");
  for (int i = 0; i < currentIndex; i++) {
    Serial.print(ultraSonicReadings[i]);
    Serial.println(" cm");
  }
  Serial.println();
  Serial.print("Showing last ");
  Serial.print(currentIndex);
  Serial.println(" measurements for photo sensor:");
  for (int i = 0; i < currentIndex; i++) {
    Serial.println(photoReadings[i]);
  }
  Serial.println();
}

void setRgbValues() {
  Serial.println("Set the red value (1 - ON, everyhting else - OFF): ");
  while (Serial.available() <= 0) {}
  isColorOn[0] = Serial.parseInt() == 1;
  
  Serial.println("Set the green value (1 - ON, everyhting else - OFF): ");
  while (Serial.available() <= 0) {}
  isColorOn[1] = Serial.parseInt() == 1;

  Serial.println("Set the blue value (1 - ON, everything else - OFF):");
  while (Serial.available() <= 0) {}
  isColorOn[2] = Serial.parseInt() == 1;

  Serial.println("Colos set!");
}

void toggleAutomaticMode() {
  automaticMode = !automaticMode;
  EEPROM.put(adrresses[3], automaticMode);
  Serial.print("Automatic mode is now ");
  Serial.println(automaticMode ? "ON" : "OFF");
}
