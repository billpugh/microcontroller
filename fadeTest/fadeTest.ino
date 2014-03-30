#include <Adafruit_NeoPixel.h>
#include <hsv2rgb.h>
#include "RNLightsNeoPixel.h"

#define PIN 2

void p(char *fmt, ... ){
  char tmp[256]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 256, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

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
  Serial.begin(115200);
  Serial.println("Starting");
  hsv.s = 255;
  hsv.v = 255;
  delay(5000);
  Serial.println("Go");
  
}

unsigned long lastUpdate = 0;
void loop() {

  unsigned long ms = millis();
  hsv.h = (ms/10)%256;
  hsv2rgb_rainbow(hsv,rgb);
  lights.fadeMultiply(220);
  lights.setPixelColor(pos, rgb.r, rgb.g, rgb.b);
  pos = lights.normalize(pos+1);
  if (0) {
    int b = (ms/10) % 512;
    if (b <= 256)
      lights.setBrightness(b);
    else
      lights.setBrightness(512-b);
  }
  unsigned long time = lights.show();
  if (lastUpdate + 5000 < ms) {
    p("%d usecs to copy\n", time);
    lastUpdate = ms;
  }

  delay(1000 / 24);
}



