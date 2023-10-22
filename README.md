# Introduction to Robotics (2023-2024)
Welcome to "Introduction to Robotics" repository! 
This repository contains laboratory homework assignments for the third-year course taken at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework assignment comes with _requirements_, _implementation details_, _code_ and _image files_.

## Homework 2 - An RGB LED controlled with 3 potentiometers
For this homework, I'm using an Arduino to control a special color-changing LED (RGB LED). But the cool part is that I'm not just picking colors from a list - I'm using three knobs called potentiometers to make the LED change colors in a way I like. So, each knob lets me pick a different color for the LED. It's like having three magic color dials to create any color I like! 🪄

Technically speaking, the objective of this homework is to control each channel (Red, Green, Blue) of an RGB LED using individual potentiometers in conjunction with an Arduino microcontroller.

The setup involves the use of specific electronic components, including:
1. _RGB LED_: The RGB LED used has 4 pins, one for each color channel (Red, Green, Blue) and one common cathode pin.
2. _Potentiometers_: Three potentiometers are used, one for each color channel. They are variable resistors that provide analog voltage readings based on their position.
3. _Resistors and wires_: These components are used to connect the potentiometers and the RGB LED to the Arduino. Resistor values are selected based on the LED's forward voltage and current requirements. For this particular setup, I've used a 220-Ohm resistor for the Red channel and two 100-Ohm resistors for Blue and Green channels.

The following steps are involved in achieving the desired control:
1. Connect the potentiometers to the Arduino's analog pins (A0, A1, A2 pins for this setup).
2. Read the analog values from the potentiometers. These values corespond to the positions of the potentiometer knobs.
3. Map the analog readings to appropiate output values for each color channel (Red, Green and Blue). This mapping ensures that the LED's colors change according to the potentiometer positions.
4. Send the mapped values to the pins of the RGB LED, controlling the intensity of each color channel (for this setup: Red - pin 9, Green - pin 10, Blue - pin 11).

Here is an image with the setup:
![IMG_0057](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/2c70afec-1e08-4347-a397-a6044bdbc2fc)

If you want to watch a demo of the homework, click [here](https://youtube.com/shorts/uMUZUHX3868?si=dJovIazbBCfvslvJ).
