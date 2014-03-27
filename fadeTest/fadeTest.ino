#include <Adafruit_NeoPixel.h>
#include <hsv2rgb.h>
#include "RNLightsNeoPixel.h"

#define PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

RNLightsNeoPixel lights = RNLightsNeoPixel(strip);
int16_t pos = 0;
CHSV hsv;
CRGB rgb;
void setup() {
  strip.begin();
  hsv.s = 255;
  hsv.v = 255;
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  
  unsigned long ms = millis();
  hsv.h = (ms/10)%256;
  hsv2rgb_rainbow(hsv,rgb);
  lights.fadeMultiply(220);
  lights.setPixelColor(pos, rgb.r, rgb.g, rgb.b);
  pos = lights.normalize(pos+1);
  int b = (ms/10) % 512;
  if (b <= 256)
    lights.setBrightness(b);
  else
    lights.setBrightness(512-b);
  lights.show();
  
  delay(1000 / 24);
}


