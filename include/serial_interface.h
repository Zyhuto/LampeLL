#pragma once

#include <Arduino.h>

namespace AppSerial {

// Hier kann der aktive Ausgabekanal zentral umgestellt werden.
enum class OutputMode {
    Usb,
    Bluetooth,
    UsbAndBluetooth
};

constexpr OutputMode OUTPUT_MODE = OutputMode::UsbAndBluetooth;
constexpr unsigned long USB_BAUDRATE = 115200;
constexpr const char *BLUETOOTH_NAME = "LampeLL";

class Interface : public Print {
public:
    void begin();

    using Print::write;

    // Leitet einzelne Zeichen an den gewählten Kanal bzw. die Kanäle weiter.
    size_t write(uint8_t value) override;

    // Entspricht Serial.printf(), bleibt aber Teil der zentralen Schnittstelle.
    size_t printf(const char *format, ...);
};

extern Interface serial;

}
