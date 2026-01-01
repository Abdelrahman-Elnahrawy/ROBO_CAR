/*
 * Project: ESP32 PS3 Smart Robot
 * Author: Abdelrahman Elnahrawy
 * Features:
 * - PS3 Bluetooth Controller
 * - 4 DC Motors via 2x L298N with PWM speed control
 * - NeoPixel LED feedback (speed & status)
 * - IR line-following
 * - Fail-safe restart on PS3 disconnect
 * - Adjustable speed (1-9) via controller buttons
 */

#include <Ps3Controller.h>
#include <Adafruit_NeoPixel.h>

// ===================== CONFIG =====================
#define SPEED_DEFAULT     5       // Default speed
#define SPEED_FACTOR      0.5     // For turning adjustments
#define PWM_FREQ          1000
#define PWM_RESOLUTION    6
const int MAX_DUTY_CYCLE = (1 << PWM_RESOLUTION) - 1;

// ===================== MOTOR PINS =====================
// Right side motors (L298N #1)
#define MOTOR1_1  4
#define MOTOR1_2  17
#define MOTOR2_1  16
#define MOTOR2_2  5
#define MOTOR_RIGHT_EN   13

// Left side motors (L298N #2)
#define MOTOR3_1  19
#define MOTOR3_2  18
#define MOTOR4_1  22
#define MOTOR4_2  23
#define MOTOR_LEFT_EN    15

// ===================== IR SENSOR PINS =====================
#define IR_FRONT_LEFT  34
#define IR_FRONT_MID   35
#define IR_FRONT_RIGHT 32
#define IR_BACK_LEFT   25
#define IR_BACK_MID    26
#define IR_BACK_RIGHT  27

// IR thresholds
#define IR_FRONT_LEFT_T   3030
#define IR_FRONT_MID_T    2260
#define IR_FRONT_RIGHT_T  2150
#define IR_BACK_LEFT_T    950
#define IR_BACK_MID_T     2830
#define IR_BACK_RIGHT_T   2350

// ===================== NeoPixel =====================
#define NEOPIXEL_PIN 14
#define NEOPIXEL_COUNT 30
#define BRIGHTNESS 50
Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ===================== VARIABLES =====================
int SPEED = SPEED_DEFAULT;
bool Line_follower = false;

// IR sensor readings
int IR_FL, IR_FM, IR_FR, IR_BL, IR_BM, IR_BR;

