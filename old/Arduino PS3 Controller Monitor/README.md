# 🎮 Arduino PS3 Controller Monitor

![Platform: Arduino](https://img.shields.io/badge/Platform-Arduino-orange) ![Language: C/C++](https://img.shields.io/badge/Language-C%2B%2B-blue) ![Status: Demo](https://img.shields.io/badge/Status-Demo-brightgreen)

A small utility project to read and monitor input from a PlayStation 3 (PS3) controller using an Arduino. Great for testing controller buttons, axes, and rumble feedback when building custom game interfaces, robotics controllers, or interactive installations.

Emojis and colored badges included for clarity and friendliness 🎉🕹️

---

## Project goal
- Connect a PS3 controller to an Arduino (via wired adapter or Bluetooth depending on hardware).
- Read button presses, analog sticks, and other inputs.
- Display controller state via serial monitor (or optional display).
- Optionally demonstrate rumble (vibration) or lightbar control if supported.

---

## Supported hardware (typical)
- Arduino Uno / Nano / Mega (or an ESP32 for Bluetooth-enabled variants)
- USB Host Shield (for wired PS3 controller) OR Bluetooth module + PS3 pairing support (more advanced)
- PS3 controller (DualShock 3)
- USB cable (for wired connection) or Bluetooth pairing setup for wireless

Notes:
- For most Arduino boards the simplest approach is to use a USB Host Shield + the USB Host library to talk to the PS3 controller.
- Some projects use an ESP32 and Bluetooth libraries to connect wirelessly — that requires additional code and pairing steps.

---

## Features
- Serial monitor output of button presses and analog stick values
- Friendly human-readable output and JSON-like debug output for easy parsing
- Example code shows how to read D-pad, face buttons, shoulder buttons, analog sticks, and triggers
- Optional rumble/LED control examples (if controller & library support it)

---

## Wiring (wired, using USB Host Shield)
- Mount the USB Host Shield on the Arduino (Uno/Mega compatible)
- Connect PS3 controller to the USB port on the shield using a USB cable
- Power the Arduino with a stable 5V supply (USB Host Shield + controller power may draw extra current)

For wireless/Bluetooth approaches:
- Use an ESP32 with DualShock 3 compatible Bluetooth stack OR use a PS3-compatible Bluetooth dongle and pairing code (advanced — see library docs).

---

## Libraries & dependencies
- USB Host Library 2.0 (for wired USB Host Shield)  
  Example: [felis/USB_Host_Shield_2.0](https://github.com/felis/USB_Host_Shield_2.0)
- If using ESP32 wireless approach, use an appropriate Bluetooth/PS3 library specific to the board.

Install libraries via Arduino Library Manager or clone them into your Arduino/libraries folder.

---

## Example sketch (serial monitor debug)
This example assumes a USB Host Shield setup and the popular USB Host Library. It's a compact debug/monitor sketch — consult the library docs for exact API and initialization.

```cpp
// Pseudocode / illustrative example — tune for your chosen USB Host library
#include <PS3BT.h>           // depends on the USB Host library variant
#include <usbhub.h>

USB Usb;
USBHub Hub(&Usb);
BTD Btd(&Usb);              // Bluetooth dongle object (if using)
PS3BT PS3(&Btd);            // or PS3BT PS3(&Usb) for wired

void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield init failed");
    while (1); // halt
  }
  Serial.println("PS3 monitor starting...");
}

void loop() {
  Usb.Task();

  if (PS3.connected()) {
    // Example: print face buttons and analog sticks
    Serial.print("Buttons: ");
    if (PS3.getButtonPress(PS)) Serial.print("PS ");
    if (PS3.getButtonPress(UP)) Serial.print("UP ");
    // ... add checks for other buttons

    int lx = PS3.getAnalogHat(LeftHatX);
    int ly = PS3.getAnalogHat(LeftHatY);
    int rx = PS3.getAnalogHat(RightHatX);
    int ry = PS3.getAnalogHat(RightHatY);
    Serial.print(" | LX:"); Serial.print(lx);
    Serial.print(" LY:"); Serial.print(ly);
    Serial.print(" RX:"); Serial.print(rx);
    Serial.print(" RY:"); Serial.println(ry);
  } else {
    Serial.println("Controller not connected");
  }

  delay(120);
}
```

Notes:
- Replace calls with the exact API of the PS3/USB library you install.
- Many USB libraries provide example sketches — start from those and adapt to show the values you need.

---

## Usage
1. Install required library (USB Host Library 2.0 or board-specific PS3 library).
2. Upload the example sketch to your Arduino.
3. Open the Serial Monitor (115200 baud) to view real-time controller state.
4. Press buttons / move sticks and observe the output.

---

## Troubleshooting
- Controller won't connect: ensure proper power, USB cable, and that the USB Host Shield is seated correctly.
- Serial shows garbage: verify baud rate and serial settings.
- Wireless pairing issues: consult the library docs for pairing steps and required Bluetooth adapters.

---

## Improvements and ideas
- Add a web or LCD dashboard to visualize controller inputs.
- Map controller inputs to robot motors or servo actions for rapid prototyping.
- Record input sequences for playback or testing.
- Add calibration and deadzone configuration for analog sticks.

---

## License & Author
- Author: Abdelrahman Elnahrawy  
- Suggested license: MIT

---
