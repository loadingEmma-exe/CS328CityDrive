//Includes the Arduino Stepper Library
#include <Servo.h>
#include <Pixy2.h>

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

#define echoPin 22
#define trigPin 23

#define MotorPWM_A 4 //left motor
#define MotorPWM_B 5 //right motor
#define BLUETOOTH_BAUD_RATE 38400

// Motor pins
#define MotorPWM_L 4   // left motor PWM
#define MotorPWM_R 5   // right motor PWM
#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

#define LTsensor_l  8   // left sensor (yellow)
#define LTsensor_c  7   // center sensor (green)
#define LTsensor_r  6   // right sensor (white)
#define RGBLED 48

//Ultrasonic sensor variables
long duration;
float distance;

//Pixy2 Camera declaration
Pixy2 pixy;

/*Buzzer Music Stuff*/

// change this to make the song slower or faster
int tempo = 125;

//Buzzer pin
int buzzer = 11;

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

int melody2[] = { //Amaurot (Neath Dark Waters - FFXIV)
  NOTE_FS4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_A4, 2, NOTE_D4, 8, NOTE_A4, 8, NOTE_FS4, 8, NOTE_G4, 4, NOTE_A4, 2,
  NOTE_FS4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_A4, 2, NOTE_D4, 8, NOTE_G4, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_G4, 4, NOTE_A4, 2,

  NOTE_FS4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_A4, 2, NOTE_D4, 8, NOTE_A4, 8, NOTE_FS4, 8, NOTE_G4, 4, NOTE_A4, 2,
  NOTE_FS4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_A4, 2, NOTE_D4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_C5, 4, NOTE_D5, 2,

  NOTE_A4, 8, NOTE_AS4, 8, NOTE_C5, 8, NOTE_C5, 2, NOTE_F4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_AS4, 4, NOTE_C5, 2,
  NOTE_A4, 8, NOTE_AS4, 8, NOTE_C5, 8, NOTE_C5, 2, NOTE_F4, 8, NOTE_AS4, 8, NOTE_DS5, 8, NOTE_D5, 8, NOTE_AS4, 8, NOTE_C5, 2,

  NOTE_A4, 8, NOTE_AS4, 8, NOTE_C5, 8, NOTE_C5, 2, NOTE_F4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_AS4, 4, NOTE_C5, 2,
  NOTE_A4, 8, NOTE_AS4, 8, NOTE_C5, 8, NOTE_C5, 2, NOTE_F4, 8, NOTE_C5, 8, NOTE_DS5, 8, NOTE_D5, 4, NOTE_DS5, 8, NOTE_F5, 2,
};

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
/*Servomotor*/
Servo myServo;
// Defines the number of steps per rotation

// Servo angles
int dirLeft = 120;
int dirStraight = 95;
int dirRight = 60;

bool lineMode = false;
// ============================
// Motor control
// ============================
void Forward(int speed) {
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
}

void Halt(int startSpeed)
{
  for (int s = startSpeed; s > 0; s -= 20) {
    Forward(s);
    delay(20);
  }
  StopMotors();
  
}

void Turn(int maxSpeed, int turnTime)
{
  for (int s = 80; s <= maxSpeed; s += 20) {
    Right(s);
    delay(20);
  }
  delay(turnTime);  // actual turning duration
  StopMotors();
}

void Accelerate(int maxSpeed)
{
  for (int s = 80; s <= maxSpeed; s += 20) {
    Forward(s);
    delay(20);
  }
}

void ffVictory()
{
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

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
  delay(20);
  }
}

void Amaurot()
{
  int notes = sizeof(melody2) / sizeof(melody2[0]) / 2; 
  for (int i = 0; i < sizeof(melody2) / sizeof(melody2[0]); i += 2) {

  divider = melody2[i + 1];

  if (divider > 0) {
    noteDuration = wholenote / divider;
  } else {
    noteDuration = (wholenote / abs(divider)) * 1.5;
  }

  tone(buzzer, melody2[i], noteDuration);
  delay(noteDuration);
  noTone(buzzer);
  delay(20);
  }
}

void dearlyBeloved()
{
  int notes = sizeof(melody1) / sizeof(melody1[0]) / 2; 
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
  delay(20);
  }
}

void pixyBarcode()
{
  pixy.line.getAllFeatures(); //get line features
  // pixy.line.getMainFeatures(); // Could use this too
  if (pixy.line.barcodes) // detected road sign
  {
  int code = pixy.line.barcodes[0].m_code;
    if (code == 0)
    {
      Serial.println("Going forward.");
      ffVictory();
    }
    else if (code == 14)
    {
      Serial.println("Turning Right..");
      Amaurot();
    }
    else if (code == 4)
    {
      Serial.println("Turning Left..");
      dearlyBeloved();
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  pixy.init();
  pixy.changeProg("line");
  Serial2.begin(BLUETOOTH_BAUD_RATE);

  // Motor pins
  pinMode(MotorPWM_L, OUTPUT);
  pinMode(MotorPWM_R, OUTPUT);
  pinMode(INA1A, OUTPUT);
  pinMode(INA2A, OUTPUT);
  pinMode(INA1B, OUTPUT);
  pinMode(INA2B, OUTPUT);

  //Line Sensor pins
  pinMode(LTsensor_l, INPUT);
  pinMode(LTsensor_c, INPUT);
  pinMode(LTsensor_r, INPUT);
  pinMode(RGBLED, OUTPUT);

  StopMotors();
  delay(1000);

  Serial.begin(9600);
  Serial2.begin(BLUETOOTH_BAUD_RATE);

   //Servomotor Setup
  Serial.begin(9600);
  myServo.attach(13); //Pin for servomotor input.
  myServo.write(dirStraight); // start centered

  // Serial.println("Commands: L (left), C (center), R (right)");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  ffVictory();
}


void loop() {
  digitalWrite(RGBLED, HIGH);
  if (Serial2.available() > 0) {
    char cmd = Serial2.read();

   switch (cmd)
    {
      case 'F':
        Forward(128);
        break;
      
      case 'S':
        StopMotors();
        break;

      case 'L':
        Left(100);
        break;

      case 'R':
        Right(100);
        break;

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

      case 'X':
        pixyBarcode();
    }
  }

    // Set the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // The pulseIn function times the signal return after bouncing off the object
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (wave goes and comes back)
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
    delay(100);
}