// ===================== MOTOR FUNCTIONS =====================
void set_speed_right(int duty){
  if(duty == 1) duty = 2;
  ledcWrite(1, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}

void set_speed_left(int duty){
  if(duty == 1) duty = 2;
  ledcWrite(0, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}

void STOP(){
  digitalWrite(MOTOR1_1,HIGH); digitalWrite(MOTOR1_2,HIGH);
  digitalWrite(MOTOR2_1,HIGH); digitalWrite(MOTOR2_2,HIGH);
  digitalWrite(MOTOR3_1,HIGH); digitalWrite(MOTOR3_2,HIGH);
  digitalWrite(MOTOR4_1,HIGH); digitalWrite(MOTOR4_2,HIGH);
}

void forward(int duty){
  set_speed_left(duty); set_speed_right(duty);
  digitalWrite(MOTOR1_1,HIGH); digitalWrite(MOTOR1_2,LOW);
  digitalWrite(MOTOR2_1,HIGH); digitalWrite(MOTOR2_2,LOW);
  digitalWrite(MOTOR3_1,HIGH); digitalWrite(MOTOR3_2,LOW);
  digitalWrite(MOTOR4_1,HIGH); digitalWrite(MOTOR4_2,LOW);
}

void reverse(int duty){
  set_speed_left(duty); set_speed_right(duty);
  digitalWrite(MOTOR1_1,LOW);  digitalWrite(MOTOR1_2,HIGH);
  digitalWrite(MOTOR2_1,LOW);  digitalWrite(MOTOR2_2,HIGH);
  digitalWrite(MOTOR3_1,LOW);  digitalWrite(MOTOR3_2,HIGH);
  digitalWrite(MOTOR4_1,LOW);  digitalWrite(MOTOR4_2,HIGH);
}

void forward_left(int duty){
  set_speed_left(duty*SPEED_FACTOR); set_speed_right(duty);
  forward(duty);
}

void forward_right(int duty){
  set_speed_left(duty); set_speed_right(duty*SPEED_FACTOR);
  forward(duty);
}

void reverse_left(int duty){
  set_speed_left(duty*SPEED_FACTOR); set_speed_right(duty);
  reverse(duty);
}

void reverse_right(int duty){
  set_speed_left(duty); set_speed_right(duty*SPEED_FACTOR);
  reverse(duty);
}

void left(int duty){
  set_speed_left(duty); set_speed_right(duty);
  digitalWrite(MOTOR1_1,HIGH); digitalWrite(MOTOR1_2,LOW);
  digitalWrite(MOTOR2_1,HIGH); digitalWrite(MOTOR2_2,LOW);
  digitalWrite(MOTOR3_1,LOW);  digitalWrite(MOTOR3_2,HIGH);
  digitalWrite(MOTOR4_1,LOW);  digitalWrite(MOTOR4_2,HIGH);
}

void right(int duty){
  set_speed_left(duty); set_speed_right(duty);
  digitalWrite(MOTOR1_1,LOW);  digitalWrite(MOTOR1_2,HIGH);
  digitalWrite(MOTOR2_1,LOW);  digitalWrite(MOTOR2_2,HIGH);
  digitalWrite(MOTOR3_1,HIGH); digitalWrite(MOTOR3_2,LOW);
  digitalWrite(MOTOR4_1,HIGH); digitalWrite(MOTOR4_2,LOW);
}

// ===================== NEOPIXEL FUNCTIONS =====================
void show_speed(){
  strip.clear();
  int s = ((SPEED*4)/9)+1;
  for(int i=0;i<5;i++) strip.setPixelColor(i+8,strip.Color(247,0,255));
  for(int i=0;i<s;i++) strip.setPixelColor(i+8,strip.Color(64,224,208));
  strip.show();
}

void colorWipe(uint32_t c, uint8_t wait){
  for(uint16_t i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,c);
    strip.show();
    delay(wait);
  }
}

// ===================== PS3 CONTROLLER CALLBACK =====================
void notify(){
  // Movement buttons
  if(Ps3.data.analog.button.up==255) forward(SPEED);
  if(Ps3.data.analog.button.down==255) reverse(SPEED);
  if(Ps3.data.analog.button.cross==255) forward_left(SPEED);
  if(Ps3.data.analog.button.triangle==255) forward_right(SPEED);
  if(Ps3.data.analog.button.square==255) reverse_left(SPEED);
  if(Ps3.data.analog.button.circle==255) reverse_right(SPEED);
  
  // Stop buttons
  if(Ps3.event.button_up.up || Ps3.event.button_up.down) STOP();
  
  // Speed adjustment
  if(Ps3.event.button_up.select){ SPEED++; if(SPEED>9) SPEED=9; show_speed();}
  if(Ps3.event.button_up.start){ SPEED--; if(SPEED<1) SPEED=1; show_speed();}
  
  // Toggle line follower
  if(Ps3.event.button_up.ps) Line_follower = !Line_follower;
}

void onConnect(){
  Serial.println("PS3 Connected");
  colorWipe(strip.Color(0,0,255),50);
}

// ===================== SETUP =====================
void setup(){
  Serial.begin(115200);
  
  // PS3 Controller
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("00:1a:7d:da:71:10");
  
  // NeoPixel
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
  
  // PWM setup
  ledcSetup(1,PWM_FREQ,PWM_RESOLUTION); ledcAttachPin(MOTOR_RIGHT_EN,1);
  ledcSetup(0,PWM_FREQ,PWM_RESOLUTION); ledcAttachPin(MOTOR_LEFT_EN,0);
  
  // Motor pins
  pinMode(MOTOR1_1,OUTPUT); pinMode(MOTOR1_2,OUTPUT);
  pinMode(MOTOR2_1,OUTPUT); pinMode(MOTOR2_2,OUTPUT);
  pinMode(MOTOR3_1,OUTPUT); pinMode(MOTOR3_2,OUTPUT);
  pinMode(MOTOR4_1,OUTPUT); pinMode(MOTOR4_2,OUTPUT);
  
  STOP();
}

// ===================== LOOP =====================
void loop(){
  // Read IR sensors
  IR_FL = analogRead(IR_FRONT_LEFT);
  IR_FM = analogRead(IR_FRONT_MID);
  IR_FR = analogRead(IR_FRONT_RIGHT);
  IR_BL = analogRead(IR_BACK_LEFT);
  IR_BM = analogRead(IR_BACK_MID);
  IR_BR = analogRead(IR_BACK_RIGHT);
  
  // Autonomous line-following
  while(Line_follower){
    if(IR_FL>IR_FRONT_LEFT_T && IR_FM<IR_FRONT_MID_T && IR_FR>IR_FRONT_RIGHT_T){
      forward(5);
    }
    // Add more line-following conditions as needed
  }
  
  // Fail-safe restart
  if(!Ps3.isConnected()){
    static unsigned long t = millis();
    if(millis()-t>10000) ESP.restart();
  }
}
