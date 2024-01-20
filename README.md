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
3. _Resistors and wires_: These components are used to connect the potentiometers and the RGB LED to the Arduino. Resistor values are selected based on the LED's forward voltage and current requirements. For this particular setup, I've used a 220-Ohm resistor for the Red channel and two 100-Ohm resistors for the Blue and Green channels.

The following steps are involved in achieving the desired control:
1. Connect the potentiometers to the Arduino's analog pins (A0, A1, A2 pins for this setup).
2. Read the analog values from the potentiometers. These values correspond to the positions of the potentiometer knobs.
3. Map the analog readings to appropriate output values for each color channel (Red, Green and Blue). This mapping ensures that the LED's colors change according to the potentiometer positions.
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
2. _Joystick_: It is a handheld input device commonly used for controlling the movement or position of the object in a digital environment. It typically consists of a stick-like handle that can be pushed or tilted in various directions.
3. _Resistors and wires_: These components are used to connect the 7-segment display and the joystick to the Arduino. Resistor values are selected based on the 7-segment display LED's forward voltage and current requirements. For this setup, I've used eight 330-Ohm resistors, one for segment LED.

Here is a picture of the setup:
![IMG_0238](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/ded80fa2-2d46-40af-9b4c-2f87865abd40)

If you want to watch a demo of the homework, click [here](https://youtube.com/shorts/pzlum-Pjn4I).
</details>

<details>
<summary><h2>Homework 5 - Stopwatch Timer</h2></summary>
For this project, I'm using a 4-digit 7-segment display and 3 buttons in order to implement a stopwatch timer with 3 functionalities: start/pause, reset and save lap.
  
It should use the following workflow:  

1. Start the timer by pressing the Start button when the display shows "000.0".
2. Save lap times in memory (up to 4 laps) by pressing the lap button during the timer. The 5th press overrides the 1st. The reset button has no effect during the timer, and the pause button stops the timer.
3. In Pause Mode, the lap flag button is disabled. Pressing the reset button sets the timer to "000.0".
4. After reset, use flag buttons to cycle through lap times. Pressing continuously cycles through laps, and resetting while in this state clears flags and resets the timer to "000.0".

The setup includes the following electronic components:
1. _4-digit 7-segment display_: It is a small electronic screen that uses 7 separate LED segments for each of the 4 digits to show numbers and some letters.
2. _Buttons_: Three buttons, one to start or pause the counting, one for resetting the counter or the saved laps and one for saving the laps and cycle through them.
3. _Resistors and wires_: These components are used to connect the 7-segment display and the buttons to the Arduino. Resistor values are selected based on the 7-segment display LED's forward voltage and current requirements. For this setup, I've used eight 330-Ohm resistors, one for each segment LED.
4. _Shift register_: It is a digital circuit that can shift its stored data through a series of flip-flops, typically in a linear fashion.

Here is a picture of the setup:
![IMG_0507](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/7307c809-0819-4284-9de1-042c4bd0fffc)

If you want to watch a homework demo, click [here](https://youtube.com/shorts/PFdx8g3E60M).
</details>

<details>
<summary><h2>Homework 6 - Pseudo - Smart Environment Monitor and Logger</h2></summary>

This project aims to create a "Smart Environment Monitor and Logger" using Arduino, combining a diverse array of sensors to collect crucial environmental data. The system efficiently logs this information into EEPROM for later retrieval. A key feature of the project is the incorporation of an RGB LED, offering visual feedback based on the environmental conditions. Additionally, users can interact with the system through a Serial Menu, providing a seamless interface for navigation and customization. The project underscores the integration of sensor readings, effective memory management, serial communication, and the overarching goal of constructing a user-friendly menu system.

**Menu structure**
1. _Sensor Settings_  
 &nbsp;1.1. Sensors Sampling Interval <br>
 &nbsp;1.2. Ultrasonic Alert Threshold <br>
 &nbsp;1.3. LDR Alert Threshold <br>
 &nbsp;1.4. Back <br>
2. _Reset Logger Data_ <br>
 &nbsp;2.1. Yes <br>
 &nbsp;2.2. No <br>
3. _System Status_ <br>
  &nbsp;3.1. Current Sensor Readings <br>
  &nbsp;3.2. Current Sensor Settings <br>
  &nbsp;3.3. Display Logged Data <br>
  &nbsp;3.4. Back <br>
4. _RGB LED Control_ <br>
 &nbsp;4.1. Manual Color Control <br>
 &nbsp;4.2. LED Toggle Automatic ON/OFF <br>
 &nbsp;4.3. Back <br>

The setup involves the use of specific electronic components, including:
1. _RGB LED_: The RGB LED used has 4 pins, one for each color channel (Red, Green, Blue) and one common cathode pin.
2. _Ultrasonic Sensor_: The HC-SR04 is a widely-used ultrasonic distance sensor that employs sound waves to measure distances. It consists of an ultrasonic transmitter that emits pulses and a receiver that detects the reflected signals. By calculating the time taken for the ultrasonic waves to travel to an object and back, the sensor provides accurate distance measurements, making it popular for applications such as robotics, obstacle avoidance, and distance sensing in various projects.
3. _LDR_: Light Dependent Resistor is a type of photoresistor that exhibits a change in resistance based on the intensity of light falling on it. As light levels fluctuate, the LDR's resistance varies in response. This characteristic makes it a valuable component in light-sensitive circuits. Commonly used in applications like automatic streetlights, camera exposure control, and darkness-activated switches, the LDR enables devices to adapt to ambient light conditions by providing a variable resistance corresponding to the surrounding illumination.
4. _Resistors and wires_: These components are used to connect the potentiometers and the RGB LED to the Arduino. Resistor values are selected based on the LED's forward voltage and current requirements. For this particular setup, I've used a 220-Ohm resistor for the Red channel and two 100-Ohm resistors for Blue and Green channels. And for LDR I have used a 10k-Ohm resistor.

Here is a picture of the setup:
![IMG_0537](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/8c3a3882-a7ab-4328-9eda-7d1be00fc589)

If you want to watch a demo of the project, click [here](https://youtube.com/shorts/qPYjCvedyBo)

</details>

<details>
  <summary><h2>Homework 7 - Matrix Game Checkpoint (MindMatrix Challenge)</h2></summary>
  This homework is a checkpoint for a bigger project, a game on an 8x8 LED Matrix controlled with a joystick and paired with an LCD to display relevant information.
  The game's name is MindMatrix Challenge because guess what? ... It's a memory game where you can test your brain and attention. 
  It consists of 3 levels with progressing difficulty. At each level, some LEDs are turned on in order to form a random pattern and it is displayed for a couple of seconds. Than, there's a LED blinking ... that's the player. It can be moved on the matrix using the joystick. When clicking the switch button, the LED at pleyer's position is turned on or off, depending on its previous state. 
  To win the game, you should represent the correct pattern at each level. If an LED that is not in the pattern is turned on, the game is over. 

  The setup involves the use of specific electronic components, including:
1. _8x8 LED Matrix_: The 8x8 LED Matrix for Arduino is a dynamic display module that brings a vibrant visual element to Arduino projects. Consisting of 64 individually addressable LEDs arranged in an 8x8 grid, this matrix allows for the creation of custom patterns, scrolling text, and dynamic graphics. With seamless integration into Arduino projects, it provides a simple and effective way to add eye-catching visual feedback. Ideal for projects requiring compact and programmable displays, the 8x8 LED Matrix enhances the creative possibilities of Arduino-based applications.
2. _LCD_: The Hitachi LCD for Arduino is a compact and versatile liquid crystal display module designed to seamlessly integrate with Arduino microcontrollers. Featuring a user-friendly interface and clear, crisp display capabilities, it allows developers to easily incorporate visual feedback into their Arduino projects. With its compatibility and reliability, the Hitachi LCD enhances the overall user experience, making it an ideal choice for projects requiring a compact and efficient display solution.
3. _Joystick_: It is a handheld input device commonly used for controlling the movement or position of the object in a digital environment. It typically consists of a stick-like handle that can be pushed or tilted in various directions.
4. _MAX7219 Driver_: The MAX7219 Driver for Arduino Matrix is a powerful display driver module that simplifies the control of LED matrix displays. Specifically designed for use with Arduino, this driver seamlessly interfaces with 8x8 LED matrices, providing an efficient means to control individual LEDs and create captivating visual effects. With its versatile features and ease of integration, the MAX7219 Driver enhances the capabilities of Arduino projects, enabling users to easily implement scrolling text, animations, and customized patterns on LED matrix displays with minimal programming effort. It's an excellent choice for those seeking a compact and user-friendly solution for dynamic visual displays in Arduino-based applications.
5. _Resistors and wires_: These components are used to connect the matrix, LCD, joystick and the driver to the Arduino.

Here is a picture of the setup:
![IMG_0620](https://github.com/iuliastaci/IntroductionToRobotics/assets/103101598/2b7f41f5-dc9c-4325-9bab-8f06863ca49c)

If you want to watch a demo of the game, click [here](https://youtube.com/shorts/noldko7yy7M).
</details>

