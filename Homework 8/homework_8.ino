#include "LedControl.h"
#include <LiquidCrystal.h>
#include "EEPROM.h"

// Define pins for matrix
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;

// Define pins for LCD
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte pwmPin = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte buzzerPin = 2;

// Define pins for the joystick
const int pinY = A1;  // Analog pin connected to X output
const int pinX = A0;  // Analog pin connected to Y output
const int pinSW = 13;

// Define game variables
const int patternLengths[] = { 3, 5, 7 };  // Pattern lengths for each difficulty level
const int numLevels = 3;                   // Number of difficulty levels
int currentLevel = 1;                      // Current difficulty level
int patternIndex = 0;                      // Index of the current step in the pattern
int playerIndex = 0;                       // Index of the player's current step
int pattern[7];                            // Array to store the pattern
int playerPattern[7];                      // Array to store the player's input pattern
bool gameOver = false;                     // Flag to indicate if the game is over
unsigned long startTime = 0;               // Start time of the pattern
unsigned long endTime = 0;                 // End time of the pattern
unsigned long score = 0;                   // Player's score
bool gameStarted = false;                  // Flag to indicate if the game has started
bool difficultySelected = false;           // Flag to indicate if the difficulty has been selected
const int blinkInterval = 300;
const int rapidBlinkInterval = 100;
unsigned long lastBlink = 0;
byte ledState = LOW;
int noOfTurnedOnLEDs = 0;
const int displayDelay = 1000;
int lastTimeDisplayed = 0;
const int fiveSDelay = 5000;
const int twoSDelay = 2000;
const int shortDelay = 200; 


// Define variables for joystick
int playerX = 0;  // Player's X position
int playerY = 0;  // Player's Y position
int xValue = 0;
int yValue = 0;
byte swState = LOW;
const int minThreshold = 400;
const int maxThreshold = 600;
bool joyMoved = false;
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
const int noOfDirections = 4;

// Define variables for sw button debounce
long lastTimeSWPressed = 0;
const int debounceDelay = 50;
byte currentSWRead = LOW;


// Define variable for button interrupt
volatile bool swPressed = false;

// Define variables for menu
const char* menuItems[] = { "Start Game", "Settings", "About" };
const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);
int currentMenuItem = 0;

bool currentMatrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

void buttonPressed() {
  swPressed = true;
}

void setup() {

  matrixBrightness = EEPROM.read(0);
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);

  lcd.begin(16, 2);

  // Initialize joystick pins
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonPressed, FALLING);

  Serial.begin(9600);

  //Display an intro message on lcd via a function call
  displayIntro();

  resetGame();  // Reset the game variables
  // Generate a random pattern for the current level
  generatePattern();

  displayMenu();
}


void loop() {
  if (gameStarted) {
    playGame();
  } else {
    readJoystick();
    updateMenu();
  }
}

void playGame() {
  // Check if the game is over
  if (gameOver) {
    Serial.println("Game over");
    displayGameOver();
    // TO DO: Add function to save score
    delay(5000);
    resetGame();
    displayPattern();
    gameStarted = false;
  } else {
    // Check if the player has completed the pattern
    if (noOfTurnedOnLEDs == patternLengths[currentLevel - 1]) {
      Serial.println("Win condition");
      displayWin();
      delay(5000);
      noOfTurnedOnLEDs = 0;
      if (currentLevel < numLevels) {
        increaseLevel();
        resetGame();
        generatePattern();
      } else {
        // The player has completed all levels
        gameStarted = false;
      }
    } else {
      // The player is still playing the current level
      moveJoystick(joyMoved);
      buttonPress();
      checkGameStatus();

      for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
          if (i == playerX && j == playerY) {
            if (currentMatrix[playerX][playerY] == 1) {
              blinkPlayerLED(rapidBlinkInterval);
              // blinked the player LED more quickly when that LED is already on
            } else {
              blinkPlayerLED(blinkInterval);
              // blink the player LED normally when that LED is off
            }
          } else if (currentMatrix[i][j] == 1) {
            lc.setLed(0, i, j, true);
          } else {
            lc.setLed(0, i, j, false);
          }
        }
      }
      if (millis() - lastTimeDisplayed >= displayDelay) {
        score = (millis() - startTime) / 1000;
        displayScore();
        lastTimeDisplayed = millis();
      }
    }
  }
}

void checkGameStatus() {
  noOfTurnedOnLEDs = 0;
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      // check no of turned on LEDs corresponding to the pattern
      if (i < patternLengths[currentLevel - 1] && j == pattern[i] && currentMatrix[i][j] == 1) {
        noOfTurnedOnLEDs++;
      }

      // Check if exists a turned on LED that is not in the pattern
      if (i >= patternLengths[currentLevel - 1] && currentMatrix[i][j] == 1) {
        gameOver = true;
        return;
      }
      if (i < patternLengths[currentLevel - 1] && j != pattern[i] && currentMatrix[i][j] == 1) {
        gameOver = true;
        return;
      }
    }
  }
}

