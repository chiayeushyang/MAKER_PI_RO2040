#include <Adafruit_NeoPixel.h>
//VCC -> 3.3v
//GND -> GND
//RX(5) -> HC-05 TX
//TX(4) -> HC-05 RX
#define RX_PIN 5
#define TX_PIN 4

#define LED_PIN 18
#define NUM_LEDS 2

int Button = 20;

int buttonState = 0;
int prevButtonState = HIGH; // Previous button state

unsigned long pressTime = 0; // Time of button press
boolean buttonFlag = false;
boolean ledState = false; // LED state

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

  if (buttonState != prevButtonState) {
    // Button state changed
    delay(50); // Debounce delay

    if (buttonState == LOW) {
      // Button pressed
      pressTime = millis(); // Record press time
      buttonFlag = true;
    } else {
      // Button released
      if (buttonFlag) {
        unsigned long releaseTime = millis(); // Record release time

        if (releaseTime - pressTime < 500) {
          // Short press (less than 500ms)
          performFunction1();
        } else {
          // Long press (500ms or more)
          performFunction2();
        }

        buttonFlag = false; // Reset button flag
      }
    }
  }

  prevButtonState = buttonState;

  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}

void performFunction1() {
  // Function to perform on short button press
  if (!ledState) {
    ledState = true;
    Serial2.println("R"); // Turn LED on
  } else {
    ledState = false;
    Serial2.println("G"); // Turn LED off
  }
}

void performFunction2() {
    if (!ledState) {
    ledState = true;
    Serial2.println("R"); // Turn LED on
  } else {
    ledState = false;
    Serial2.println("G"); // Turn LED off
  }
}
