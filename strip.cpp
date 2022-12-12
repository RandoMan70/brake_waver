#include <Arduino.h>
#include <microLED.h>
#include "strip.hpp"

microLED< STRIP_NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> STRIP;

CStrip::CStrip() {
  STRIP.setBrightness(STRIP_BRIGHTNESS);
}

void CStrip::Set(int index, unsigned char value) {
  STRIP.leds[index] = mRGB(value, 0, 0);
}
int CStrip::Size() {
  return STRIP_NUMLEDS;
}
void CStrip::Show() {
  STRIP.show();
}
