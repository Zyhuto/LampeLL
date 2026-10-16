#include <Arduino.h>
#include "led_strip.h"
#include "rfid.h"
#include "serial_interface.h"

void setup() {
    AppSerial::serial.begin();
    delay(1000);

    AppSerial::serial.println("ESP32 gestartet!");

    LedStrip::begin();
    Rfid::begin();
}

void loop() {
    if (Rfid::poll()) {
        LedStrip::toggle();
    }
}
