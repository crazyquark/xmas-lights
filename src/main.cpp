#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// WebServer stuff
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

#include "data.h"
// WiFi config
#include "config.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

static bool pwr = true;

String outputState(int output) {
  switch (output) {
  case 1:
    return pwr ? "checked" : "";
  default:
    break;
  }

  return String();
}

// Replaces placeholder with button section in your web page
String processor(const String &var) {
  // Serial.println(var);
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Power</h4><label class=\"switch\"><input "
               "type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"1\" " +
               outputState(1) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

// WS2801 setup
#define CLOCK_PIN D1 // Green wire / SCL
#define DATA_PIN D2  // Yellow wire / SDA

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

  // Launch SPIFFS file system
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("output")) {
      const auto output = request->getParam("output")->value().toInt();
      const auto state = request->getParam("state")->value().toInt();

      switch (output)
      {
      case 1:
        pwr = state == 1;
        Serial.println("Power" + pwr ? "on" : "off");
        break;

      default:
        break;
      }
    }
  });

  // Start server
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
  if (pwr) {
    colorTurnOn(CRGB::Red);
    colorTurnOn(CRGB::Blue);
    colorTurnOn(CRGB::Green);
    colorTurnOn(CRGB::HotPink);
  }
}
