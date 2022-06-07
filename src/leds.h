#pragma once

#include <Adafruit_NeoPixel.h>

#define PIN 4
#define LEDS 300

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);


// Color buffer
byte color_r[LEDS];
byte color_g[LEDS];
byte color_b[LEDS];


void writeOut() {
  for (int i = 0; i < LEDS; ++i) {
    leds.setPixelColor(i, color_r[i], color_g[i], color_b[i]);
  }
  leds.show();
}

void allSet(const byte r, const byte g, const byte b) {
  memset(color_r, r, sizeof(color_r));
  memset(color_g, g, sizeof(color_g));
  memset(color_b, b, sizeof(color_b));
}

void introSet()
{
  for (int i = -6; i < LEDS; i += 3)
  {
    // leds.setPixelColor(i-1, 0, 0, 0);
    leds.setPixelColor(i + 0, 255, 0, 0);
    leds.setPixelColor(i + 1, 0, 255, 0);
    leds.setPixelColor(i + 2, 0, 0, 255);
    leds.setPixelColor(i + 3, 255, 225, 0);
    leds.setPixelColor(i + 4, 255, 0, 255);
    leds.setPixelColor(i + 5, 0, 255, 255);
    leds.setPixelColor(i + 6, 255, 255, 255);
    leds.show();
    delay(15);
  }
}

void reset()
{
  allSet(0, 0, 0);
  leds.show();
}


void introSetv2()
{
  introSet();
  reset();
  allSet(255, 0, 0);
  writeOut();
  delay(100);
  allSet(0, 255, 0);
  writeOut();
  delay(100);
  allSet(0, 0, 255);
  writeOut();
  delay(100);
  allSet(255, 255, 0);
  writeOut();
  delay(100);
  allSet(255, 0, 255);
  writeOut();
  delay(100);
  allSet(0, 255, 255);
  writeOut();
  delay(100);
  allSet(128, 128, 128);
  writeOut();
  delay(100);
  reset();
}