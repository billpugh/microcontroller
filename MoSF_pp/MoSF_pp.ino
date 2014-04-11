
#include <Adafruit_NeoPixel.h>
#include <hsv2rgb.h>
#include "RNLightsNeoPixel.h"


const uint8_t particleCollector = 0;
const uint8_t leftOrb = 1;
const uint8_t rightOrb = 2;
const uint8_t engine = 3;
const uint8_t fuelTank = 4;

const uint8_t CONTROLS = 5;

// Special in pins:
//  -1 = always on
//  -2 = on if (millis() % 1000) < 500)
//  -3 = on if (millis() % 1000) >= 500)
const int8_t inPin[] = {
  1, -2, -3, 5, 5};
const uint8_t outPin [] = { 
  15, 17, 19, 21, 23 };
const uint8_t numLEDs [] = {
  13, 13, 13, 8, 15 };

float level[CONTROLS];
const uint16_t rampUp [] = {
  1000, 300, 300, 1000, 1000};
const uint16_t rampDown [] = {
  500, 300, 300, 500, 500};
const uint8_t brightnessOn [] = {
  255, 255, 255, 255, 80};
const uint8_t brightnessOff [] = {
  40,  40,   40,  60, 10};

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
  RNLightsNeoPixel(strip[4]),
};



uint8_t on[CONTROLS];
unsigned long lastUpdate;
uint8_t minimumParticleCollector = 40;
unsigned long tick = 0;

void p(char *fmt, ... ){
  char tmp[256]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 256, fmt, args);
  va_end (args);
  Serial.print(tmp);
}
CHSV hsv;
CRGB rgb;

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH);

  Serial.println("Starting v3");
  delay(1000);
  digitalWrite(13, LOW);
  hsv.s = 255;
  hsv.v = 255;
  lights[fuelTank].setAllPixelColors(128,0,0);
  lights[engine].setAllPixelColors(0,0,80);
  lights[leftOrb].setAllPixelColors(0,80,80);
  lights[rightOrb].setAllPixelColors(80,80,0);
  lights[particleCollector].setAllPixelColors(0,minimumParticleCollector,0);
  Serial.println("Set pixel colors");

  for(int i = 0; i < CONTROLS; i++) {
    int8_t pin = inPin[i];
    if (pin >= 0)
      pinMode(pin, INPUT);
    on[i] = 0;
    level[i] = 0.0f;
    if (getInput(i))
      p("%s on\n", name[i]);
    lights[i].show();
    p("%s initialized\n", name[i]);
  }
  delay(5000);
  lastUpdate = millis();
}

float easeInOutQuad(float t) {
  t = t*2;
  if (t < 1)
    return t*t/2;
  t -= 2;
  return 1-t*t*t/2;
}

float easeInOutCubic(float t) {
  t = t*2;
  if (t < 1)
    return t*t*t/2;
  t -= 2;
  return 1+t*t*t/2;
}

void showStrips() {
  for(int i = 0; i < CONTROLS; i++) {
    int brightness = (brightnessOn[i] - brightnessOff[i]) * easeInOutQuad(level[i]) + brightnessOff[i];
    lights[i].setBrightness(brightness);
    lights[i].show();
  }
}

uint8_t getInput(uint8_t control) {
  int8_t pin = inPin[control];
  switch (pin) {
  case -1:
    return true;
  case -2:
    return (millis() % 1000) < 500;
  case -3:
    return (millis() % 1000) >= 500;
  default: 
    return digitalRead(pin);
  }
}


bool collectingParticle = false;
//void updateComputer() {
//  if (tick % 3 != 0)
//    return;
//  Serial.println("Updating computer");
//  for(int i = 0; i < numLEDs[computer]; i++) {
//    p("%3d ", lights[computer].getPixelGreen(i));
//  }
//  lights[computer].rotate(true);
//  lights[computer].fade(10,200);
//  uint16_t pixel = random(numLEDs[computer]);
//  hsv.h = random(256);
//  hsv2rgb_rainbow(hsv,rgb);
//  lights[computer].setPixelColor(pixel, rgb.r, rgb.g, rgb.b);
//}