void moveJoystick(bool& joyMoved) {
  // Read the joystick values
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (xValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    movePlayer(up);
  }
  // Check if the joystick was moved down
  if (xValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    movePlayer(down);
  }
  // Check if the joystick was moved left
  if (yValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    movePlayer(left);
  }
  // Check if the joystick was moved right
  if (yValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    movePlayer(right);
  }

  if (xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold) {
    joyMoved = false;
  }
}


void movePlayer(int direction) {
  // Clear the previous player position
  Serial.print("Player move ");
  Serial.println(direction);
  lc.setLed(0, playerX, playerY, false);

  // Move the player in the specified direction
  switch (direction) {
    case up:
      playerY--;
      break;
    case down:
      playerY++;
      break;
    case left:
      playerX--;
      break;
    case right:
      playerX++;
      break;
  }

  // Check if the player has moved off the matrix
  if (playerX < 0) {
    playerX = matrixSize - 1;
  }
  if (playerX >= matrixSize) {
    playerX = 0;
  }
  if (playerY < 0) {
    playerY = matrixSize - 1;
  }
  if (playerY >= matrixSize) {
    playerY = 0;
  }
  // Set the new player position -  blink the player LED depending on its state (on or off)
  if (currentMatrix[playerX][playerY] == 1) {
    //blinkPlayerLED(rapidBlinkInterval);
    // blinked the player LED more quickly when that LED is already on
  } else {
    //blinkPlayerLED(blinkInterval);
    // blink the player LED normally when that LED is off
  }
  lc.setLed(0, playerX, playerY, true);
}

void buttonPress() {
  if (swPressed) {
    swPressed = false;
    lastTimeSWPressed = millis();
  }
  if (millis() - lastTimeSWPressed >= debounceDelay) {
    currentSWRead = digitalRead(pinSW);
    if (currentSWRead != swState) {
      swState = currentSWRead;
      if (swState == LOW) {
        if (currentMatrix[playerX][playerY] == 1) {
          currentMatrix[playerX][playerY] = 0;  // turn off the LED at player position
        } else {
          currentMatrix[playerX][playerY] = 1;  // turn on the LED at player position
        }
      }
    }
  }
}

void generatePattern() {
  // Generate a random pattern for the current level
  for (int i = 0; i < patternLengths[currentLevel - 1]; i++) {
    pattern[i] = random(8);  // Generate a random number between 0 and 7
  }
  // Display the pattern on the matrix
  displayPattern();
  // Start the timer
  startTime = millis();
  // Display player at start position
  blinkPlayerLED(blinkInterval);
}

void displayPattern() {
  // Display the pattern on the matrix
  lc.clearDisplay(0);
  for (int i = 0; i < patternLengths[currentLevel - 1]; i++) {
    lc.setLed(0, i, pattern[i], true);  // Turn on the LED at the specified position
  }
  delay(fiveSDelay);         // Display the pattern for 5 seconds
  lc.clearDisplay(0);  // Clear the matrix display
}

void displayIntro() {
  // Display an intro message on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test your memory");
  lcd.setCursor(0, 1);
  lcd.print("Press to start");
}

void displayGameOver() {
  // Display "Game Over" on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
}

void displayWin() {
  // Display "You Win!" on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You Win!");
}

void displayScore() {
  // Display the player's score on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
}

// TO DO: Modify this function to use update instead of write
// void saveScore() {
//   // Save the player's score to EEPROM
//   EEPROM.write(currentLevel, score);
// }

void increaseLevel() {
  // Increase the difficulty level and reset the player's position
  currentLevel++;
  playerX = 0;
  playerY = 0;
  score = 0;
}

void resetGame() {
  // Reset the game variables
  gameOver = false;
  patternIndex = 0;
  playerIndex = 0;
  score = 0;
  // Clear the matrix display
  lc.clearDisplay(0);
  // Generate a new pattern for the current level
  // generatePattern();
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++) {
      currentMatrix[i][j] = 0;
    }
}

void blinkPlayerLED(int delayTime) {
  // Blink the LED for the player on the matrix
  if (millis() - lastBlink >= delayTime) {
    lastBlink = millis();
    ledState = !ledState;
    lc.setLed(0, playerX, playerY, ledState);
  }
}

