#include "SPI.h"
#include <FastLED.h>

#include "data.h"

// WS2801 setup
#define DATA_PIN 4   // Yellow wire / SDA
#define CLOCK_PIN 5  // Green wire / SCL

#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void lights(CRGB color, uint8_t intensity) {
  for (size_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;

    leds[i] %= intensity;
  }

  FastLED.show();
}

void colorTurnOn(uint32_t color) {
  int loopDelay = 100;
  int colorDelay = 2000;
  int maxIntensity = 10;
  int minIntensity = 1;

  for (int intensity = minIntensity; intensity <= maxIntensity; intensity++) {
    lights(color, intensity);
    delay(loopDelay);
  }

  for (int intensity = maxIntensity; intensity >= minIntensity; intensity--) {
    lights(color, intensity);
    delay(loopDelay);
  }

  delay(colorDelay);
}

void loop() {
  colorTurnOn(CRGB::Red);
  colorTurnOn(CRGB::Blue);
  colorTurnOn(CRGB::Green);
  colorTurnOn(CRGB::HotPink);
}
