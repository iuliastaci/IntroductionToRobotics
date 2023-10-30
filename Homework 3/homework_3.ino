/*
Homework 3

This sketch creates a state-based control system that simulates a 3-floor elevator.
I am using the following components:
  * 3 red LEDs - each of these represents one of the 3 floors.
  * a green LED - it represents the elevator's operational state. It should blink when the
  elevator is moving and remain static when stationary.
  * 3 buttons - they represent the call buttons from the 3 floors. 
  * buzzer - it should sound when:
    - elevator is arriving at the desired floor
    - elevator doors are closing and movement ( 2 different sounds)
Also, this code includes debounce for button presses.

Created 28/10/2023
By Ana-Iulia Staci 

Modified 29/10/2023
By Ana-Iulia Staci

https://github.com/iuliastaci/IntroductionToRobotics

*/

// Define the pins for LEDs, buttons and buzzer
const int floorLedsPin[] = {2, 3, 4}; // Array to store pins for floor indicator LEDs
const int stateLedPin = 5; // Pin for the state indicator LED
const int callButtonsPin[] = {6, 7, 8}; // Array to store pins for call buttons
const int buzzerPin = 9; // Pin for the buzzer 

// Elevator states
enum ElevatorState {IDLE, MOVING, DOORS_OPEN, DOORS_CLOSE};

ElevatorState elevatorState = IDLE; // Initializing the elevator state
int currentFloor = 1;
int targetFloor = 1;
unsigned long lastStateChangeTime = 0;
unsigned long debounceDelay = 50; // Delay for button debouncing

byte callButtonState; // Store the state of a call button 
byte stateLedValue; // Store the state LED value (HIGH or LOW)

int doorsClosingTime = 2000; // Time it takes for doors to close (in milliseconds)
int doorsOpeningTime = 2000; // Time it takes for doors to open (in milliseconds)
int movingTime = 1000; // Time it takes for the elevator to move between floors (in milliseconds)
int buzzerSoundTime = 100; // Duration for the buzzer sound (in milliseconds)

unsigned long previousBlinkTime = 0;
unsigned long currentBlinkTime;
const unsigned long blinkingInterval = 100; // Interval for blinking the state LED


bool buzzerOn = false; // Flag to track if the buzzer is ON or OFF

int buttonPressSoundFrequency = 1000; // Frequency of the buzzer for the button press
int doorsClosingSoundFrequency = 1500; // Frequency of the buzzer for doors closing
int doorsOpeningSoundFrequency = 2000; // Frequency of the buzzer for doors opening


void setup() {
  // Initialize pins
  for(int i = 0; i < 3; i++) {
    pinMode(floorLedsPin[i], OUTPUT);
    pinMode(callButtonsPin[i], INPUT_PULLUP); // Use INPUT_PULLUP for button debouncing
  }
  pinMode(stateLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the elevator at the first floor
  digitalWrite(floorLedsPin[0], HIGH); // Turn on the first floor LED
  noTone(buzzerPin); // Turn offn the buzzer

  Serial.begin(9600);

}

void loop() {
  switch (elevatorState) {
  case IDLE:
    // Check for button presses
    for (int i = 0; i < 3; i++) {
      callButtonState = digitalRead(callButtonsPin[i]);
      if (callButtonState == LOW && millis() - lastStateChangeTime > debounceDelay) {
        targetFloor = i + 1;
        if (targetFloor != currentFloor) {
          elevatorState = DOORS_CLOSE;
          lastStateChangeTime = millis();
          // Sound for button press
          buzzerOn = true;
          tone(buzzerPin, buttonPressSoundFrequency);
        }
      }
    }
    break;

  case DOORS_CLOSE:
    if (millis() - lastStateChangeTime > doorsClosingTime) {
    elevatorState = MOVING;
    // Sound for doors closing
    buzzerOn = true;
    tone(buzzerPin, doorsClosingSoundFrequency);
    lastStateChangeTime = millis();
    }
    break;

  case MOVING:
    // Blink the state LED to indicate movement
    currentBlinkTime = millis();
    if (currentBlinkTime - previousBlinkTime >= blinkingInterval) {
      previousBlinkTime = currentBlinkTime;
      stateLedValue = !digitalRead(stateLedPin);
      digitalWrite(stateLedPin, stateLedValue);
    }
    // Check if elevator is moving up
    if (currentFloor < targetFloor) {
      if (millis() - lastStateChangeTime > movingTime) {
        digitalWrite(floorLedsPin[currentFloor - 1], LOW);
        currentFloor++;
        digitalWrite(floorLedsPin[currentFloor - 1], HIGH);
        lastStateChangeTime = millis();
        
      }
    } else if (currentFloor > targetFloor) { // Check if elevator is moving down
      if (millis() - lastStateChangeTime > movingTime) {
        digitalWrite(floorLedsPin[currentFloor - 1], LOW);
        currentFloor--;
        digitalWrite(floorLedsPin[currentFloor - 1], HIGH);
        lastStateChangeTime = millis();
      }
    } else { // Elevator arrives at the desired floor
      elevatorState = DOORS_OPEN;
      lastStateChangeTime = millis();
      digitalWrite(stateLedPin, HIGH);
    }
    break;

  case DOORS_OPEN:
    if (millis() - lastStateChangeTime > doorsOpeningTime) {
      elevatorState = IDLE;
      // Sound for doors opening
      buzzerOn = true;
      tone(buzzerPin, doorsOpeningSoundFrequency);
      currentFloor = targetFloor;
      digitalWrite(floorLedsPin[currentFloor - 1], HIGH);
      // Turn off the buzzer after opening the doors
      if (buzzerOn == true) {
        noTone(buzzerPin);
        buzzerOn = false;
      }
    }
    break;
  }

}
