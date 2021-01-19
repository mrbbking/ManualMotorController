// Started from Adafruit "Servo" example.
// Changed a bit.

#include <Servo.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

Servo servo;
Servo motor;

int servoPin = 9;
int motorPin = 10;
int potPin   = 0;  
int speed    = 0;  // 0 - 100 percent, either direction
int dir      = 0;  // 1 = reverse, 0 = forward
bool enabled = false;  // no signal out until pot goes to center (stopped)

// for updating the serial monitor every second during dev.
unsigned long previousMillis = 0;
const long updateInterval = 1000;

void setup() 
{
  #ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
  #endif
  
  servo.attach(servoPin);
  motor.attach(motorPin);
  
  matrix.begin(0x70);
  matrix.print(4611);
  matrix.writeDisplay();
  
  Serial.begin(9600);

  Serial.println("Center the speed knob to begin...");
} 
 
void loop() 
{ 
  unsigned long currentMillis = millis();
  
  int reading = analogRead(potPin);           // analogRead returns 0 to 1023
  
  do{
    reading = analogRead(potPin);
    if( reading == 512 ){
      enabled = true;
    }
  } while (enabled == false);

  int ms = map(reading, 0, 1023, 1000, 2000); // map analogRead to a range of 1000 to 2000 microseconds
  int angle = map(reading, 0, 1023, 0, 180);  // ...and to angles, for a servo, for testing.
  //servo.write(angle);                         // rotate servo
  motor.writeMicroseconds(ms);                // spin motor

  if( ms >= 1475 && ms <= 1525 )              // center, per Spark Max manual
  {
    speed = 0;
  }
  if( ms < 1475 )                             // reverse, per Spark Max manual
  {
    speed = 0 - map( ms, 1000, 1475, 99, 0 ); // map ms to percentage
    dir = 1;
  }

  if( ms > 1525 )                             // forward, per Spark Max manual
  {
    speed = map(ms, 1525, 2000, 0, 99 );      // map ms to percentage
    dir = 0;
  }

  matrix.print(speed);
  matrix.writeDisplay();
 
  if( currentMillis - previousMillis >= updateInterval)
  {
    Serial.print("\n\nDirection: ");
    if( dir ){
      Serial.println("Forward");
    } else {
      Serial.println("Reverse");
    }
    
    Serial.print("ms   : ");
    Serial.println(String(ms));
    Serial.print("Speed: ");
    Serial.println(String(speed));

    Serial.print("reading: ");
    Serial.println(String(reading));
    Serial.print("angle  : ");
    Serial.println(String(angle));

    previousMillis = currentMillis;
  }
}
