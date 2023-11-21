/*
  Homework 6

  This sketch simulate a Smart Environment Monitor and Logger. 
  It utilize an Ultrasonic Sensor (HC-SR04) and a LDR to gather environmental data.
  Data for sensors and RGB LED are stored on EEPROM. This whole system focuses on 
  integrating sensor readings, memory management, Serial Communication and menu building.

  The idea with InputContext was developed in collaboration with my colleague, Tudor Butufei,
  because I have encountered some errors while reading the values for sampling rate and sensors threshold.

  Created 19/11/2023
  By Ana - Iulia Staci

  Modified 20/11/2023
  By Ana - Iulia Staci

  Modified 21/11/2023
  By Ana - Iulia Staci
*/

#include <EEPROM.h>

//Define RGB LED pins
const int redChannelLEDPin = 9;
const int greenChannelLEDPin = 10;
const int blueChannelLEDPin = 11;

// Define photocell pin
const int photocellPin = A0;

// Define Ultrasonic Sensor pins
const int trigPin = 6;
const int echoPin = 5;

// Define variables for sensors
int sensorsSamplingRate = 0;
int ultrasonicAlertThreshold = 0;
int ldrAlertThreshold = 0;
float ldrReadings[10] ;
float ultrasonicReadings[10];
int index = 0;
int samplingRateAddress = 0;
int ultrasonicAlertThresholdAddress = 1;
int ldrAlertThresholdAddress = 2;
unsigned int currentSamplingRate;
int currentUltrasonicAlertThreshold;
int currentLDRAlertThreshold;
long duration = 0;
int distance = 0;
unsigned long lastReadingTime = 0;
int photocellValue;
const int noOfReadings = 10;

// Define variables for RGB LED
int redChannelLEDValue = 0;
int greenChannelLEDValue = 0;
int blueChannelLEDValue = 0;
bool ledToggleAutomaticMode = false;
int redChannelAddress = 3;
int greenChannelAddress = 4;
int blueChannelAddress = 5;

// Define variables for menu
enum Menus {MAIN_MENU, SENSOR_SETTINGS, RESET_LOGGED_DATA, SYSTEM_STATUS, RGB_LED_CONTROL};
Menus currentMenu = MAIN_MENU;

enum InputContext {MENU_CHOICE, SAMPLING_RATE, ULTRASONIC_THRESHOLD, LDR_THRESHOLD, RED_CHANNEL, GREEN_CHANNEL, BLUE_CHANNEL, DISPLAY_SENSOR_VALUES};
int currentInputContext = MENU_CHOICE;

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redChannelLEDPin, OUTPUT);
  pinMode(greenChannelLEDPin, OUTPUT);
  pinMode(blueChannelLEDPin, OUTPUT);

  pinMode(photocellPin, INPUT);

  Serial.begin(9600);

  printMenu();
}

void loop() {
  if (Serial.available()) {
    int menuChoice;
    switch (currentInputContext) {
      case MENU_CHOICE:
        menuChoice = Serial.parseInt();
        menuInteraction(menuChoice);
        break;
      case SAMPLING_RATE:
        sensorsSamplingRate = Serial.parseInt();
        // Sampling rate must be an integer between 1 and 10
        if (sensorsSamplingRate >=1 && sensorsSamplingRate <= 10) {
          EEPROM.update(samplingRateAddress, sensorsSamplingRate);
          currentInputContext = MENU_CHOICE;
          printSubmenu1();
        }
        else {
          Serial.println("\nInserted value is outside interval limits. Please try again.");
        }
        break;
      case ULTRASONIC_THRESHOLD:
        ultrasonicAlertThreshold = Serial.parseInt();
        EEPROM.update(ultrasonicAlertThresholdAddress, ultrasonicAlertThreshold);
        printSubmenu1();
        break;
      case LDR_THRESHOLD:
        ldrAlertThreshold = Serial.parseInt();
        EEPROM.update(ldrAlertThresholdAddress, ldrAlertThreshold);
        printSubmenu1();
        break;
      case RED_CHANNEL:
        redChannelLEDValue = Serial.parseInt();
        EEPROM.update(redChannelAddress,redChannelLEDValue);
        currentInputContext = GREEN_CHANNEL;
        Serial.println("Please insert a value between 0 and 255 for RGB LED's Green Channel:");
        break;
      case GREEN_CHANNEL:
        greenChannelLEDValue = Serial.parseInt();
        EEPROM.update(greenChannelAddress, greenChannelLEDValue);
        currentInputContext = BLUE_CHANNEL;
        Serial.println("Please insert a value between 0 and 255 for RGB LED's Blue Channel:");
        break;
      case BLUE_CHANNEL:
        blueChannelLEDValue = Serial.parseInt();
        EEPROM.update(blueChannelAddress, blueChannelLEDValue);
        currentInputContext = MENU_CHOICE;
        updateRGBLed();
        printSubmenu4();
        break;
      case DISPLAY_SENSOR_VALUES:
        currentInputContext = MENU_CHOICE;
        Serial.readString();
        printSubmenu3();
        break;
      default:
        Serial.println("ccc");
        break;
    }
    
  }
  sensorsInteraction();
}

