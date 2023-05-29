#include <Adafruit_NeoPixel.h>

#define LED_PIN 18
#define NUM_LEDS 2

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show(); // Initialize all pixels to off
  pixels.setBrightness(20);
}

void loop() {
  // Example: Fill the strip with red color
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Set pixel color to red (RGB)
    
  }
pixels.show(); // Update the strip
    delay(100); // Delay between pixels
  // Example: Turn off all LEDs
  pixels.clear();
  pixels.show();
  delay(2000); // Delay before repeating the loop
}