int engineTick = 0;
void updateEngine() {
  if (level[engine] < 0.3) {
    if (tick % 4 != 0)
      return;
  } 
  else  if (level[engine] < 0.7) {
    if (tick % 3 != 0)
      return;
  } 
  else  if (level[engine] < 0.9) {
    if (tick % 2 != 0)
      return;
  } 
  else if (tick % 3 == 0)
    return;

  int mode = engineTick++;
  lights[engine].shift(true);
  if (mode % 4 == 0) {
    if (on[engine])
      lights[engine].setPixelColor(0,150, 150, 150);
    else
      lights[engine].setPixelColor(0, 0,0, 150);
  } 
  else {
    if (on[engine])
      lights[engine].setPixelColor(0,0, 0, 100);
  }
}



void rotateUpAllButFirst(uint8_t control) {
  uint16_t lastLED = numLEDs[control]-1;
  uint8_t r,g,b;
  lights[control].getPixelColor(lastLED, r, g, b);
  for(int i = lastLED; i > 1 ; i--)
    lights[control].copyPixelColor(i-1,i);
  lights[control].setPixelColor(1, r, g, b);
}

void rotateDownAllButFirst(uint8_t control) {
  uint16_t lastLED = numLEDs[control]-1;
  uint8_t r,g,b;
  lights[control].getPixelColor(1, r, g, b);
  for(int i = 1; i < lastLED; i++)
    lights[control].copyPixelColor(i+1,i);
  lights[control].setPixelColor(lastLED, r, g, b);
}

void updateOrb(uint8_t control) {
  if (control == leftOrb)
    rotateUpAllButFirst(control);
  else
    rotateDownAllButFirst(control);
  uint16_t hue = (random(32) + 256 - 16) % 256;
  uint8_t pixel = random(numLEDs[control]);
  hsv.h = hue;
  hsv2rgb_rainbow(hsv,rgb);
  lights[control].setPixelColor(pixel, rgb.r, rgb.g, rgb.b);
}


void updateParticleCollector() {

  rotateDownAllButFirst(particleCollector);

  if (!collectingParticle && on[particleCollector] && random(8) == 0) {
    collectingParticle = true;
    Serial.println("Starting particle collection");
    for(int i = 0; i < numLEDs[particleCollector]; i++) {
      p("%3d ", lights[particleCollector].getPixelGreen(i));
    }
    Serial.println();
  }
  uint16_t g;
  if (collectingParticle) {
    g = lights[particleCollector].getPixelGreen(0);

    if (on[particleCollector]) {
      if (g == 255) {
        uint8_t p = 1+random(12);
        lights[particleCollector].copyPixelColor(0,p);
        lights[particleCollector].setPixelColor(0, 0, minimumParticleCollector, 0);
        collectingParticle = false;
        Serial.println("finished particle collection");
      } 
      else {
        g = min(255, g + 20);
        lights[particleCollector].setPixelColor(0, 0, g, 0);
      }
    } 
    else {
      g =  max(minimumParticleCollector, g-20);
      lights[particleCollector].setPixelColor(0, 0,g, 0);
      if (g <= minimumParticleCollector)
        collectingParticle = false;
    }
  }
  g = lights[particleCollector].getPixelGreen(0);
  lights[particleCollector].fade(5, minimumParticleCollector);
  lights[particleCollector].setPixelColor(0, 0, g, 0);
}

void loop() {
  uint8_t value;
  unsigned long ms = millis();
  float delta = (float)(ms - lastUpdate);

  for(int i = 0; i < CONTROLS; i++) {
    uint8_t value = getInput(i);
    if (on[i] != value) {
      on[i] = value;
      if (value)
        p("%s on", name[i]);
      else
        p("%s off", name[i]);
    } 
    else if (value) {
      level[i] += delta / rampUp[i];
      if (level[i] > 1.0f)
        level[i] = 1.0f;
    } 
    else {
      level[i] -= delta / rampDown[i];
      if (level[i] < 0.0f)
        level[i] = 0.0f;
    }
  }
  updateParticleCollector();
  updateOrb(leftOrb);
  updateOrb(rightOrb);
  updateEngine();
  showStrips();
  delay(50);
  tick++;
}













