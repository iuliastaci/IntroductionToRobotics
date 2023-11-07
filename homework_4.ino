/*
Homework 4

This sketch use the position of the joystick to draw on a 7-segment display. 
Behaviour:
  ~The segments should jump only to neighbours, without moving through obstacles.
  ~The initial position is on the DP.
  ~The current postion always blinks.
  ~Short pressing the switch button toggles the segment state (from ON to OFF and vice-versa).
  ~Long pressing the switch button resets the entire display (turn off all the segmets and 
  set the current postion to the DP).

Created 05/11/2023
By Ana-Iulia Staci

Modified 06/11/2023
By Ana-Iulia Staci

https://github.com/iuliastaci/IntroductionToRobotics

*/

// Define pins for the 7-segment display
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;


// Define pins for the joystick
const int pinX = A0; // Analog pin connected to X output
const int pinY = A1; // Analog pin connected to Y output
const int pinSW = 2;

// Define variables for controlling 7-segment display
const int segSize = 8;
bool commonAnode = false;

// States of segments
byte segmentState = LOW;
byte dpState = LOW;

// Define variables for joystick
int xValue = 0;
int yValue = 0;
byte swState = LOW;
const int minThreshold = 400;
const int maxThreshold = 600;
bool joyMoved = false;
long lastTimeJoyMoved = 0;

// Define variables for button debounce 
long lastTimeSWPressed = 0;
int shortInterval = 50;
int longInterval = 1000;
byte currentSWRead = LOW;
byte previousSWRead = LOW;
byte currentJoyRead = LOW;
byte previousJoyRead = HIGH;
byte longSWPress = LOW;
volatile bool swPressed = false;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int segmentStates[segSize] = {
  segmentState, segmentState, segmentState, segmentState, segmentState, segmentState, segmentState, segmentState
};

// Define variables for changing states
bool currentState = true; // state 1, false for state 2
bool changeState = false; // true - changing to state 2

// Define variables for blinking
unsigned long blinkInterval = 300;
unsigned long lastBlink = 0;


// Define segment matrix for 'natural' movement between segments
const int noOfDirections = 4;
const byte segmentMatrix[segSize][noOfDirections] = {
  {-1, 6, 5, 1}, // 0 - a
  {0, 6, 5, -1}, // 1 - b
  {6, 3, 4, 7}, // 2 - c
  {6,-1, 4, 2}, // 3 - d
  {6, 3,-1, 2}, // 4 - e
  {0, 6,-1, 1}, // 5 - f
  {0, 3,-1,-1}, // 6 - g
  {-1,-1, 2,-1}  // 7 - dp
// up, down, left, right
// -1 = N/A
};
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
int neighbourSegment = 0;

// Define current segment position
int currentSegment = segSize - 1; // Initial position on DP
int previousSegment = segSize - 1;


// Interrupt service routine for button press
void buttonInterrupt() {
  swPressed = true;
}

// Setup function
void setup() {
  // Initialize 7-segment display pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  
  // Initialize joystick pins
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  // Initialize button pin with an internal pull-up resistor
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonInterrupt, FALLING);

  if (commonAnode == true) {
    segmentState = !segmentState;
  }

  resetDisplay();
  Serial.begin(9600);
}


// Loop function
void loop() {
  //Check if the switch button was pressed
  if (swPressed) {
    swPressed = false;
    lastTimeSWPressed = millis();
  }

  // Check for long sw button press + debounce
  if (millis() - lastTimeSWPressed > longInterval) {
    currentSWRead = digitalRead(pinSW);
    if (currentSWRead != longSWPress) {
      longSWPress = currentSWRead;
      if (longSWPress == LOW) {
        resetDisplay();
      }
    }
    previousSWRead = currentSWRead;
  }
  // Check for short sw button press + debounce
  else if (millis() - lastTimeSWPressed > shortInterval) {
    currentSWRead = digitalRead(pinSW);
    if (currentSWRead != swState) {
      swState = currentSWRead;
      if (swState == LOW) {
        currentState != currentState;
        segmentStates[currentSegment] = !segmentStates[currentSegment];
      }
    }
    previousSWRead = currentSWRead;
  }
  
  // Check for state 1
  if (currentState) {
    // Blink the LED when selecting the segment
    if (millis() - lastBlink >= blinkInterval) {
      lastBlink = millis();
      segmentState = !segmentState;
      digitalWrite(segments[currentSegment], segmentState);
    }
    // Read joystick values
    int xValue = analogRead(pinX);
    int yValue = analogRead(pinY);
    // Check if the joystick was moved up
    if (xValue > maxThreshold && joyMoved == false) {
      joyMoved = true;
      moveSegment(up);
    }
    // Check if the joystick was moved down
    if (xValue < minThreshold && joyMoved == false) {
      joyMoved = true;
      moveSegment(down);
    }
    // Check if the joystick was moved left
    if (yValue < minThreshold && joyMoved == false) {
      joyMoved = true;
      moveSegment(left);
    }
    // Check if the joystick was moved right
    if (yValue > maxThreshold && joyMoved == false) {
      joyMoved = true;
      moveSegment(right);
    }
    // Check if the joystick was just a bit 
    if ((xValue > minThreshold && xValue < maxThreshold) || (yValue > minThreshold && yValue < maxThreshold)) {
      currentJoyRead = LOW;
    }
    // Check if the joystick was moved in any direction
    if ((xValue < minThreshold || xValue > maxThreshold) || (yValue < minThreshold || yValue > maxThreshold )) {
      currentJoyRead = HIGH;
    }
    if (currentJoyRead != previousJoyRead) {
      lastTimeJoyMoved = millis();
      if (currentJoyRead == HIGH) {
        joyMoved = false;
      }
    }
    previousJoyRead = currentJoyRead;
  }
  // State 2
  else {
    if (changeState == false) {
      changeState = true;
      // Stop blinking the segment
      digitalWrite(segments[currentSegment], segmentStates[currentSegment]);
    }
  }
  
 
}

void moveSegment(int direction){
  neighbourSegment = segmentMatrix[currentSegment][direction];
  if (neighbourSegment != -1) {
    previousSegment = currentSegment;
    currentSegment = neighbourSegment;
    digitalWrite(segments[previousSegment], segmentStates[previousSegment]);
  }
}

void resetDisplay(){
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segments[i], LOW);
  }
  currentSegment = segSize - 1;
  currentState = true;
}