// Function to display the main menu
void printMenu(){
  Serial.println("Main Menu:");
  Serial.println("Please select an option:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

// Functions to display submenus
void printSubmenu1(){
  Serial.println("\t1. Sensors Sampling Interval");
  Serial.println("\t2. Ultrasonic Alert Threshold");
  Serial.println("\t3. LDR Alert Threshold");
  Serial.println("\t4. Back");
}

void printSubmenu2(){
  Serial.println("\tAre you sure?");
  Serial.println("\t1. Yes");
  Serial.println("\t2. No");
}

void printSubmenu3(){
  Serial.println("\t1. Current Sensor Readings");
  Serial.println("\t2. Current Sensor Settings");
  Serial.println("\t3. Display Logged Data");
  Serial.println("\t4. Back");
}

void printSubmenu4(){
  Serial.println("\t1. Manual Color Control");
  Serial.println("\t2. LED: Toggle Automatic ON/OFF");
  Serial.println("\t3. Back");
}

// Function to display submenus based on user's choice
void mainMenuInteraction(int option){
  switch (option) {
    case 1:
      printSubmenu1();
      currentMenu = SENSOR_SETTINGS;
      break;
    case 2:
      printSubmenu2();
      currentMenu = RESET_LOGGED_DATA;
      break;
    case 3:
      printSubmenu3();
      currentMenu = SYSTEM_STATUS;
      break;
    case  4:
      printSubmenu4();
      currentMenu = RGB_LED_CONTROL;
      break;
    default:
      Serial.println("Invalid option. Please select other option.");
      printMenu();
  }
}

// Function to interact with the system based on user's options
void menuInteraction(int menuOption){
  switch (currentMenu) {
    case MAIN_MENU:
      mainMenuInteraction(menuOption);
      break;
    case SENSOR_SETTINGS: 
      sensorSettingsInteraction(menuOption);
      break;
    case RESET_LOGGED_DATA:
      resetLoggedDataInteraction(menuOption);
      break;
    case  SYSTEM_STATUS:
      systemStatusInteraction(menuOption);
      break;
    case RGB_LED_CONTROL:
      rgbLedInteraction(menuOption);
      break;
  }

}

// Function for 1. Sensor Settings
void sensorSettingsInteraction(int submenuOption){
  switch (submenuOption) {
        case 1: // Sensor Sampling Interval
          currentInputContext = SAMPLING_RATE;
          Serial.println("Please insert an integer value between 1 and 10 for sampling rate:");
          break;
        case 2: // Ultrasonic Alert Threshold
          currentInputContext = ULTRASONIC_THRESHOLD;
          Serial.println("Please insert an integer value for minimum Ultrasonic Alert Threshold:");
          break;
        case 3: // LDR Alert Threshold
          currentInputContext = LDR_THRESHOLD;
          Serial.println("Please insert an integer value for minimum LDR Alert Threshold:");
          break;
        case 4: // Back
          printMenu();
          currentMenu = MAIN_MENU;
          break;
        default:
          Serial.println("Invalid option. Please select other option.");
          printSubmenu1();
      }
}

// Function for 2. reset Logger Data
void resetLoggedDataInteraction(int submenuOption){
  switch (submenuOption) {
        case 1: // Yes
          sensorsSamplingRate = 0;
          EEPROM.update(samplingRateAddress, sensorsSamplingRate);
          ultrasonicAlertThreshold = 0;
          EEPROM.update(ultrasonicAlertThresholdAddress, ultrasonicAlertThreshold);
          ldrAlertThreshold = 0;
          EEPROM.update(ldrAlertThresholdAddress, ldrAlertThreshold);
          printMenu();
          currentMenu = MAIN_MENU;
          break;
        case 2: // No
          printMenu();
          currentMenu = MAIN_MENU;
          break;
        default:
          Serial.println("Invalid option. Please select other option.");
          printMenu();
      }
}

// Function for 3. System Status
void systemStatusInteraction(int submenuOption){
  switch (submenuOption) {
        case 1: // Current Sensor Readings
          currentInputContext = DISPLAY_SENSOR_VALUES;
          Serial.println("\nPress any key to exit.");
          break;
        case 2: // Current Sensor Settings
          Serial.println("\nCurrent sampling rate is:");
          currentSamplingRate = EEPROM.read(samplingRateAddress);
          Serial.println(currentSamplingRate);
          Serial.println("\nMinimum Ultrasonic Alert Threshold Value is:");
          currentUltrasonicAlertThreshold = EEPROM.read(ultrasonicAlertThresholdAddress);
          Serial.println(currentUltrasonicAlertThreshold);
          Serial.println("\nMinimum LDR Alert Threshold Value is:");
          currentLDRAlertThreshold = EEPROM.read(ldrAlertThresholdAddress);
          Serial.println(currentLDRAlertThreshold);
          break;
        case 3: // Last 10 saved readings
          for (int i = index; i < index + noOfReadings; i++) {
            Serial.print("Distance: ");
            Serial.println(ultrasonicReadings[i % noOfReadings]);
            Serial.print("Analog reading = ");
            Serial.println(ldrReadings[i % noOfReadings]);
          }
          printSubmenu3();
          break;
        case 4: // Back
          printMenu();
          currentMenu = MAIN_MENU;
          break;
        default:
          Serial.println("Invalid option. Please select other option.");
          printSubmenu3();
      }
}

// Function for 4. RGB LED Control 
void rgbLedInteraction(int submenuOption){
  switch (submenuOption) {
        case 1: // Manual Color Control
          currentInputContext = RED_CHANNEL;
          Serial.println("Please insert a value between 0 and 255 for RGB LED's Red Channel:");
          break;
        case 2: // Toggle Automatic Mode
          ledToggleAutomaticMode = !ledToggleAutomaticMode;
          if (!ledToggleAutomaticMode) {
            updateRGBLed();
          }
          break;
        case 3: // Back
          printMenu();
          currentMenu = MAIN_MENU;
          break;
        default:
          Serial.println("Invalid option. Please select other option.");
          printSubmenu4();
      }
}

void updateRGBLed(){
  redChannelLEDValue = EEPROM.read(redChannelAddress);
  greenChannelLEDValue = EEPROM.read(greenChannelAddress);
  blueChannelLEDValue = EEPROM.read(blueChannelAddress);

  analogWrite(redChannelLEDPin, redChannelLEDValue);
  analogWrite(greenChannelLEDPin, greenChannelLEDValue);
  analogWrite(blueChannelLEDPin, blueChannelLEDValue);

}

// Function for gathering sensor readings, store last 10 values and
// control RGB LED in automatic mode 
void sensorsInteraction(){
  currentSamplingRate = EEPROM.read(samplingRateAddress);
  currentUltrasonicAlertThreshold = EEPROM.read(ultrasonicAlertThresholdAddress);
  currentLDRAlertThreshold = EEPROM.read(ldrAlertThresholdAddress);

  if (millis() - lastReadingTime >= currentSamplingRate) {
    lastReadingTime = millis();
    // Reading ultrasonic sensor values
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.034/2;

    // Reading photocell sensor values
    photocellValue = analogRead(photocellPin);

    if (currentInputContext == DISPLAY_SENSOR_VALUES) {
      Serial.print("Distance: ");
      Serial.println(distance);
      Serial.print("Analog reading = ");
      Serial.println(photocellValue);
    }

    ultrasonicReadings[index] = distance;
    ldrReadings[index] = photocellValue;
    index = (index + 1) % noOfReadings;
    
    bool sensorThresholdExceeded = (distance < currentUltrasonicAlertThreshold) || (photocellValue < currentLDRAlertThreshold);
    if (ledToggleAutomaticMode) {
      if (sensorThresholdExceeded) {
      digitalWrite(redChannelLEDPin, HIGH);
      digitalWrite(greenChannelLEDPin, LOW);
      digitalWrite(blueChannelLEDPin, LOW);
      }
      else {
        digitalWrite(redChannelLEDPin, LOW);
        digitalWrite(greenChannelLEDPin, HIGH);
        digitalWrite(blueChannelLEDPin, LOW);
      }
    }
    
    
  }
}
