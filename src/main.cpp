#include <Arduino.h>
#include "rfid.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("ESP32 gestartet!");

    Rfid::begin();
}

void loop() {
    Rfid::poll();
}
