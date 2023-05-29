#include <Adafruit_NeoPixel.h>
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

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);

  Serial2.setRX(RX_PIN);
  Serial2.setTX(TX_PIN);

  Serial2.begin(9600);

  pinMode(Button, INPUT_PULLUP);

  pixels.begin();
  pixels.show(); // Initialize all pixels to off
  pixels.setBrightness(20);
}

void loop() {

  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    char val = Serial2.read();

    switch (val)
    {
      case 'F':
        // Perform forward operation
        Serial.println("Car moving forward");

        for (int i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        }
        pixels.show();
        delay(100);

        pixels.clear();
        pixels.show();

        break;

      case 'B':
        // Perform backward operation
        Serial.println("Car moving backward");

        for (int i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        }
        pixels.show();
        delay(100);

        pixels.clear();
        pixels.show();

        break;

      case 'L':
        // Perform left operation
        Serial.println("Car turning left");

        pixels.setPixelColor(0, pixels.Color(0, 0, 255));

        pixels.show();
        delay(100);

        pixels.clear();
        pixels.show();

        break;

      case 'R':
        // Perform right operation
        Serial.println("Car turning right");

        pixels.setPixelColor(1, pixels.Color(0, 0, 255));

        pixels.show();
        delay(100);

        pixels.clear();
        pixels.show();

        break;

      case 'S':
        // Perform stop operation
        Serial.println("Car stopped");

        for (int i = 0; i < NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 10, 10));
        }
        pixels.show();
        delay(100);

        pixels.clear();
        pixels.show();

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

        break;
    }
  }
}
