
//VCC -> 3.3v 
//GND -> GND 
//RX(4) -> HC-05 TX 
//TX(5) -> HC-05 RX 

void setup() {

  Serial.begin(9600);

  Serial2.setRX(5);
  Serial2.setTX(4);

  Serial2.begin(9600);
}

void loop() {
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}
