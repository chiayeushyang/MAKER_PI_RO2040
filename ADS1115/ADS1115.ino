
#include "ADS1X15.h"

// choose your sensor
// ADS1013 ADS(0x48);
// ADS1014 ADS(0x48);
// ADS1015 ADS(0x48);
// ADS1113 ADS(0x48);
// ADS1114 ADS(0x48);
ADS1115 ADS(0x48, &Wire); //SCL 0, SCA 0 || &Wire1 -> SCL 1, SDA 1


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin(0, 1);   // SDA (Pin 0), SCL(Pin 1)
  ADS.setDataRate(7);  // 0 = slow   4 = medium   7 = fast
  ADS.setMode(0);      // continuous mode
}


void loop()
{
  ADS.setGain(1);
 
  int16_t val_0 = ADS.readADC(0);  
  int16_t val_1 = ADS.readADC(1);  
  int16_t val_2 = ADS.readADC(2);  
  int16_t val_3 = ADS.readADC(3);  
 
  float f = ADS.toVoltage(2);  // voltage factor
 
  Serial.print("\tAnalog0: "); Serial.print(val_0); Serial.print('\t'); Serial.println(val_0 * f, 3);
  Serial.print("\tAnalog1: "); Serial.print(val_1); Serial.print('\t'); Serial.println(val_1 * f, 3);
  Serial.print("\tAnalog2: "); Serial.print(val_2); Serial.print('\t'); Serial.println(val_2 * f, 3);
  Serial.print("\tAnalog3: "); Serial.print(val_3); Serial.print('\t'); Serial.println(val_3 * f, 3);
  Serial.println();
 
  delay(500);
}
