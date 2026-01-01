#include <Ps3Controller.h>      // Library for PS3 controller communication
#include <Adafruit_NeoPixel.h>  // Library to control NeoPixel LEDs

#define BRIGHTNESS 50            // Default brightness for NeoPixel strip
#define neopixel  14             // GPIO pin connected to NeoPixel data
Adafruit_NeoPixel strip(30, neopixel, NEO_GRB + NEO_KHZ800); // 30 LEDs on GPIO14
int SPEED = 9;                 // Default speed for motors (1-9 scale)

// ==================== INITIALIZATION ====================
void initAhmed() {
    Serial.begin(9600);        // Start serial communication for debugging
    pinMode(neopixel, OUTPUT); // Set NeoPixel pin as output
    strip.begin();             // Initialize NeoPixel strip
    strip.show();              // Turn off all LEDs initially
    strip.setBrightness(BRIGHTNESS); // Set LED brightness
}

// Fill all LEDs with a specific color gradually
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

// Light up all LEDs blue
void leds_blue() {
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 255));
        strip.show();
        delay(50);
    }
}

// Light up only specific LEDs
void specific(int pixels[]) {
    for(int i = 0; i < strip.numPixels(); i++) {
        for (int x = 0; x < sizeof(pixels); x++) {
            if (pixels[x] == i) {
                strip.setPixelColor(i, strip.Color(255, 215, 0)); // Gold color
                strip.show();
                delay(10);
            }
        }
    }
}

// Clear specific LEDs
void specific_clear(int pixels[]) {
    for(int i = 0; i < strip.numPixels(); i++) {
        for (int x = 0; x < sizeof(pixels); x++) {
            if (pixels[x] == i) {
                strip.setPixelColor(i, strip.Color(247, 0, 255)); // Purple color
                strip.show();
                delay(10);
            }
        }
    }
}

// Display current speed on LEDs
void show_speed() {
    int s = ((SPEED * 4) / 9) + 1;
    for(int i = 0; i < 5; i++) {
        strip.setPixelColor(i + 8, strip.Color(247, 0, 255)); // Purple background
        strip.show();
    }
    for(int i = 0; i < s; i++) {
        strip.setPixelColor(i + 8, strip.Color(64, 224, 208)); // Cyan for speed
        strip.show();
    }
}

// ==================== MOTOR PINS ====================
#define MOTOR1_1  4
#define MOTOR2_1  16
#define MOTOR1_2  17
#define MOTOR2_2  5
#define MOTOR3_1  19
#define MOTOR3_2  18
#define MOTOR4_2  23
#define MOTOR4_1  22

// ==================== IR SENSOR PINS ====================
#define IR_FRONT_LEFT  34
int IR_FRONT_LEFT_PIXELS [] = {20};
#define IR_FRONT_MID   35
int IR_FRONT_MID_PIXELS  [] = {16,17};
#define IR_FRONT_RIGHT 32
int IR_FRONT_RIGHT_PIXELS[] = {13};
#define IR_BACK_LEFT  25
int IR_BACK_LEFT_PIXELS [] = {0,1};
#define IR_BACK_MID   26
int IR_BACK_MID_PIXELS  [] = {29};
#define IR_BACK_RIGHT 27
int IR_BACK_RIGHT_PIXELS[]= {4};

// Sensor thresholds
const int IR_MOST_BLACK = 2000;
const int IR_LEAST_WHITE = 3000;

const int IR_FRONT_LEFT_T = 3530 - 500;
const int IR_FRONT_MID_T  = 2760 - 500;
const int IR_FRONT_RIGHT_T = 2650 - 500;
const int IR_BACK_LEFT_T   = 1450 - 500;
const int IR_BACK_MID_T    = 3330 - 500;
const int IR_BACK_RIGHT_T  = 2850 - 500;

// Variables to store sensor readings
int IR_FRONT_LEFT_READING = 0;
int IR_FRONT_MID_READING = 0;
int IR_FRONT_RIGHT_READING = 0;
int IR_BACK_LEFT_READING = 0;
int IR_BACK_MID_READING = 0;
int IR_BACK_RIGHT_READING = 0;

