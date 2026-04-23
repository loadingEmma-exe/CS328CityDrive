#include <SPI.h>
#include <Wire.h>
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "protothreads.h" //protothreading
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Pixy2.h>

//========================
// Bluetooth Defintions
//========================

#define BLUETOOTH_BAUD_RATE 38400

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

byte LArrow[8] = { //Robot's left arrow sprite.
  0b00000000,
  0b00110000,
  0b00111100,
  0b01111110,
  0b00111100,
  0b00110000,
  0b00000000,
  0b00000000
};

byte RArrow[8] = { //Robot's right arrow sprite.
  0b00000000,
  0b00001100,
  0b00111100,
  0b01111110,
  0b00111100,
  0b00001100,
  0b00000000,
  0b00000000
};

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

int projectTime = 0;
int preprojectTime;
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

int rON = 0;
int lON = 0;
int fON = 0;
int bON = 0;
int hON = 0;

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

int songFF = 0; //in camera, will turn to 1. When played will turn to 0
int FFNOTE = sizeof(melody) / sizeof(melody[0]);

int songDB = 0;
int KHNOTE;

int songAM = 0;
int AMNOTE;

//========================
// Servo Defintions
//========================

Servo myServo;

// Servo angles
int dirLeft = 120;
int dirStraight = 95;
int dirRight = 60;

//========================
// UltraSonic Defintions
//========================

#define echoPin 22
#define trigPin 23

//Ultrasonic sensor variables
long duration;
float distance;
int trigTriggerCount; //if 0 --> low, 1 --> high, 2 --> low

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

//=======================
// Line Sensor Defintions
//=======================

//line sensors themselves
#define LNSensorL  8  // left sensor 
#define LNSensorC  7  // center sensor 
#define LNSensorR  6  // right sensor 

//Line Sensor pins for the little lights
pinMode(LNSensorL, INPUT);
pinMode(LNSensorC, INPUT);
pinMode(LNSensorR, INPUT);
//pinMode(RGBLED, OUTPUT);

//========================
// Pixy Camera Defintions
//========================

Pixy2 pixy;

void pixyBarcode(){
  pixy.line.getAllFeatures(); //get line features
  // pixy.line.getMainFeatures(); // Could use this too
  if (pixy.line.barcodes) // detected road sign
  {
    int code = pixy.line.barcodes[0].m_code;
    switch (code){
      case 0:
        Serial.println(code + " Pixy Read");
      break;
      case 1:
        Serial.println(code + " Pixy Read");
      break;
      case 2:
        Serial.println(code + " Pixy Read");
      break;
      case 3:
        Serial.println(code + " Pixy Read");
      break;
      case 4:
        Serial.println(code + " Pixy Read");
      break;
      case 5:
        Serial.println(code + " Pixy Read");
      break;
      default:
        Serial.println("Default Pixy Read");
      break;
    }
  }
}

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

