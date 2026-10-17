#include <Arduino.h>
#include "led_strip.h"
#include "rfid.h"
#include "serial_interface.h"
#include "usb_output.h"
#if APP_SERIAL_ENABLE_BLUETOOTH
#include "bluetooth_output.h"
#endif
#include "wifi_manager.h"

void configureSerialOutputs() {
    AppSerial::serial.add(AppSerial::usbOutput());

#if APP_SERIAL_ENABLE_BLUETOOTH
    AppSerial::serial.add(AppSerial::bluetoothOutput());
#endif
}

void setup() {
    configureSerialOutputs();
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
