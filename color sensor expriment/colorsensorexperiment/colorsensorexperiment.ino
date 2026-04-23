#include <Wire.h>                 // Allows Arduino to communicate using I2C
#include "Adafruit_TCS34725.h"   // Library for TCS34725 RGB color sensors

// ==================================================
// MOTOR PINS (Arduino Mega)
// These pins control the left and right motors
// ==================================================
#define MotorPWM_A 4     // PWM speed control for left motor
#define MotorPWM_B 5     // PWM speed control for right motor

#define INA1A 32         // Left motor direction pin 1
#define INA2A 34         // Left motor direction pin 2
#define INA1B 30         // Right motor direction pin 1
#define INA2B 36         // Right motor direction pin 2

// ==================================================
// SENSOR CHANNELS (TCA9548A I2C Multiplexer)
// Each RGB sensor is plugged into a separate mux channel
// ==================================================
#define LEFT_SENSOR_CHANNEL    0
#define CENTER_SENSOR_CHANNEL  1
#define RIGHT_SENSOR_CHANNEL   2

#define rgbLED 48        // Turns sensor LEDs ON/OFF

// ==================================================
// SPEED SETTINGS
// PWM values from 0-255
// Higher number = faster motor speed
// ==================================================
#define PWM_FORWARD    75   // Normal forward speed
#define PWM_TURN       85   // Sharp turning speed
#define PWM_SOFTTURN   55   // Gentle turning speed
#define PWM_REVERSE    70   // Reverse speed

// ==================================================
// BLACK LINE DETECTION THRESHOLD
// Lower sensor reading than this = black line detected
// ==================================================
int threshold = 20;

// ==================================================
// Create sensor object
// 24ms integration = slower but better light reading
// ==================================================
Adafruit_TCS34725 tcs =
Adafruit_TCS34725(
TCS34725_INTEGRATIONTIME_24MS,
TCS34725_GAIN_1X
);

// ==================================================
// SELECT TCA9548A CHANNEL
// Switches which sensor is active
// ==================================================
void tcaSelect(uint8_t channel)
{
  Wire.beginTransmission(0x70); // TCA9548A default address
  Wire.write(1 << channel);     // Enable selected channel
  Wire.endTransmission();
}

// ==================================================
// CHECK IF SENSOR SEES BLACK LINE
// Returns true if dark surface detected
// ==================================================
bool blackDetected(uint8_t channel)
{
  tcaSelect(channel);   // Select requested sensor
  delay(20);           // Wait for channel switch

  tcs.begin();         // Reinitialize sensor on that channel

  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c); // Read sensor values

  Serial.println(c);   // Print brightness value for debugging

  return (c < threshold); // If darker than threshold = black line
}

// ==================================================
// READ BRIGHTNESS VALUE ONLY
// Returns CLEAR channel value
// Used for testing/calibration
// ==================================================
uint16_t readClear(uint8_t channel)
{
  tcaSelect(channel);
  delay(15);

  tcs.begin();
  delay(5);

  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  return c; // Return brightness value
}

// ==================================================
// SETUP
// Runs once when robot powers on
// ==================================================
void setup()
{
  Serial.begin(9600); // Open serial monitor
  Wire.begin();       // Start I2C communication

  // Set LED control pin as output
  pinMode(rgbLED, OUTPUT);

  // Turn sensor LEDs ON
  digitalWrite(rgbLED, HIGH);

  // Start by selecting left sensor
  tcaSelect(0);

  // Check if sensor responds
  if (!tcs.begin())
    Serial.println("No sensor");
  else
    Serial.println("Sensor OK");
}

// ==================================================
// LOOP
// Runs over and over forever
// Currently prints brightness values from all 3 sensors
// ==================================================
void loop()
{
  Serial.print("L:");
  Serial.print(readClear(0));   // Left sensor brightness

  Serial.print(" C:");
  Serial.print(readClear(1));   // Center sensor brightness

  Serial.print(" R:");
  Serial.println(readClear(2)); // Right sensor brightness

  delay(500); // Update twice per second
}

// ==================================================
// MOTOR FUNCTIONS
// ==================================================

// Move robot forward
void Forward()
{
  analogWrite(MotorPWM_A, PWM_FORWARD);
  analogWrite(MotorPWM_B, PWM_FORWARD);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

// Move robot backward
void Reverse()
{
  analogWrite(MotorPWM_A, PWM_REVERSE);
  analogWrite(MotorPWM_B, PWM_REVERSE);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

// Turn robot left sharply
void Left()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

// Turn robot right sharply
void Right()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

// Gentle left turn while moving forward
void LeftSoft()
{
  analogWrite(MotorPWM_A, PWM_SOFTTURN);
  analogWrite(MotorPWM_B, PWM_TURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

// Gentle right turn while moving forward
void RightSoft()
{
  analogWrite(MotorPWM_A, PWM_TURN);
  analogWrite(MotorPWM_B, PWM_SOFTTURN);

  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

// Stop both motors
void Stop()
{
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, LOW);
}