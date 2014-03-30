
#include <Adafruit_NeoPixel.h>
#include "RNLightsNeoPixel.h"

const uint8_t anomalyCollector = 0;
const uint8_t particleCollector = 1;
const uint8_t computer = 2;
const uint8_t engine = 3;
const uint8_t fuelTank = 4;

const uint8_t CONTROLS = 5;

const uint8_t inPin[] = {
  1, 3, 5, 7, 7};
const uint8_t outPin [] = { 
  16, 18, 20, 22, 24 };
const uint8_t numLEDs [] = {
  4, 17, 8, 8, 15 };

float level[CONTROLS];
const uint16_t rampUp [] = {
  2000, 2000, 2000, 2000, 2000};
const uint16_t rampDown [] = {
  500, 500, 500, 500, 500};
const uint8_t brightnessOn [] = {
  255, 255, 255, 255, 50};
const uint8_t brightnessOff [] = {
  40,  40,   0,  60, 20};

const char* name[] = { 
  "anomaly collector", "particle collector", "computer", "engine", "fuel tank" };

Adafruit_NeoPixel strip[CONTROLS] = 
{
  Adafruit_NeoPixel(numLEDs[0], outPin[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[1], outPin[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[2], outPin[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[3], outPin[3], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[4], outPin[4], NEO_GRB + NEO_KHZ800)};

RNLightsNeoPixel lights[CONTROLS] = 
{
  RNLightsNeoPixel(strip[0]),
   RNLightsNeoPixel(strip[1]),
    RNLightsNeoPixel(strip[2]),
     RNLightsNeoPixel(strip[3]),
          RNLightsNeoPixel(strip[5]),
  };



uint8_t on[CONTROLS];
unsigned long lastUpdate;

void p(char *fmt, ... ){
  char tmp[256]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 256, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

void setup() {
  Serial.begin(115200);
  lights[fuelTank].setAllPixelColors(128,0,0);
  lights[engine].setAllPixelColors(0,0,80);
  lights[computer].setAllPixelColors(0,0,80);
  lights[particleCollector].setAllPixelColors(0,200,0);
  lights[anomalyCollector].setAllPixelColors(128,0,64);
  
  for(int i = 0; i < CONTROLS; i++) {
    pinMode(inPin[i], INPUT);
    on[i] = 0;
    level[i] = 0.0f;
    if (digitalRead(inPin[i]))
      p("%s on\n", name[i]);
    lights[i].show();
  }
  
  lastUpdate = millis();
}

void showStrips() {
  for(int i = 0; i < CONTROLS; i++) {
    int brightness = (brightnessOn[i] - brightnessOff[i]) * level[i] + brightnessOff[i];
    lights[i].setBrightness(brightness);
    lights[i].show();
  }
}

uint8_t getInput(uint8_t control) {
   return digitalRead(inPin[control]);
}

void loop() {
  uint8_t value;
  unsigned long ms = millis();
  float delta = (float)(ms - lastUpdate);

  for(int i = 0; i < CONTROLS; i++) {
    uint8_t value = getInput(i);
    if (on[i] != value) {
      on[i] = value;
    } else if (value) {
      level[i] += delta / rampUp[i];
      if (level[i] > 1.0f)
        level[i] = 1.0f;
    } else {
      level[i] -= delta / rampDown[i];
      if (level[i] < 0.0f)
        level[i] = 0.0f;
    }
  }
}







