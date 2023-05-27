#include "Wire.h"
#include <MPU6050_light.h>
#include <SoftwareSerial.h>

MPU6050 mpu(Wire1);

#define RX_PIN 5
#define TX_PIN 4

const int speedButton = 20;
int speedState = HIGH;
int prevSpeedState = HIGH;
boolean carOn = false;

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

  pinMode(speedButton, INPUT_PULLUP);
}

void loop() {
  mpu.update();

  speedState = digitalRead(speedButton);

  if (speedState != prevSpeedState) {
    delay(50);

    if (speedState == LOW) {
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

    prevSpeedState = speedState;
  }

  if (carOn) {
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
  }
}
