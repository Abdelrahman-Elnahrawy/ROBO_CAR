# ESP32 Line Following & PS3 Controlled Robot

![ESP32](https://img.shields.io/badge/ESP32-DevBoard-black?style=for-the-badge&logo=espressif)
![PS3 Controller](https://img.shields.io/badge/Controller-PS3-blue)
![NeoPixel](https://img.shields.io/badge/LED-NeoPixel-purple)
![Line Follower](https://img.shields.io/badge/Feature-Line_Follower-orange)
![L298N Motor Driver](https://img.shields.io/badge/Driver-L298N-red)
![PWM](https://img.shields.io/badge/Feature-PWM-yellow)
![IR Sensors](https://img.shields.io/badge/Feature-IR_Sensors-lightgrey)
![Language: C/C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Author](https://img.shields.io/badge/Author-Abdelrahman--Elnahrawy-orange?style=for-the-badge)

---

## Project Overview

This project implements a **dual-mode robot** using an ESP32:  

1. **Line Following Mode** – Automatically follows a line using IR sensors.  
2. **PS3 Controller Mode** – Controlled manually using a PS3 controller via Bluetooth.  

The robot integrates **NeoPixel LEDs** for visual feedback, **L298N motor driver** for motor control, and uses **PWM** for speed adjustments.

---

## Block Diagram

The system structure can be seen in the following diagram:

![Block Diagram](block_diagram.png)

> **Note:** IR sensors are connected as inputs to the ESP32, while the motors are driven through the L298N motor driver. NeoPixel LEDs are controlled directly from the ESP32.

---

## Hardware

- **ESP32 Dev Board**  
- **L298N Motor Driver**  
- **DC Motors** (4-wheel drive)  
- **IR Sensors** (Front & Back, for line detection)  
- **NeoPixel Strip** (30 LEDs)  
- **PS3 Controller** (Bluetooth)  
- **Power Supply / Battery**  

---

## Software

- **Arduino IDE** (ESP32 board support)  
- **Libraries Used:**
  - `Ps3Controller` – for PS3 Bluetooth connection
  - `Adafruit_NeoPixel` – for LED control

---

## Features

- Dual mode operation: Line following / Manual PS3 control  
- Adjustable speed with PS3 buttons  
- LED feedback for sensors and speed  
- Forward, backward, left, right, diagonal movement  
- Non-blocking sensor reading loop

---

## Wiring / Pinout Overview

| Component           | ESP32 Pin |
|--------------------|-----------|
| IR Front Left       | 34        |
| IR Front Mid        | 35        |
| IR Front Right      | 32        |
| IR Back Left        | 25        |
| IR Back Mid         | 26        |
| IR Back Right       | 27        |
| NeoPixel Data       | 14        |
| L298N ENA           | 15        |
| L298N ENB           | 13        |
| Motor Inputs        | 4, 5, 16, 17, 18, 19, 22, 23 |

---

## Usage

1. Upload the code to the ESP32 using Arduino IDE.  
2. Power the robot and wait for the NeoPixel LED initialization.  
3. Connect the PS3 controller via Bluetooth (`Ps3.begin(MAC_ADDRESS)`).  
4. Press the PS button to toggle between **manual control** and **line follower mode**.  
5. Use analog sticks and buttons for directional control and speed adjustment.

---

## License

This project is licensed under the **MIT License**.  

---

## Author

**Abdelrahman Elnahrawy**
