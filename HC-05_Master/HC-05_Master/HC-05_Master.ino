#include <Adafruit_NeoPixel.h>
//VCC -> 3.3v
//GND -> GND
//RX(5) -> HC-05 TX
//TX(4) -> HC-05 RX
#define RX_PIN 5
#define TX_PIN 4
#define NUM_LEDS 2
#define LED_PIN 18
#define NUM_LEDS 2

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
}

void loop() {
  buttonState = digitalRead(Button);
  Serial.println(buttonState);

  delay(500);
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  if (buttonState == LOW && buttonFlag == false) {
    // turn LED on:
    Serial2.println("A");
    buttonFlag = true;
  } else if (buttonState == LOW && buttonFlag == true) {
    // turn LED off:
    Serial2.println("R");
    buttonFlag = false;
  }
}
