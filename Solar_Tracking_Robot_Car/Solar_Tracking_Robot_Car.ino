#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include "CytronMotorDriver.h"
#include "ADS1X15.h"

//VCC -> 3.3v
//GND -> GND
//RX(5) -> HC-05 TX
//TX(4) -> HC-05 RX
#define RX_PIN 5
#define TX_PIN 4
#define NUM_LEDS 2
#define LED_PIN 18


int Button = 20;

int buttonState = 0;

boolean buttonFlag = false;

char val;

int16_t top_r;
int16_t top_l;
int16_t bot_r;
int16_t bot_l;
int avgtop;
int avgbot;
int avgleft;
int avgright;
int diffelev;
int diffazi;

int THRESHOLD_VALUE = 900;
int SERVO_MIN = 30;
int SERVO_MAX = 160;
int SERVO_SMOOTH = 15;

CytronMD motor1(PWM_PWM, 8, 9);   // PWM 1A = Pin 8, PWM 1B = Pin 9.
CytronMD motor2(PWM_PWM, 10, 11); // PWM 2A = Pin 10, PWM 2B = Pin 11.

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

ADS1115 ADS(0x48, &Wire); //SCL 0, SCA 0 || &Wire1 -> SCL 1, SDA 1

Servo servo_leftright;
Servo servo_updown;

void setup() {

  Serial.begin(9600);

  Serial2.setRX(RX_PIN);
  Serial2.setTX(TX_PIN);

  Serial2.begin(9600);

  pinMode(Button, INPUT_PULLUP);

  //Neopixle LED
  pixels.begin();
  pixels.show(); // Initialize all pixels to off
  pixels.setBrightness(20);
}

void loop() {

  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    val = Serial2.read();
  }

  switch (val)
  {
    case 'F':
      // Perform forward operation
      //  Serial.println("Car moving forward");

      //  for (int i = 0; i < NUM_LEDS; i++) {
      //   pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      //  }
      // pixels.show();
      // delay(100);

      // pixels.clear();
      // pixels.show();

      motor1.setSpeed(-255);
      motor2.setSpeed(255);
      break;

    case 'B':
      // Perform backward operation
      //      Serial.println("Car moving backward");

      //      for (int i = 0; i < NUM_LEDS; i++) {
      //        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      //      }
      //      pixels.show();
      //      delay(100);
      //
      //      pixels.clear();
      //      pixels.show();

      motor1.setSpeed(255);
      motor2.setSpeed(-255);
      break;

    case 'L':
      // Perform left operation
      //      Serial.println("Car turning left");
      //
      //      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
      //
      //      pixels.show();
      //      delay(100);
      //
      //      pixels.clear();
      //      pixels.show();

      motor1.setSpeed(200);
      motor2.setSpeed(200);

      break;

    case 'R':
      // Perform right operation
      //      Serial.println("Car turning right");
      //
      //      pixels.setPixelColor(1, pixels.Color(0, 0, 255));
      //
      //      pixels.show();
      //      delay(100);
      //
      //      pixels.clear();
      //      pixels.show();

      motor1.setSpeed(-200);
      motor2.setSpeed(-200);

      break;

    case 'S':
      // Perform stop operation
      //      Serial.println("Car stopped");
      //
      //      for (int i = 0; i < NUM_LEDS; i++) {
      //        pixels.setPixelColor(i, pixels.Color(255, 10, 10));
      //      }
      //      pixels.show();
      //      delay(100);
      //
      //      pixels.clear();
      //      pixels.show();

      motor1.setSpeed(0);
      motor2.setSpeed(0);

      break;

    case 'O':
      // Perform off operation
      Serial.println("Car turned off");

      for (int i = 0; i < NUM_LEDS; i++) {
        pixels.setPixelColor(i, pixels.Color(191, 39, 191));
      }
      pixels.show();
      delay(100);

      pixels.clear();
      pixels.show();

      motor1.setSpeed(0);
      motor2.setSpeed(0);

      break;
  }
}

// Running on core1
void setup1() {

  //ADS1115
  ADS.begin(0, 1);   // SDA (Pin 0), SCL(Pin 1)
  ADS.setDataRate(7);  // 0 = slow   4 = medium   7 = fast
  ADS.setMode(0);      // continuous mode
  ADS.setGain(1);

  //Servo
  servo_leftright.attach(14);
  servo_updown.attach(15);

  servo_leftright.write(60);
  servo_updown.write(60);
}

void loop1() {
  automaticsolartracker();
}

void automaticsolartracker() {
  top_r = ADS.readADC(0); //top right
  bot_r = ADS.readADC(1); //bot right
  top_l = ADS.readADC(2); //top left
  bot_l = ADS.readADC(3); //bot left

  avgtop = (top_r + top_l) / 2;
  avgbot = (bot_r + bot_l) / 2;
  avgleft = (top_l + bot_l) / 2;
  avgright = (top_r + bot_r) / 2;

  diffelev = avgtop - avgbot;
  diffazi = avgright - avgleft;

  if (abs(diffazi) >= THRESHOLD_VALUE) {
    if (diffazi > 0) {
      if (servo_leftright.read() < SERVO_MAX) {
        servo_leftright.write((servo_leftright.read() + 2));
        delay(SERVO_SMOOTH);
      }
    }
    if (diffazi < 0) {
      if (servo_leftright.read() > SERVO_MIN) {
        servo_leftright.write((servo_leftright.read() - 2));
        delay(SERVO_SMOOTH);
      }
    }
  }

  if (abs(diffelev) >= THRESHOLD_VALUE) {
    if (diffelev > 0) {
      if (servo_updown.read() < SERVO_MAX) {
        servo_updown.write((servo_updown.read() - 2));
        delay(SERVO_SMOOTH);
      }
    }
    if (diffelev < 0) {
      if (servo_updown.read() > SERVO_MIN) {
        servo_updown.write((servo_updown.read() + 2));
        delay(SERVO_SMOOTH);
      }
    }
  }
}
