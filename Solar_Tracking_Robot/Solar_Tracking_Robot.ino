#include "Wire.h"
#include <MPU6050_light.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 28
#define NUM_LEDS 1

MPU6050 mpu(Wire1);
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define RX_PIN 5
#define TX_PIN 4

const int powerButton = 20;
int powerState = HIGH;
int prevPowerState = HIGH;
boolean carOn = false;

unsigned long buttonPressTime = 0;
const unsigned long longPressDuration = 500; // Adjust the long-press duration as needed (in milliseconds)

void setup() {

  Serial2.setRX(RX_PIN);
  Serial2.setTX(TX_PIN);

  Serial2.begin(9600);

  Wire1.setSDA(26);
  Wire1.setSCL(27);
  Serial.begin(9600);
  Wire1.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("Done!\n");

  pinMode(powerButton, INPUT_PULLUP);

  pixels.begin();
  pixels.show(); // Initialize all pixels to off
}

void loop() {
  mpu.update();

  powerState = digitalRead(powerButton);

  if (powerState != prevPowerState) {
    delay(20);

    if (powerState == LOW) {
      buttonPressTime = millis(); // Record button press time
    } else {
      unsigned long buttonReleaseTime = millis();
      unsigned long buttonPressDuration = buttonReleaseTime - buttonPressTime;

      if (buttonPressDuration < longPressDuration) {
        toggleCarPower(); // Perform short press action
      } else {
        performLongPressAction(); // Perform long press action
      }
    }

    prevPowerState = powerState;
  }

  if (carOn) {
    pixels.setBrightness(20);

    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Set pixel color to red (RGB)
    }

    pixels.show(); // Update the strip

    int angleX = mpu.getAccAngleX();
    int angleY = mpu.getAccAngleY();

    if (angleY < -30) {
      Serial2.write('B'); // Send backward command
      Serial.println('B'); //
    } else if (angleY > 30) {
      Serial2.write('F'); // Send fowars command
      Serial.println('F');
    } else if (angleX > 30) {
      Serial2.write('R'); // Send right command
      Serial.println('R');
    } else if (angleX < -30) {
      Serial2.write('L'); // Send left command
      Serial.println('L');
    } else {
      Serial2.write('S'); // Send stop command
      Serial.println('S');
    }
  } else {
    Serial2.write('O'); // Send off command
    Serial.println('O');

    pixels.setBrightness(50);

    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Set pixel color to red (RGB)
    }
    pixels.show(); // Update the strip
  }
}

void toggleCarPower() {
  if (carOn) {
    carOn = false;
    Serial2.write('0'); // Send off command
    Serial.println("Car turned OFF");
  } else {
    carOn = true;
    Serial2.write('1'); // Send on command
    Serial.println("Car turned ON");
  }
}

void performLongPressAction() {
  for (int i = 0; i < 3; i++) {
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Set pixel color to red (RGB)
    }
    pixels.show(); // Update the strip
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Set pixel color to red (RGB)
    }
    pixels.show(); // Update the strip
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Set pixel color to red (RGB)
    }
    pixels.show(); // Update the strip
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Set pixel color to red (RGB)
    }
    pixels.show(); // Update the strip
  }
  NVIC_SystemReset();
}
