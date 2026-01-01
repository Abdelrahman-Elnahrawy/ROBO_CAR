# ESP32 PS3 Controller Dual L298N Motor Driver

![Arduino IDE](https://img.shields.io/badge/ArduinoIDE-Compatible-blue)
![ESP32](https://img.shields.io/badge/ESP32-Bluetooth-orange)
![PS3](https://img.shields.io/badge/PS3-Controller-black)

## 📌 Overview
This project controls a **4-wheel robotic car** using an **ESP32** and a **PlayStation 3 Bluetooth controller**.
Motor driving is handled using **two L298N H-Bridge drivers**, allowing full forward, reverse, and turning control.

## 🚀 Features
- PS3 Bluetooth controller support
- 4 DC motors (tank steering)
- Dual L298N motor drivers
- Hardware PWM using ESP32 LEDC
- Adjustable turning speed (speed factor)
- Auto restart if controller disconnects

## 🧠 Control Logic
- **D-Pad Up / Down** → Forward / Reverse
- **Cross / Triangle** → Forward Left / Forward Right
- **Square / Circle** → Reverse Left / Reverse Right
- **Release buttons** → Stop motors

## 🔌 Hardware Requirements
- ESP32 Development Board
- 2 × L298N Motor Driver
- 4 × DC Motors
- PS3 Controller (Bluetooth)

## 📍 Pin Mapping
### L298N #1 (Right Side)
| Motor | GPIO |
|------|------|
| IN1  | 4 |
| IN2  | 17 |
| IN3  | 16 |
| IN4  | 5 |
| ENA  | 13 |

### L298N #2 (Left Side)
| Motor | GPIO |
|------|------|
| IN1  | 19 |
| IN2  | 18 |
| IN3  | 22 |
| IN4  | 23 |
| ENB  | 15 |

## ⚙️ PWM Configuration
- Frequency: 1000 Hz
- Resolution: 6-bit
- Channels:
  - Channel 0 → Left motors
  - Channel 1 → Right motors

## 🛠 Arduino IDE File Name
esp32_ps3_controller_dual_l298n_motor_driver.ino

markdown
Copy code

## 📜 Notes
- Uses ESP32 hardware PWM (LEDC) instead of software PWM
- Speed factor is applied for smoother turns
- Safe restart if controller disconnects

## 📄 License
Open-source project – free to modify and use