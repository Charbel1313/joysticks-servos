/*
 * Joystick et servomoteurs (Arduino Uno)
 *
 * Deux servomoteurs suivent la position d'un joystick analogique :
 *   - axe X (horizontal) -> servo 1
 *   - axe Y (vertical)   -> servo 2
 * Un buzzer emet un bip court quand le joystick s'ecarte du centre
 * de plus de `marge`.
 *
 * Cablage du joystick (5 broches) :
 *   VCC -> 5V        GND -> GND
 *   HORZ (VRx) -> A0
 *   VERT (VRy) -> A1
 *   SEL  (bouton) -> broche 2 (lu pour l'instant : pas encore utilise)
 */

#include <Arduino.h>
#include <Servo.h>

const uint8_t Xpin = A0;       // axe horizontal du joystick
const uint8_t Ypin = A1;       // axe vertical du joystick
const uint8_t Spin = 2;        // bouton SEL du joystick
const uint8_t servoPin1 = 5;   // servo 1 (suit l'axe X)
const uint8_t servoPin2 = 3;   // servo 2 (suit l'axe Y)
const uint8_t buzzerPin = 8;

const int centerX = 512;       // valeur au repos du joystick
const int centerY = 512;
const int marge = 50;          // ecart au centre a partir duquel le buzzer sonne
const int delayTime = 200;     // pause entre deux mesures (ms)

Servo motor1;
Servo motor2;

void setup() {
  Serial.begin(9600);
  pinMode(Spin, INPUT_PULLUP);  
  motor1.attach(servoPin1);     
  motor2.attach(servoPin2);
  // A remarquer aussi que
  // Xpin/Ypin : les entrees analogiques n'ont pas besoin de pinMode
  // buzzerPin : tone() configure la broche lui-meme
}

void loop() {
  int Xvalue = analogRead(Xpin);   // 0..1023
  int Yvalue = analogRead(Ypin);

  // convertit 0..1023 en angle 0..180 degres
  motor1.write(map(Xvalue, 0, 1023, 0, 180));
  motor2.write(map(Yvalue, 0, 1023, 0, 180));

  // le buzzer sonne si l'un des deux axes s'ecarte du centre de plus de `marge`
  bool joystickUsed = abs(Xvalue - centerX) > marge || abs(Yvalue - centerY) > marge;
  if (joystickUsed) {
    tone(buzzerPin, 1000, 50);     // 1000 Hz pendant 50 ms
  }

  Serial.print("Xvalue = ");
  Serial.println(Xvalue);
  Serial.print("Yvalue = ");
  Serial.println(Yvalue);
  delay(delayTime);
}