void HEADLights(int side = 0){
  if (side == -1){ //left side
    digitalWrite(LEFTFRONT, HIGH);
  }
  else if (side == 1){ //right side
    digitalWrite(RIGHTFRONT, HIGH);
  }
  else{
    digitalWrite(RIGHTFRONT, HIGH);
    digitalWrite(LEFTFRONT, HIGH);
  }
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
    if(moving){
      OFFLights();
      HEADLights();
      projectTime = 0;
    }

    if(!moving){
      projectTime += 100;
      BREAKLights();
    }

    if(rON && right){ //if turning right and need to blink off
      OFFLights();
      HEADLights(-1);
      rON = !rON;
      PT_SLEEP(mythread, PTdelay);
    }
    
    if(!rON && right) { //if turning right and need to blink on
      OFFLights();
      HEADLights(-1);
      RIGHTLights();
      rON = !rON;
      PT_SLEEP(mythread, PTdelay);
    }

    if(lON && left){ //if turning left and need to blink off
      OFFLights();
      HEADLights(1);
      lON = !lON;
      PT_SLEEP(mythread, PTdelay);
    }

    if(!lON && left) { //if turning left and need to blink on
      OFFLights();
      HEADLights(1);
      LEFTLights();
      lON = !lON;
      PT_SLEEP(mythread, PTdelay);
    }

    if(projectTime >= 2000){
      hON = !hON;
    }

    if(projectTime >= 2000 && hON){ //if hazards and needs to blink off
      OFFLights();
      hON = !hON;
      PT_SLEEP(mythread, PTdelay);
    }

    if(projectTime >= 2000 && !hON){ //if hazards and needs to blink on
      HEADLights(1);
      LEFTLights();
      lON = !lON;

      HEADLights(-1);
      RIGHTLights();
      rON = !rON;
      PT_SLEEP(mythread, PTdelay);
    }

    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int cameraThread(struct pt* mythread){ //barcode scanning
  PT_BEGIN(mythread);

  for(;;){
    pixy.line.getAllFeatures(); //get line features
    if (pixy.line.barcodes) // detected road sign
    {
      int code = pixy.line.barcodes[0].m_code;
      switch (code){
        case 0: //start
          Serial.println("0 Pixy Read, Start");
        break;
        case 1: //Turn Right
          Serial.println("1 Pixy Read, Right");
        break;
        case 2: //U turn right
          Serial.println("2 Pixy Read, Right U turn");
        break;
        case 3: case 14: //U turn left
          Serial.println("3 Pixy Read, Left U turn");
        break;
        case 4: //Turn Left
          Serial.println("4 Pixy Read, Left turn");
        break;
        case 5: //stop
          Serial.println("5 Pixy Read, Stop");
        break;
        default:
          Serial.println("Default Pixy Read");
        break;
      }
    }
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int musicThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    switch (FFNOTE){
      case 0:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      case 1:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      case 2:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      case 3:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      case 4:
        tone(buzzer, NOTE_C5, noteDuration);
        FFNOTE++;
        break;
      case 5:
        tone(buzzer, NOTE_D5, noteDuration);
        FFNOTE++;
        break;
      case 6:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      case 7:
        tone(buzzer, NOTE_D5, noteDuration);
        FFNOTE++;
        break;
      case 8:
        tone(buzzer, NOTE_E5, noteDuration);
        FFNOTE++;
        break;
      default:
        FFNOTE = 0;
        break;
    }
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int movementThread(struct pt* mythread){ //NOT TESTED
  PT_BEGIN(mythread);

  for(;;){
    if (Serial2.available()) {
      char cmd = Serial2.read();
      
      switch (cmd)
      {
        case 'F': case 'f':
          Forward(128);
          break;
        
        case 'S': case 's':
          StopMotors();
          break;

        case 'L': case 'l':
          Left(100);
          break;

        case 'R': case 'r':
          Right(100);
          break;
      }
    }

    PT_SLEEP(mythread, PTdelay);

  }

  PT_END(mythread);
}

int OLEDThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    display.drawBitmap(120, (SCREEN_HEIGHT - LOGO_HEIGHT) / 2, LArrow, LOGO_WIDTH, LOGO_HEIGHT, YELLOW); //Robot's left arrow
    display.drawBitmap(4, (SCREEN_HEIGHT - LOGO_HEIGHT) / 2, RArrow, LOGO_WIDTH, LOGO_HEIGHT, YELLOW); //Robot's right arrow
    Serial.println("OLED");
    PT_SLEEP(mythread, PTdelay);
  }

  PT_END(mythread);
}

int servoThread(struct pt* mythread){
  PT_BEGIN(mythread);

  for(;;){
    if (trigTriggerCount == 0){
      // Set the trigPin condition
      digitalWrite(trigPin, LOW);
      trigTriggerCount++;

      PT_SLEEP(mythread, PTdelay);
    }
    else if (trigTriggerCount == 1){
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      trigTriggerCount++;

      PT_SLEEP(mythread, PTdelay);
    }
    else if (trigTriggerCount == 2){
      digitalWrite(trigPin, LOW);
      // The pulseIn function times the signal return after bouncing off the object
      duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (wave goes and comes back)
      // Displays the distance on the Serial Monitor
      Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
      trigTriggerCount = 0;

      PT_SLEEP(mythread, PTdelay);

    }

    if (Serial2.available()) {
      char cmd = Serial2.read();
      //servo shit

      switch (cmd) {
        case 'A':
          myServo.write(dirRight);
          Serial.println("Looking right");
          break;

        case 'C':
          myServo.write(dirStraight);
          Serial.println("Centered");
          break;

        case 'D':
          myServo.write(dirLeft);
          Serial.println("Looking left");
          break;
      }
    
      PT_SLEEP(mythread, PTdelay);
    }
  }
  PT_END(mythread);
}

// ============================
// Setup
// ============================
void setup() {
  Serial.begin(9600);
  myServo.attach(13); //Pin for servomotor input.
  myServo.write(dirStraight); // start centered
  Serial2.begin(BLUETOOTH_BAUD_RATE);
  pixy.init();
  pixy.changeProg("line");

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
  PT_SCHEDULE(servoThread(&ptServo));
  PT_SCHEDULE(movementThread(&ptMovement));
  PT_SCHEDULE(cameraThread(&ptCamera));
  //PT_SCHEDULE(musicThread(&ptMusic));
  PT_SCHEDULE(blinkThread(&ptBlink));
  // PT_SCHEDULE(OLEDThread(&ptOLED));
}