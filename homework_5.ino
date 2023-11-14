/*
Homework 5

This sketch implements a stopwatch timer with 3 functionalities (buttons): start/pause, reset and save lap.
Reset button doesn't work while timer works.

Createed 14/11/2023
By Ana-Iulia Staci

https://github.com/iuliastaci/IntroductionToRobotics
*/

// Define connections to the shift register
const int latchPin = 11; 
const int clockPin = 10; 
const int dataPin = 12; 
// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Define variables for button pins
const int startPauseButtonPin = 13;
const int resetButtonPin = 8;
const int saveLapButtonPin = 9;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; 

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 11;

// Define byte encodings for the hexadecimal characters 0-F
int byteEncodings[encodingsNumber] = {
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
B00000001  // DP
};
// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 100; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

// Define variables for buttons interrupts
volatile bool startPauseButtonPressed = false;
volatile bool saveLapButtonPressed = false;

// Define variables for stopwatch timer
bool timerRunning = true;
bool pauseFlag = false;
int lapCounter = 0;
unsigned long lapTimes[4] = {0};

// Define variables for debounce
unsigned long debounceDelay = 50;
unsigned long lastDebounceTimeStartPause = 0;
unsigned long lastDebounceTimeReset = 0;
unsigned long lastDebounceTimeSaveLap = 0;

byte lastStartPauseState = HIGH;
byte lastResetState = HIGH;
byte lastSaveLapState = HIGH;

byte startPauseButtonState = HIGH;
byte resetButtonState = HIGH;
byte saveLapButtonState = HIGH;

bool resetButtonPressed = false;

void startPauseButtonInterrupt(){
  startPauseButtonPressed = true;
}

void saveLapButtonInterrupt(){
  saveLapButtonPressed = true;
}

void setup() {
  // Initialize the pins connected to the shift register 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize button pins
  pinMode(startPauseButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(startPauseButtonPin), startPauseButtonInterrupt, FALLING);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(saveLapButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(saveLapButtonPin), saveLapButtonInterrupt, FALLING);
  // Initialize digit control pins and set them to LOW 
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  
  Serial.begin(9600);
}

void loop() {
  debounceButtons();

  if (!pauseFlag) {
    handleRunningState();
  } else {
    handlePauseState();
  }
  // Display the current state
  writeNumber(number);
}

void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  // Initialize necessary variables for tracking the current number and digit position
  int currentNumber = number;
  int displayDigit = 3; // Start with the least significant digit
  int lastDigit = 0;

  while (displayDigit >= 0) {
  lastDigit = currentNumber % 10;
  activateDisplay(displayDigit);
  if(displayDigit == 2){
    writeReg(byteEncodings[lastDigit]+B00000001);
  }
  else
  writeReg(byteEncodings[lastDigit]);
  delay(0);
  displayDigit--;
  currentNumber /= 10;
  writeReg(B00000000);
  }
}

void debounceButtons(){
  // Debounce Start/Pause button
  byte readingStartPause = digitalRead(startPauseButtonPin);
  if (startPauseButtonPressed) {
    lastDebounceTimeStartPause = millis();
  }
  if (millis() - lastDebounceTimeStartPause > debounceDelay) {
    if (readingStartPause != startPauseButtonState) {
      startPauseButtonState = readingStartPause;
      if (startPauseButtonState == LOW) {
        pauseFlag = !pauseFlag;
      }
    }
  }

  // Debounce Reset button
  byte readingReset = digitalRead(resetButtonPin);
  if (readingReset != lastResetState) {
    lastDebounceTimeReset = millis();
  }
  if (millis() - lastDebounceTimeReset > debounceDelay) {
    if (readingReset != resetButtonState) {
      resetButtonState = readingReset;
      if (resetButtonState == LOW) {
        resetButtonPressed = true;
      }
    }
  }

  // Debounce Save Lap button
  byte readingSaveLap = digitalRead(saveLapButtonPin);
  if (saveLapButtonPressed) {
    lastDebounceTimeSaveLap = millis();
  }
  if (millis() - lastDebounceTimeSaveLap > debounceDelay) {
    if (readingSaveLap != saveLapButtonState) {
      saveLapButtonState = readingSaveLap;
      if (saveLapButtonState == LOW) {
        saveLapButtonPressed = true;
      }
    }
  }

  // Update button states
  lastStartPauseState = readingStartPause;
  lastResetState = readingReset;
  lastSaveLapState = readingSaveLap;
}

void handleRunningState(){
  if (timerRunning) {
    if (millis() - lastIncrement > delayCount) {
      number++;
      number %= 10000;
      lastIncrement = millis();
    }
  }
  // Check if the start/pause button was pressed
   if (startPauseButtonPressed) {
    startPauseButtonPressed = false;
    handleStartPauseButton();
  }

  // Check if save lab button was pressed
  if (saveLapButtonPressed) {
    saveLapButtonPressed = false;
    handleSaveLapButton();
  }
}

void handlePauseState(){
  if (resetButtonPressed) {
    resetButtonPressed = false;
    number = 0;
    pauseFlag = true;
    lapCounter = 0;
  }
}

void handleStartPauseButton(){
  if (!timerRunning) {
    // Start the timer
    timerRunning = true;
  } else {
    // Save lap time
    handleSaveLapButton();
  }
}

void handleSaveLapButton(){
    // Override the 1st saved lap with the 5th lap
    lapTimes[0] = lapTimes[1];
    lapTimes[1] = lapTimes[2];
    lapTimes[2] = lapTimes[3];
    lapTimes[3] = number;
  
}