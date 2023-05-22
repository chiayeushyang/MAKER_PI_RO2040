
//VCC -> 3.3v 
//GND -> GND 
//RX(5) -> HC-05 TX 
//TX(4) -> HC-05 RX 

int Button = 20; 

int buttonState = 0;   

boolean buttonFlag = true;

void setup() {

  Serial.begin(9600);

  Serial2.setRX(5);
  Serial2.setTX(4);

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

  if (buttonState == HIGH && buttonFlag == true) {
    // turn LED on:

     buttonFlag = true;
  } else {
    // turn LED off:

    buttonFlag = false;
  }
}
