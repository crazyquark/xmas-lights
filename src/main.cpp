#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// WebServer stuff
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "data.h"
// WiFi config
#include "config.h"

// WS2801 setup
#define CLOCK_PIN D1  // Green wire / SCL
#define DATA_PIN  D2  // Yellow wire / SDA

#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

void setup() {
  // Config LEDs
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  // Start serial
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
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
