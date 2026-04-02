# Bluetooth Controlled Robot with Robotic Arm

## Overview
This project is a mobile robot controlled via Bluetooth using a mobile application.  
It includes movement control (forward, backward, left, right) and a robotic arm with a gripper controlled by servos.

## Components
- Arduino board
- L298N motor driver
- HC-05 / HC-06 Bluetooth module
- DC motors
- Servo motors (arm + gripper)
- Mobile phone (Bluetooth controller app)
- Battery pack
- Chassis

## Features
- Wireless robot control via Bluetooth
- Directional movement (forward, backward, left, right, stop)
- Robotic arm movement (up/down with smooth control)
- Gripper control (open/close)
- Simultaneous motor and servo control

## Control Commands
- F → Move Forward  
- B → Move Backward  
- L → Turn Left  
- R → Turn Right  
- S → Stop  
- U → Move arm up  
- D → Move arm down  
- X → Center arm  
- O → Open gripper  
- C → Close gripper  

## Working Principle
- The robot receives commands via Bluetooth serial communication.
- Each command triggers a specific motor or servo action.
- The arm uses smooth incremental movement for better control.
- Motor driver (L298N) controls the DC motors for navigation.

## What I Learned
- Wireless communication using Bluetooth modules
- Serial communication in Arduino
- Servo motor control
- Integration of multiple subsystems (motors + servos + communication)
- Real-time command processing

## Future Improvements
- Add mobile app UI customization
- Integrate obstacle detection sensors
- Combine with autonomous modes
- Improve arm precision with feedback control
