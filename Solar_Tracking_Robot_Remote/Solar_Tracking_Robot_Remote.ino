#include "Wire.h"
#include <MPU6050_light.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 28
#define NUM_LEDS 1

#define Min_Angle 30
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

char previousCommand = '\0'; // Previous command sent to the control

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
  delay(50);
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

    prevPowerState = powerState; // Update the previous power state
  }

  if (carOn) {
    pixels.setBrightness(20);

    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Set pixel color to green (RGB)
    }

    pixels.show(); // Update the strip

    int angleX = mpu.getAccAngleX();
    int angleY = mpu.getAccAngleY();
    
    char commandToSend = '\0'; // Command to be sent to the control

    if (angleY < -Min_Angle) {
      commandToSend = 'B'; // Send backward command
    } else if (angleY > Min_Angle) {
      commandToSend = 'F'; // Send forward command
    } else if (angleX > Min_Angle) {
      commandToSend = 'R'; // Send right command
    } else if (angleX < -Min_Angle) {
      commandToSend = 'L'; // Send left command
    } else {
      commandToSend = 'S'; // Send stop command
    }

    if (commandToSend != previousCommand) {
      Serial2.write(commandToSend); // Send the command to the control
      Serial.println(commandToSend);
      previousCommand = commandToSend; // Update the previous command
    }
  } else {
    if (previousCommand != 'O') {
      Serial2.write('O'); // Send off command
      Serial.println('O');
      previousCommand = 'O'; // Update the previous command
    }

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
    if (previousCommand != '0') {
      Serial2.write('0'); // Send off command
      Serial.println("Car turned OFF");
      previousCommand = '0'; // Update the previous command
    }
  } else {
    carOn = true;
    if (previousCommand != '1') {
      Serial2.write('1'); // Send on command
      Serial.println("Car turned ON");
      previousCommand = '1'; // Update the previous command
    }
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
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Set pixel color to off (RGB)
    }
    pixels.show(); // Update the strip
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Set pixel color to blue (RGB)
    }
    pixels.show(); // Update the strip
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Set pixel color to off (RGB)
    }
    pixels.show(); // Update the strip
  }
  NVIC_SystemReset();
}
