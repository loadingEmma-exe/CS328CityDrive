#include <Wire.h>
#include "Adafruit_TCS34725.h"

// ==================================================
// MOTOR PINS (Arduino Mega)
// ==================================================
#define MotorPWM_A 4     // Left motor PWM
#define MotorPWM_B 5     // Right motor PWM

#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

// ==================================================
// SENSOR CHANNELS (TCA9548A)
// ==================================================
#define LEFT_SENSOR_CHANNEL    0
#define CENTER_SENSOR_CHANNEL  1
#define RIGHT_SENSOR_CHANNEL   2
#define rgbLED 48

// ==================================================
// SPEED SETTINGS
// ==================================================
#define PWM_FORWARD    75
#define PWM_TURN       85
#define PWM_SOFTTURN   55
#define PWM_REVERSE    70

// ==================================================
// BLACK DETECTION THRESHOLD
// Change after calibration
// ==================================================
int threshold = 20;

// ==================================================
Adafruit_TCS34725 tcs =
Adafruit_TCS34725(
TCS34725_INTEGRATIONTIME_24MS,
TCS34725_GAIN_1X
);

// ==================================================
// SELECT TCA9548A CHANNEL
// ==================================================
void tcaSelect(uint8_t channel)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// ==================================================
// READ SENSOR + DETECT BLACK
// ==================================================
bool blackDetected(uint8_t channel)
{
  tcaSelect(channel);
  delay(20);

  tcs.begin();   // important

  uint16_t r,g,b,c;
  tcs.getRawData(&r,&g,&b,&c);

  Serial.println(c);

  return (c < threshold);
}

uint16_t readClear(uint8_t channel)
{
  tcaSelect(channel);
  delay(15);

  tcs.begin();      // VERY IMPORTANT
  delay(5);

  uint16_t r,g,b,c;
  tcs.getRawData(&r,&g,&b,&c);

  return c;
}

// ==================================================
void setup() {
 Serial.begin(9600);
 Wire.begin();

 pinMode(rgbLED, OUTPUT);
 digitalWrite(rgbLED, HIGH);   // try LOW first

 tcaSelect(0);

 if(!tcs.begin()) Serial.println("No sensor");
 else Serial.println("Sensor OK");
}

void loop()
{
  Serial.print("L:");
  Serial.print(readClear(0));

  Serial.print(" C:");
  Serial.print(readClear(1));

  Serial.print(" R:");
  Serial.println(readClear(2));

  delay(500);
}

// ==================================================
// MOTOR FUNCTIONS
// ==================================================
void Forward()
{
  analogWrite(MotorPWM_A, PWM_FORWARD);
  analogWrite(MotorPWM_B, PWM_FORWARD);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Reverse()
{
  analogWrite(MotorPWM_A, PWM_REVERSE);
  analogWrite(MotorPWM_B, PWM_REVERSE);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void Left()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Right()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void LeftSoft()
{
  analogWrite(MotorPWM_A, PWM_SOFTTURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void RightSoft()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_SOFTTURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Stop()
{
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, LOW);
}