
#include "RNLightsNeoPixel.h"

RNLightsNeoPixel::RNLightsNeoPixel(Adafruit_NeoPixel & strip) : 
	RNLights(strip.numPixels()), strip(strip) {
  strip.begin();
};

void RNLightsNeoPixel::show() {
 
  if (fullBrightness)
   for(int i = 0; i < getNumPixels(); i++) {
    uint8_t r,g,b;
    getPixelColor(i, r, g, b);
    strip.setPixelColor(i, r, g, b);
  } else {
    uint16_t br = brightness;
    for(int i = 0; i < getNumPixels(); i++) {
    uint8_t r,g,b;
    getPixelColor(i, r, g, b);
    r = ( br * r ) >> 8;
    g = ( br * g ) >> 8;
    b = ( br * b ) >> 8;
    strip.setPixelColor(i, r, g, b);
    }
}

  strip.show();
}
