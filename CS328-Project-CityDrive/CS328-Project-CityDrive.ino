//From LAB 5 PWM Motor.c.txt
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "protothreads.h" //protothreading
#include <SoftwareSerial.h>

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define LOGO_WIDTH 8 // OLED display width, in pixels
#define LOGO_HEIGHT 8 // OLED display height, in pixels

//Color definitions
#define BLACK 0x000000
#define BLUE 0x0000FF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define CYAN 0x00FFFF
#define MAGENTA 0xFF00FF
#define YELLOW 0xFFFF00
#define WHITE 0xFFFFF

#define MotorPWM_A 4 //left motor
#define MotorPWM_B 5 //right motor
#define BLUETOOTH_BAUD_RATE 38400

//protothreads
int PTdelay = 100;

pt ptBlink;
pt ptCamera;
pt ptMovement;
pt ptMusic;
pt ptOLED;

//Turn Defs
int right = 0;
int left = 0;
int moving = 0;
int hazards = 0;

//Timer
int globalTime = 0;
int ptTime = 0;

// Motor pins
#define MotorPWM_L 4   // left motor PWM
#define MotorPWM_R 5   // right motor PWM
#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

//LED pins
#define LEFTREAR 31
#define LEFTFRONT 49
#define RIGHTREAR 37
#define RIGHTFRONT 43

// Encoder pins
#define ENCODER_LEFT  2
#define ENCODER_RIGHT 3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //init display

// ============================
// Encoder counters
// ============================
volatile long count_left = 0;
volatile long count_right = 0;

// ============================
// Light control
// ============================
void OFFLights(){
  digitalWrite(LEFTREAR, LOW);
  digitalWrite(RIGHTREAR, LOW);
  digitalWrite(LEFTFRONT, LOW);
  digitalWrite(RIGHTFRONT, LOW);  
}

void ONLights(){
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(RIGHTREAR, HIGH);
  digitalWrite(LEFTFRONT, HIGH);
  digitalWrite(RIGHTFRONT, HIGH);
}

void HEADLights(){
  digitalWrite(LEFTFRONT, HIGH);
  digitalWrite(RIGHTFRONT, HIGH);
}

void BREAKLights(){
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(RIGHTREAR, HIGH);
}

void RIGHTLights(){
  digitalWrite(RIGHTREAR, HIGH);
  digitalWrite(RIGHTFRONT, HIGH);
}

void LEFTLights(){
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(LEFTFRONT, HIGH);
}

// Method: writetext
// Input: x position
// Print Lab Text across the screen
void writetext(int x) { 
  String text = "Lab 4 by: Emma Raymond Austin Hoang";

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false); //Ensures the text does not wrap around to the second line of the OLCD display.
  display.setCursor(x, 0);
  display.print(text);
}

// ============================
// Motor control
// ============================
void Forward(int speed) {
  moving = 1;
  left = 0;
  right = 0;
  hazards = 0;

  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed);

  // Left motor forward
  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);

  // Right motor forward
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Backward(int speed) {
  moving = 1;
  left = 0;
  right = 0;
  hazards = 0; //should this be on?

  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed);

  // Left motor backward
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);

  // Right motor backward
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void Left(int speed) 
{
  moving = 1;
  left = 1;
  right = 0;
  hazards = 0;

  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed - 20);
  
  // Left motor backward
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);

  // Right motor forward
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Right(int speed)
{
  moving = 1;
  left = 0;
  right = 1;
  hazards = 0;

  analogWrite(MotorPWM_L, speed - 20);
  analogWrite(MotorPWM_R, speed);
  
  // Left motor forward
  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);

  // Right motor backward
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void StopMotors() {
  analogWrite(MotorPWM_L, 0);
  analogWrite(MotorPWM_R, 0);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, LOW);

  moving = 0;
  left = 0;
  right = 0;
  hazards = 0;
}

// ============================
// General Movement Routines
// ============================
void Halt(int startSpeed){
  for (int s = startSpeed; s > 0; s -= 20) {
    Forward(s);
    delay(20);
  }
  StopMotors();
}

void Turn(int maxSpeed, int turnTime){
  for (int s = 80; s <= maxSpeed; s += 20) {
    Right(s);
    delay(20);
  }
  delay(turnTime);  // actual turning duration
  StopMotors();
}

void Accelerate(int maxSpeed){
  for (int s = 80; s <= maxSpeed; s += 20) {
    Forward(s);
    delay(20);
  }
}

// ============================
// Protothreading
// ============================
int blinkThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    //action
    PT_SLEEP(mythread, PTdelay);
    //action
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int musicThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    //action
    PT_SLEEP(mythread, PTdelay);
    //action
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

// ============================
// SETUP 
// ============================
void setup() {
  Serial.begin(9600);
  Serial2.begin(BLUETOOTH_BAUD_RATE);

  // Motor pins
  pinMode(MotorPWM_L, OUTPUT);
  pinMode(MotorPWM_R, OUTPUT);
  pinMode(INA1A, OUTPUT);
  pinMode(INA2A, OUTPUT);
  pinMode(INA1B, OUTPUT);
  pinMode(INA2B, OUTPUT);

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;); //Don't proceed, loop forever
  }

  // Encoder pins
  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);

  // Light Control
  pinMode(LEFTFRONT, OUTPUT);
  pinMode(RIGHTFRONT, OUTPUT);
  pinMode(LEFTREAR, OUTPUT);
  pinMode(RIGHTREAR, OUTPUT);

  //pt setup
  PT_INIT(&ptBlink);

  //attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), ISRMotorLeft, FALLING);
  //attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), ISRMotorRight, FALLING);

  StopMotors();
  delay(1000);

  Serial.begin(9600);
  Serial2.begin(BLUETOOTH_BAUD_RATE);
  display.clearDisplay();
  display.display();
}

// ============================
// Loop
// ============================
void loop() {
  // put your main code here, to run repeatedly:
    if (Serial2.available()) {

    char cmd = Serial2.read();

      if (cmd == 'F') {
        Forward(128);
      }
      else if (cmd == 'B') {
        Backward(128);
      }
      else if (cmd == 'S') {
        StopMotors();
      }
      else if (cmd == 'L') {
        Left(100);
      }
      else if (cmd == 'R') {
        Right(100);
      }
      else if (cmd == 'Q') {
        //PT_SCHEDULE(blinkThread(&ptBlink)); //pt blink
        if (moving){ //first requirement
          HEADLights();
          globalTime = 0;
        }
        else if(!moving){
          OFFLights();
          globalTime += pTime;
        }

        if (left){ //second
          LEFTLights();
        }
        else if(right){
          RIGHTLights();
        }
        else if(moving){
          OFFLights();
          HEADLights();
        }

        if (globalTime > 2 * pTime){
          hazards = 1;
        }

        if(hazards){
          HAZARDLights();
        }

      }
  }
}