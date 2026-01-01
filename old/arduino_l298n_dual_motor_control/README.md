# 🏎️ arduino_l298n_dual_motor_control

![Arduino](https://img.shields.io/badge/Arduino-Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Driver](https://img.shields.io/badge/Driver-L298N_H--Bridge-red?style=for-the-badge)
![Category](https://img.shields.io/badge/Category-Robotics-blue?style=for-the-badge)

## 📖 Overview
The **arduino_l298n_dual_motor_control** project is a foundational robotics script designed to drive two DC motors. It utilizes an H-Bridge motor driver (like the L298N) to manage the high current required by motors while using low-current logic signals from the Arduino.

This code demonstrates **Directional Control** and **Active Braking**. By setting both input pins of a motor to `HIGH` simultaneously, the motor "locks," providing a much faster stop than simply cutting the power.

## 🚀 Key Features
* **🔄 Full Bi-directional Support:** Independent control for both Motor A and Motor B in clockwise and counter-clockwise directions.
* **🛑 Active Braking Logic:** Implements a "High-High" state to short the motor windings, creating an electromagnetic brake for precise movement.
* **🛠️ Modular GPIO Mapping:** Uses pins 4 through 7, leaving PWM pins (3, 5, 6, 9, 10, 11) available for speed control upgrades.

## 🛠️ Hardware Setup
### Wiring Diagram


| Component Pin | Arduino Pin | Function |
| :--- | :--- | :--- |
| **IN1** | D4 | Motor A Direction 1 |
| **IN2** | D5 | Motor A Direction 2 |
| **IN3** | D6 | Motor B Direction 1 |
| **IN4** | D7 | Motor B Direction 2 |
| **GND** | GND | Common Ground (Essential!) |

### H-Bridge Truth Table (Logic)
| IN1 | IN2 | Motor Result |
| :--- | :--- | :--- |
| **HIGH** | **LOW** | Forward / Clockwise |
| **LOW** | **HIGH** | Backward / Counter-Clockwise |
| **HIGH** | **HIGH** | **BRAKE** (Hard Stop) |
| **LOW** | **LOW** | **OFF** (Coasting) |



## ⚠️ Important Safety Tips
1. **External Power:** Never power DC motors directly from the Arduino 5V pin. Always use an external battery pack (7V-12V) connected to the motor driver's VCC.
2. **Common Ground:** Ensure the negative terminal of your battery pack is connected to the Arduino's `GND` pin. Without this, the logic signals will not work.
3. **Current Limits:** Ensure your motor driver (e.g., L298N) is rated for the stall current of your specific DC motors.

## 👤 Author
* **Abdelrahman Elnahrawy**
* GitHub: [@Abdelrahman-Elnahrawy](https://github.com/Abdelrahman-Elnahrawy)

## ⚖️ License
This project is licensed under the **MIT License**.