# Introduction to Robotics (2023-2024)
Welcome to "Introduction to Robotics" repository! 
This repository contains laboratory homework assignments for the third-year course taken at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework assignment comes with _requirements_, _implementation details_, _code_ and _image files_.

<details>
<summary><h2>Homework 2 - An RGB LED controlled with 3 potentiometers</h2></summary>
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
</details>

<details>
<summary><h2>Homework 3 - Elevator simulator wannabe</h2></summary>
In simple terms, this task is about creating a mini model of a 3-floor elevator using an Arduino. We will have lights (LEDs) that show which floor the elevator is on, buttons on each floor to call the elevator, and a buzzer to make sounds when the elevator arrives or moves. The elevator will have some rules: it will wait a bit when you press a button, and if it's already on the right floor, nothing happens. We also make sure the buttons work properly by preventing accidental presses.

The system will feature the following components:
1. _LED Indicators_: Three LEDs will be employed to represent the three elevator floors. The LED corresponding to the current floor will be illuminated. Additionally, another LED will indicate the elevator's operational state by blinking while in motion and remaining steady when stationary. I've used red LEDs for elevator floors and one green LED to indicate the operational state of the elevator.
2. _Buttons_: Three buttons, each representing a call button from one of the three floors, will be implemented. When a button is pressed, the elevator will simulate movement toward that floor after a brief delay of 2-3 seconds.
3. _Buzzer_: The buzzer will produce brief sounds during specific events - a sound when the elevator arrives at the desired floors and two distinct sounds for elevator door closing and elevator movement. For this component, I've used a 100 Ohm resistor.
4. _Resistors and wires_: These components are used to connect the buttons, the LEDs and the buzzer to the Arduino. Resistor values are selected based on the LED's forward voltage and current requirements. For this setup, I've used three 220-Ohm resistors, one for each red LED, and two 100-Ohm resistors, one for the green LED and one for the buzzer.


The elevator's behavior will be governed by the following rules:
* If the elevator is already at the desired floor, pressing the corresponding button will have no effect.
* After a button press, the elevator will "wait for the doors to close" and then "move" to the corresponding floor.

To ensure the reliability of the button inputs, a debounce mechanism will be implemented. This will prevent unintended repeated button presses caused by mechanical switch bouncing.

Here is an image with the setup:
![homework 3](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/b5849aee-960c-460a-9f5e-3877c1c51083)


If you want to watch a demo of the homework, click [here](https://youtube.com/shorts/EWf-sx_lL34 ).
</details>

<details>
<summary><h2>Homework 4 - 7 segment display drawing</h2></summary>
In this homework, I'm using a joystick to move a segment on a display screen. I can't move through obstacles and I start on a specific point (decimal point - down right). The current position blinks and I can use the joystick to move to nearby positions (up, down, left, right). Pressing the button quickly turns the segment on or off and holding the button resets the entire display.

The setup includes the following electronic components:
1. _7-segment display_: It is a small electronic screen that uses 7 separate LED segments to show numbers and some letters.
2. _Joystick_: It is a handheld input device commonly used for controlling the movement or position of object in a digital environment. It typically consists of a stick-like handle that can be pushed or tilted in various directions.
3. _Resistors and wires_: These components are used to connect the 7-segment display and the joystick to the Arduino. Resistor values are selected based on the 7-segment display LED's forward voltage and current requirements. For this setup, I've used eight 330-Ohm resistors, one for segment LED.

Here is a picture of the setup:
![IMG_0238](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/ded80fa2-2d46-40af-9b4c-2f87865abd40)

If you want to watch a demo of the homework, click [here](https://youtube.com/shorts/pzlum-Pjn4I).
</details>