//Functions used for main menu and submenus
// void displayMainMenu() {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Main Menu");
//   lcd.setCursor(0, 1);
//   lcd.print("(a) Start Game");
//   lcd.setCursor(0, 2);
//   lcd.print("(b) Settings");
//   lcd.setCursor(0, 3);
//   lcd.print("(c) About");
// }

// void printSettingsMenu() {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Settings");
//   lcd.setCursor(0, 1);
//   lcd.print("(a) Change LCD Brightness");
//   lcd.setCursor(0, 2);
//   lcd.print("(b) Change Matrix Brightness");
//   lcd.setCursor(0, 3);
//   lcd.print("(c) Back");
// }

// void printAbout() {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.autoscroll();
//   lcd.print("MindMatrix Challenge");
//   lcd.noAutoscroll();
//   lcd.setCursor(0, 1);
//   lcd.autoscroll();
//   lcd.print("Made by Ana-Iulia Staci");
//   lcd.noAutoscroll();
//   lcd.setCursor(0, 2);
//   lcd.autoscroll();
//   lcd.print("https://github.com/iuliastaci");
//   lcd.noAutoscroll();
// }

void readJoystick() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
}

void updateMenu() {
  if (yValue < minThreshold) {
    // Joystick moved up
    moveUp();
  } else if (yValue > maxThreshold) {
    // Joystick moved down
    moveDown();
  }

  if (swPressed) {
    swPressed = false;
    lastTimeSWPressed = millis();
  }
  if (millis() - lastTimeSWPressed >= debounceDelay) {
    currentSWRead = digitalRead(pinSW);
    if (currentSWRead != swState) {
      swState = currentSWRead;
      if (swState == LOW) {
        selectMenuItem();
      }
    }
  }
}

void moveUp() {
  if (currentMenuItem > 0) {
    currentMenuItem--;
    displayMenu();
    delay(shortDelay);  // Delay for visual feedback
  }
}

void moveDown() {
  if (currentMenuItem < numMenuItems - 1) {
    currentMenuItem++;
    displayMenu();
    delay(shortDelay);  // Delay for visual feedback
  }
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(2, 0);
  lcd.print(menuItems[currentMenuItem]);

  lcd.setCursor(0, 1);
  lcd.print("                ");  // Clear the second row

  lcd.setCursor(0, 1);
  lcd.print(menuItems[(currentMenuItem + 1) % numMenuItems]);

  delay(100);  // A short delay to avoid flickering
}

void selectMenuItem() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Selected: ");
  lcd.setCursor(0, 1);
  lcd.print(menuItems[currentMenuItem]);

  // Perform action based on selected menu item
  switch (currentMenuItem) {
    case 0:
      // Start game
      lcd.setCursor(0, 1);
      lcd.print("Starting game...");
      delay(twoSDelay);  // Simulate starting the game
      gameStarted = true;
      break;
    case 1:
      // Settings submenu
      currentMenuItem = 0;  // Reset submenu to the first item
      displaySubMenu(menuItems[1]);
      break;
    case 2:
      // About submenu
      currentMenuItem = 0;  // Reset submenu to the first item
      displaySubMenu(menuItems[2]);
      break;
      // Add more cases for additional menu items
  }

  delay(twoSDelay);    // Display selected menu item for 2 seconds
  displayMenu();  // Return to the main menu
}

void displaySubMenu(const char* submenuTitle) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(submenuTitle);

  // Define submenu items
  const char* submenuItems[] = { "Option 1", "Option 2", "Option 3", "Back" };
  const int numSubMenuItems = sizeof(submenuItems) / sizeof(submenuItems[0]);

  int submenuSelection = 0;

  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear the second row

    lcd.setCursor(0, 1);
    lcd.print(submenuItems[submenuSelection]);

    readJoystick();

    if (yValue < minThreshold) {
      // Joystick moved up in submenu
      if (submenuSelection > 0) {
        submenuSelection--;
      }
    } else if (yValue > maxThreshold) {
      // Joystick moved down in submenu
      if (submenuSelection < numSubMenuItems - 1) {
        submenuSelection++;
      }
    }

    if (swPressed) {
      swPressed = false;
      lastTimeSWPressed = millis();
    }
    if (millis() - lastTimeSWPressed >= debounceDelay) {
      currentSWRead = digitalRead(pinSW);
      if (currentSWRead != swState) {
        swState = currentSWRead;
        if (swState == LOW) {
          if (submenuSelection == numSubMenuItems - 1) {
            return; // Back to main menu
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Selected: ");
          lcd.setCursor(0, 1);
          lcd.print(submenuItems[submenuSelection]);

          // Perform action based on selected submenu item
          // Add your submenu-specific logic here

          delay(twoSDelay);  // Display selected submenu item for 2 seconds
          return;
        }
      }
    }


    delay(shortDelay);  // Adjust delay as needed
  }
}
