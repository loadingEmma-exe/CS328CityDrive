#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "protothreads.h" //protothreading
#include <SoftwareSerial.h>
#include <Servo.h>

//========================
// OLED Defintions
//========================

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define LOGO_WIDTH 8 // OLED display width, in pixels
#define LOGO_HEIGHT 8 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //init display

//Color definitions
#define BLACK 0x000000
#define BLUE 0x0000FF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define CYAN 0x00FFFF
#define MAGENTA 0xFF00FF
#define YELLOW 0xFFFF00
#define WHITE 0xFFFFF

//========================
// Bluetooth Defintions
//========================

#define BLUETOOTH_BAUD_RATE 38400

//========================
// Ultrasonic Defintions
//========================
#define echoPin 22
#define trigPin 23
long duration;
float distance;

//========================
// Movement Defintions
//========================

// Motor pins
#define MotorPWM_L 4   // left motor PWM
#define MotorPWM_R 5   // right motor PWM
#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

// Movement indicators
int right = 0;
int left = 0;
int moving = 0;
int hazards = 0;

//========================
// Timer Defintions
//========================

int globalTime = 0;
int ptTime = 0;

//========================
// Buzzer Defintions
//========================
int buzzer = 11;

//========================
// LED Defintions
//========================

//LED pins
#define LEFTREAR 31
#define LEFTFRONT 49
#define RIGHTREAR 37
#define RIGHTFRONT 43

//========================
// Encoder Defintions
//========================

//Encoder pins
#define ENCODER_LEFT  2
#define ENCODER_RIGHT 3

//========================
// Music Defintions
//========================

//Music note definitions
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

int melody1[] = { //Dearly Beloved - Kingdom Hearts 1
  NOTE_C5,4,  NOTE_C5,8,  NOTE_G4,8,  NOTE_G4,8,
  NOTE_F4,4,  NOTE_F4,8,  NOTE_D5,4,  NOTE_D5,8,

  NOTE_C5,4,  NOTE_C5,8,  NOTE_G4,8,  NOTE_G4,8,
  NOTE_F4,4,  NOTE_F4,8,  NOTE_D5,4,  NOTE_D5,4,

  NOTE_DS5,4, NOTE_DS5,8, NOTE_D5,8, NOTE_D5,8,
  NOTE_G5,4,

  NOTE_F5,16, NOTE_G5,16, NOTE_F5,16,

  NOTE_F5,8,

  NOTE_DS5,4, NOTE_DS5,8, NOTE_D5,8, NOTE_D5,8,
  NOTE_C5,4,  NOTE_C5,8,  NOTE_AS4,4
};

int melody[] = { //Final Fantasy Victory Jingle
  NOTE_E5, 16, NOTE_E5,16, NOTE_E5, 16,
  NOTE_E5,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,16, NOTE_D5,16,
  NOTE_E5,4
};

// change this to make the song slower or faster
int tempo = 125;
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

// sample time in ms
const unsigned long sampleTime = 100;

//========================
// Servo Defintions
//========================

Servo myServo;

// Servo angles
int dirLeft = 120;
int dirStraight = 95;
int dirRight = 60;

//========================
// Protothreads Defintions
//========================
int PTdelay = 100;

pt ptBlink;
pt ptCamera;
pt ptMovement;
pt ptMusic;
pt ptOLED;
pt ptServo;

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

// ============================
// OLED control
// ============================

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

void Left(int speed) {
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

void Right(int speed){
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

int cameraThread(struct pt* mythread){
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

int movementThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    //action
    PT_SLEEP(mythread, PTdelay);
    //action
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int OLEDThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    //action
    PT_SLEEP(mythread, PTdelay);
    //action
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int servoThread(struct pt* mythread){
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
// MUSIC 
// ============================
void ffVictory()
{
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i += 2) {

    divider = melody[i + 1];

    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = (wholenote / abs(divider)) * 1.5;
    }

    tone(buzzer, melody[i], noteDuration);
    delay(noteDuration);
    noTone(buzzer);
    }
}

void dearlyBeloved()
{
    for (int i = 0; i < sizeof(melody1) / sizeof(melody1[0]); i += 2) {

    divider = melody1[i + 1];

    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = (wholenote / abs(divider)) * 1.5;
    }

    tone(buzzer, melody1[i], noteDuration);
    delay(noteDuration);
    noTone(buzzer);
    }
}

// ============================
// Setup
// ============================
void setup() {
  Serial.begin(9600);
  myServo.attach(13); //Pin for servomotor input.
  myServo.write(dirStraight); // start centered
  Serial2.begin(BLUETOOTH_BAUD_RATE);

  // Serial.println("Commands: L (left), C (center), R (right)");

  //Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
  PT_INIT(&ptCamera);
  PT_INIT(&ptMovement);
  PT_INIT(&ptMusic);
  PT_INIT(&ptOLED);
  PT_INIT(&ptServo);

  // for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i += 2) {

  // divider = melody[i + 1];

  // if (divider > 0) {
  //   noteDuration = wholenote / divider;
  // } else {
  //   noteDuration = (wholenote / abs(divider)) * 1.5;
  // }

  // tone(buzzer, melody[i], noteDuration);
  // delay(noteDuration);
  // noTone(buzzer);
  // delay(20);

  // StopMotors();
  // delay(1000);

  Serial.begin(9600);
  Serial2.begin(BLUETOOTH_BAUD_RATE);
  display.clearDisplay();
  display.display();

   //Servomotor Setup
  Serial.begin(9600);
  myServo.attach(13); //Pin for servomotor input.
  myServo.write(dirStraight); // start centered

  // Serial.println("Commands: L (left), C (center), R (right)");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

// ============================
// Loop
// ============================
void loop() {
  PT_SCHEDULE(&servoThread(ptServo));
  PT_SCHEDULE(&movementThread(ptMovement));
  PT_SCHEDULE(&cameraThread(ptCamera));
  PT_SCHEDULE(&MusicThread(ptMusic));
  PT_SCHEDULE(&blinkThread(ptBlink));
  PT_SCHEDULE(&blinkThread(ptOLED));
}
