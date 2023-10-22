/*
  Homework 2 
  
  This sketch reads each value of the three potentiometers and writes a mapped value 
  to the RGB LED's pins. I'm using a separate potentiometer for controlling each color of the 
  RGB LED: Red, Green and Blue.

  The circuit:
  * 3 potentiometers attached to analog inputs A0, A1 and A2
  * a RGB LED for output:
      - Red channel attached to digital pin ~9
      - Green channel attached to digital pin ~10
      - Blue channel attached to digital pin ~11
  
  Created 21/10/2023
  By Ana-Iulia Staci

  https://github.com/iuliastaci/IntroductionToRobotics
  
*/

// Define the analog pin for the 3 potentiometers.
const int blueChannelPotentiometerPin = A0;
const int greenChannelPotentiometerPin = A1;
const int redChannelPotentiometerPin = A2;

// Define the digital pin for each color channel of the RGB LED.
const int blueChannelLedPin = 11;
const int greenChannelLedPin = 10;
const int redChannelLedPin = 9;

// Minimum and maximum values for analog readings.
const int minAnalogValue = 0;
const int maxAnalogValue = 1023;

// Minimum and maximum values for each color channel of the RGB LED.
const int minRedChannelValue = 0;
const int maxRedChannelValue = 255;
const int minBlueChannelValue = 0;
const int maxBlueChannelValue = 255;
const int minGreenChannelValue = 0;
const int maxGreenChannelValue = 255;



void setup() {
  Serial.begin(9600); // Initialize the serial communication for debugging.
  // Set the RGB LED pins as an output.
  pinMode(redChannelLedPin, OUTPUT);
  pinMode(blueChannelLedPin, OUTPUT);
  pinMode(greenChannelLedPin, OUTPUT);

}

void loop() {
  // Read the analog values from the potentiometers.
  float blueChannelPotentiometerValue = analogRead(blueChannelPotentiometerPin);
  float greenChannelPotentiometerValue = analogRead(greenChannelPotentiometerPin);
  float redChannelPotentiometerValue = analogRead(redChannelPotentiometerPin);

  //Map the analog values to LED intensity values.
  float blueChannelLedValue = map(blueChannelPotentiometerValue, minAnalogValue, maxAnalogValue, minBlueChannelValue, maxBlueChannelValue);
  float greenChannelLedValue = map(greenChannelPotentiometerValue, minAnalogValue, maxAnalogValue, minGreenChannelValue, maxGreenChannelValue);
  float redChannelLedValue = map(redChannelPotentiometerValue, minAnalogValue, maxAnalogValue, minRedChannelValue, maxRedChannelValue);

  // Update the LED colors using analogWrite.
  analogWrite(blueChannelLedPin, blueChannelLedValue);
  analogWrite(greenChannelLedPin, greenChannelLedValue);
  analogWrite(redChannelLedPin, redChannelLedValue);

}
