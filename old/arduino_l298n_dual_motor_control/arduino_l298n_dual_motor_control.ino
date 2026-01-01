/*
 * Project: Dual DC Motor Sequential Control
 * Author: Abdelrahman Elnahrawy
 * Description: Controls two DC motors (A and B) using an H-Bridge driver.
 * The sequence performs clockwise rotation, active braking, and 
 * counter-clockwise rotation for both motors.
 */

// --- Motor A Pins ---
const int IN1 = 4;
const int IN2 = 5;

// --- Motor B Pins ---
const int IN3 = 6;
const int IN4 = 7;

/**
 * Standard initialization of GPIO pins as outputs.
 */
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ensure motors are stopped on startup
  stopMotors();
}

/**
 * Main sequence: 
 * 1. Both Forward -> 2. Both Brake -> 3. Both Backward -> 4. Both Brake
 */
void loop() {
  // --- Rotate Motor A Clockwise ---
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(2000);

  // Brake Motor A (High-High state creates back-EMF braking)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(500);

  // --- Rotate Motor B Clockwise ---
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);

  // Brake Motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(500);

  // --- Rotate Motor A Counter-Clockwise ---
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(2000);

  // Brake Motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(500);

  // --- Rotate Motor B Counter-Clockwise ---
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(2000);

  // Brake Motor B
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(500);
}

/**
 * Utility function to set all pins LOW (Coasting stop).
 */
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}