/*
 * Project : ESP32 PS3 Controlled Robot (Dual L298N)
 * Board   : ESP32
 * Control : PS3 Bluetooth Controller
 * Motors  : 4 DC Motors via 2x L298N
 */

/* ===================== Libraries ===================== */
#include <Ps3Controller.h>

/* ===================== Config ===================== */
#define SPEED_FACTOR   0.5

/* ===================== Motor Pins ===================== */
// Right side motors (L298N #1)
#define MOTOR1_1  4
#define MOTOR2_1  16
#define MOTOR1_2  17
#define MOTOR2_2  5

// Left side motors (L298N #2)
#define MOTOR3_1  19
#define MOTOR3_2  18
#define MOTOR4_1  22
#define MOTOR4_2  23

/* ===================== PWM ===================== */
#define MOTOR_RIGHT_EN   13
#define MOTOR_LEFT_EN    15

#define PWM_FREQ         1000
#define RIGHT_MOTORS     1
#define LEFT_MOTORS      0
#define PWM_RESOLUTION   6

const int MAX_DUTY_CYCLE = (1 << PWM_RESOLUTION) - 1;

/* ===================== Motor Control ===================== */
void set_speed_right(int duty) {
  if (duty == 1) duty = 2;
  ledcWrite(RIGHT_MOTORS, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}

void set_speed_left(int duty) {
  if (duty == 1) duty = 2;
  ledcWrite(LEFT_MOTORS, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}

void STOP() {
  digitalWrite(MOTOR1_1, HIGH);
  digitalWrite(MOTOR1_2, HIGH);
  digitalWrite(MOTOR2_1, HIGH);
  digitalWrite(MOTOR2_2, HIGH);
  digitalWrite(MOTOR3_1, HIGH);
  digitalWrite(MOTOR3_2, HIGH);
  digitalWrite(MOTOR4_1, HIGH);
  digitalWrite(MOTOR4_2, HIGH);
}

void forward(int duty) {
  set_speed_left(duty);
  set_speed_right(duty);

  digitalWrite(MOTOR1_1, HIGH); digitalWrite(MOTOR1_2, LOW);
  digitalWrite(MOTOR2_1, HIGH); digitalWrite(MOTOR2_2, LOW);
  digitalWrite(MOTOR3_1, HIGH); digitalWrite(MOTOR3_2, LOW);
  digitalWrite(MOTOR4_1, HIGH); digitalWrite(MOTOR4_2, LOW);
}

void reverse(int duty) {
  set_speed_left(duty);
  set_speed_right(duty);

  digitalWrite(MOTOR1_1, LOW);  digitalWrite(MOTOR1_2, HIGH);
  digitalWrite(MOTOR2_1, LOW);  digitalWrite(MOTOR2_2, HIGH);
  digitalWrite(MOTOR3_1, LOW);  digitalWrite(MOTOR3_2, HIGH);
  digitalWrite(MOTOR4_1, LOW);  digitalWrite(MOTOR4_2, HIGH);
}

void forward_left(int duty) {
  set_speed_left(duty * SPEED_FACTOR);
  set_speed_right(duty);
  forward(duty);
}

void forward_right(int duty) {
  set_speed_left(duty);
  set_speed_right(duty * SPEED_FACTOR);
  forward(duty);
}

void reverse_left(int duty) {
  set_speed_left(duty * SPEED_FACTOR);
  set_speed_right(duty);
  reverse(duty);
}

void reverse_right(int duty) {
  set_speed_left(duty);
  set_speed_right(duty * SPEED_FACTOR);
  reverse(duty);
}

void left(int duty) {
  set_speed_left(duty);
  set_speed_right(duty);

  digitalWrite(MOTOR1_1, HIGH); digitalWrite(MOTOR1_2, LOW);
  digitalWrite(MOTOR2_1, HIGH); digitalWrite(MOTOR2_2, LOW);
  digitalWrite(MOTOR3_1, LOW);  digitalWrite(MOTOR3_2, HIGH);
  digitalWrite(MOTOR4_1, LOW);  digitalWrite(MOTOR4_2, HIGH);
}

void right(int duty) {
  set_speed_left(duty);
  set_speed_right(duty);

  digitalWrite(MOTOR1_1, LOW);  digitalWrite(MOTOR1_2, HIGH);
  digitalWrite(MOTOR2_1, LOW);  digitalWrite(MOTOR2_2, HIGH);
  digitalWrite(MOTOR3_1, HIGH); digitalWrite(MOTOR3_2, LOW);
  digitalWrite(MOTOR4_1, HIGH); digitalWrite(MOTOR4_2, LOW);
}

/* ===================== PS3 Events ===================== */
void notify() {
  if (Ps3.data.analog.button.up == 255)       forward(9);
  if (Ps3.data.analog.button.down == 255)     reverse(9);
  if (Ps3.data.analog.button.cross == 255)    forward_left(9);
  if (Ps3.data.analog.button.triangle == 255) forward_right(9);
  if (Ps3.data.analog.button.square == 255)   reverse_left(9);
  if (Ps3.data.analog.button.circle == 255)   reverse_right(9);

  if (Ps3.event.button_up.up || Ps3.event.button_up.down) STOP();
}

void onConnect() {
  Serial.println("PS3 Controller Connected");
}

/* ===================== Setup ===================== */
void setup() {
  Serial.begin(115200);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("00:1a:7d:da:71:10");

  ledcSetup(RIGHT_MOTORS, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_RIGHT_EN, RIGHT_MOTORS);

  ledcSetup(LEFT_MOTORS, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_LEFT_EN, LEFT_MOTORS);

  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(MOTOR3_1, OUTPUT);
  pinMode(MOTOR3_2, OUTPUT);
  pinMode(MOTOR4_1, OUTPUT);
  pinMode(MOTOR4_2, OUTPUT);

  STOP();
}

/* ===================== Loop ===================== */
void loop() {
  if (!Ps3.isConnected()) {
    static unsigned long t = millis();
    if (millis() - t > 10000) ESP.restart();
  }
}