// ==================== PWM DEFINITIONS ====================
#define MOTOR3_4_EN 15
#define MOTOR1_2_EN 13
#define PWMFreq        1000
#define RIGHT_MOTORS   1
#define LEFT_MOTORS    0
#define PWMResolution  6
#define factor 0.25
#define F2 300

boolean Line_follower = 0;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

// ==================== MOTOR CONTROL FUNCTIONS ====================
void forward(int duty) {
    set_speed_left(duty);
    set_speed_right(duty);
    digitalWrite(MOTOR1_1, HIGH);
    digitalWrite(MOTOR1_2, LOW);
    digitalWrite(MOTOR2_1, HIGH);
    digitalWrite(MOTOR2_2, LOW);
    digitalWrite(MOTOR3_1, HIGH);
    digitalWrite(MOTOR3_2, LOW);
    digitalWrite(MOTOR4_1, HIGH);
    digitalWrite(MOTOR4_2, LOW);
}

void reverse(int duty) {
    set_speed_left(duty);
    set_speed_right(duty);
    digitalWrite(MOTOR1_1, LOW);
    digitalWrite(MOTOR1_2, HIGH);
    digitalWrite(MOTOR2_1, LOW);
    digitalWrite(MOTOR2_2, HIGH);
    digitalWrite(MOTOR3_1, LOW);
    digitalWrite(MOTOR3_2, HIGH);
    digitalWrite(MOTOR4_1, LOW);
    digitalWrite(MOTOR4_2, HIGH);
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

// Forward-right motion
void forward_right(int duty) {
    set_speed_left(duty);
    set_speed_right(duty * factor);
    forward(duty);
}

// Forward-left motion
void forward_left(int duty) {
    set_speed_left(duty * factor);
    set_speed_right(duty);
    forward(duty);
}

// Reverse-right motion
void reverse_right(int duty) {
    set_speed_left(duty);
    set_speed_right(duty * factor);
    reverse(duty);
}

// Reverse-left motion
void reverse_left(int duty) {
    set_speed_left(duty * factor);
    set_speed_right(duty);
    reverse(duty);
}

// Left turn
void left(int duty) {
    set_speed_left(duty);
    set_speed_right(duty);
    digitalWrite(MOTOR1_1, HIGH);
    digitalWrite(MOTOR1_2, LOW);
    digitalWrite(MOTOR2_1, HIGH);
    digitalWrite(MOTOR2_2, LOW);
    digitalWrite(MOTOR3_1, LOW);
    digitalWrite(MOTOR3_2, HIGH);
    digitalWrite(MOTOR4_1, LOW);
    digitalWrite(MOTOR4_2, HIGH);
}

// Right turn
void right(int duty) {
    set_speed_left(duty);
    set_speed_right(duty);
    digitalWrite(MOTOR1_1, LOW);
    digitalWrite(MOTOR1_2, HIGH);
    digitalWrite(MOTOR2_1, LOW);
    digitalWrite(MOTOR2_2, HIGH);
    digitalWrite(MOTOR3_1, HIGH);
    digitalWrite(MOTOR3_2, LOW);
    digitalWrite(MOTOR4_1, HIGH);
    digitalWrite(MOTOR4_2, LOW);
}

// ==================== PS3 CONTROLLER CALLBACK ====================
void notify() {
    // Example: Digital buttons
    if (Ps3.event.button_up.cross) STOP();
    if (Ps3.event.button_up.square) STOP();
    if (Ps3.event.button_up.triangle) STOP();
    if (Ps3.event.button_up.circle) STOP();

    // Speed adjustment
    if (Ps3.event.button_up.select) {
        SPEED++;
        if (SPEED > 9) SPEED = 9;
        show_speed();
    }
    if (Ps3.event.button_up.start) {
        SPEED--;
        if (SPEED < 1) SPEED = 1;
        show_speed();
    }

    // Toggle line follower mode
    if (Ps3.event.button_up.ps) Line_follower = !Line_follower;

    // Analog buttons control
    if (Ps3.data.analog.button.up == 255) forward(SPEED);
    if (Ps3.data.analog.button.down == 255) reverse(SPEED);
    if (Ps3.data.analog.button.left == 255) left(SPEED);
    if (Ps3.data.analog.button.right == 255) right(SPEED);
    if (Ps3.data.analog.button.triangle == 255) forward_right(SPEED);
    if (Ps3.data.analog.button.circle == 255) reverse_right(SPEED);
    if (Ps3.data.analog.button.cross == 255) forward_left(SPEED);
    if (Ps3.data.analog.button.square == 255) reverse_left(SPEED);
}

// Called when controller connects
void onConnect() {
    Serial.println("Connected.");
    colorWipe(strip.Color(247, 0, 255), 50);
}

// ==================== SETUP FUNCTION ====================
void setup() {
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:1a:7d:da:71:10");
    Serial.println("Ready.");

    // Initialize NeoPixel
    colorWipe(strip.Color(0, 0, 255), 50);

    // Setup PWM channels
    ledcSetup(RIGHT_MOTORS, PWMFreq, PWMResolution);
    ledcAttachPin(MOTOR1_2_EN, RIGHT_MOTORS);
    ledcSetup(LEFT_MOTORS, PWMFreq, PWMResolution);
    ledcAttachPin(MOTOR3_4_EN, LEFT_MOTORS);

    // Set motor pins as output
    pinMode(MOTOR1_1, OUTPUT);
    pinMode(MOTOR2_1, OUTPUT);
    pinMode(MOTOR2_2, OUTPUT);
    pinMode(MOTOR3_1, OUTPUT);
    pinMode(MOTOR1_2, OUTPUT);
    pinMode(MOTOR3_2, OUTPUT);
    pinMode(MOTOR4_2, OUTPUT);
    pinMode(MOTOR4_1, OUTPUT);

    digitalWrite(MOTOR3_1, HIGH);
    digitalWrite(MOTOR3_2, HIGH);
}

// ==================== MAIN LOOP ====================
void loop() {
    // Read IR sensors
    IR_FRONT_LEFT_READING  = analogRead(IR_FRONT_LEFT);
    IR_FRONT_MID_READING   = analogRead(IR_FRONT_MID);
    IR_FRONT_RIGHT_READING = analogRead(IR_FRONT_RIGHT);
    IR_BACK_LEFT_READING   = analogRead(IR_BACK_LEFT);
    IR_BACK_MID_READING    = analogRead(IR_BACK_MID);
    IR_BACK_RIGHT_READING  = analogRead(IR_BACK_RIGHT);

    // Print sensor readings for debugging
    Serial.println("");
    Serial.print(" IR_BACK_LEFT_READING "); Serial.println(IR_BACK_LEFT_READING);
    Serial.print(" IR_BACK_MID_READING "); Serial.println(IR_BACK_MID_READING);
    Serial.print(" IR_BACK_RIGHT_READING "); Serial.println(IR_BACK_RIGHT_READING);
    Serial.println("");

    // Autonomous line-following logic
    while (Line_follower) {
        // Logic to control robot based on IR readings
        if (IR_FRONT_LEFT_READING > IR_FRONT_LEFT_T && IR_FRONT_MID_READING < IR_FRONT_MID_T && IR_FRONT_RIGHT_READING > IR_FRONT_RIGHT_T) {
            forward(5);
        }
        // Additional line-following conditions go here...
    }
}

// ==================== HELPER FUNCTIONS ====================
void set_speed_right(int duty) {
    if (duty == 1) duty = 2;
    ledcWrite(RIGHT_MOTORS, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}

void set_speed_left(int duty) {
    if (duty == 1) duty = 2;
    ledcWrite(LEFT_MOTORS, map(duty, 0, 9, 0, MAX_DUTY_CYCLE));
}
