#include <Arduino.h>
#include "led_strip.h"
#include "rfid.h"
#include "serial_interface.h"
#include "wifi_manager.h"

void setup() {
    AppSerial::serial.begin();
    delay(1000);

    AppSerial::serial.println("ESP32 gestartet!");

    WifiManager::begin();

    LedStrip::begin();
    Rfid::begin();
}

void loop() {
    WifiManager::maintainConnection();

    if (Rfid::poll()) {
        LedStrip::toggle();
    }
}
