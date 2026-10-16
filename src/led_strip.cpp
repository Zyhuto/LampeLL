#include "led_strip.h"

#include <Arduino.h>
#include <FastLED.h>
#include "serial_interface.h"


namespace {

// Anpassen, falls der Data-Eingang des Stripes an einem anderen GPIO liegt.
constexpr uint8_t DATA_PIN = 26;
// Anzahl der LEDs im verwendeten WS2812B-Stripe.
constexpr uint16_t LED_COUNT = 60;
constexpr uint8_t BRIGHTNESS = 128;

CRGB strip[LED_COUNT];
bool stripIsOn = false;

void applyState() {
    const CRGB color = stripIsOn ? CRGB::White : CRGB::Black;
    AppSerial::serial.printf(
        "applyState(): fill_solid(strip, %u, CRGB(%u, %u, %u))\n",
        LED_COUNT,
        color.r,
        color.g,
        color.b);
    fill_solid(strip, LED_COUNT, color);
    FastLED.show();
}

}

namespace LedStrip {

void begin() {
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(strip, LED_COUNT);
    FastLED.setBrightness(128);
    stripIsOn = false;
    applyState();
    AppSerial::serial.println("WS2812B-Stripe bereit (aus).");
}

void toggle() {
    stripIsOn = !stripIsOn;
    applyState();
    AppSerial::serial.println(stripIsOn ? "Stripe eingeschaltet." : "Stripe ausgeschaltet.");
}

bool isOn() {
    return stripIsOn;
}

}
