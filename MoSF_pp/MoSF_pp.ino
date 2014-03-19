#include <Adafruit_NeoPixel.h>

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
  4, 17, 20, 22, 24 };

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

Adafruit_NeoPixel colors[CONTROLS] = 
{
  Adafruit_NeoPixel(numLEDs[0], 0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[1], 0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[2], 0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[3], 0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(numLEDs[4], 0, NEO_GRB + NEO_KHZ800)};



uint8_t on[CONTROLS];
unsigned long lastUpdate;

void p(char *fmt, ... ){
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < CONTROLS; i++) {
    pinMode(inPin[i], INPUT);
    on[i] = 0;
    level[i] = 0.0f;
    strip[i].begin();
    strip[i].show();
    if (digitalRead(inPin[i]))
      p("%s on\n", name[i]);
  }
  lastUpdate = millis();
}


void showStrips() {
  for(int i = 0; i < CONTROLS; i++) {
    int brightness = (brightnessOn[i] - brightnessOff[i]) * level[i] + brightnessOff[i];
    strip[i].setBrightness(brightness);
    for(int j = 0; j < numLEDs[i]; j++) 
      strip[i].setPixelColor(j, colors[i].getPixelColor(j));
  }
  for(int i = 0; i < CONTROLS; i++) 
    strip[i].show();
}




void loop() {
  uint8_t value;
  unsigned long ms = millis();
  float delta = (float)(ms - lastUpdate);

  for(int i = 0; i < CONTROLS; i++) {
    uint8_t value = digitalRead(inPin[i]);
    if (on[i] != value) {
      on[i] = value;
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

}







