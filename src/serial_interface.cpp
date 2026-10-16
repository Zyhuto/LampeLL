#include "serial_interface.h"

#include <BluetoothSerial.h>
#include <cstdarg>
#include <cstdio>

namespace {

BluetoothSerial bluetoothSerial;

bool useUsb() {
    return AppSerial::OUTPUT_MODE == AppSerial::OutputMode::Usb ||
           AppSerial::OUTPUT_MODE == AppSerial::OutputMode::UsbAndBluetooth;
}

bool useBluetooth() {
    return AppSerial::OUTPUT_MODE == AppSerial::OutputMode::Bluetooth ||
           AppSerial::OUTPUT_MODE == AppSerial::OutputMode::UsbAndBluetooth;
}

}

namespace AppSerial {

Interface serial;

void Interface::begin() {
    if (useUsb()) {
        ::Serial.begin(USB_BAUDRATE);
    }

    if (useBluetooth()) {
        bluetoothSerial.begin(BLUETOOTH_NAME);
    }
}

size_t Interface::write(uint8_t value) {
    size_t written = 0;

    if (useUsb()) {
        written = ::Serial.write(value);
    }

    if (useBluetooth()) {
        bluetoothSerial.write(value);
        if (!useUsb()) {
            written = 1;
        }
    }

    return written;
}

size_t Interface::printf(const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    const int length = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (length <= 0) {
        return 0;
    }

    const size_t outputLength = static_cast<size_t>(length) < sizeof(buffer)
                                    ? static_cast<size_t>(length)
                                    : sizeof(buffer) - 1;
    return write(reinterpret_cast<const uint8_t *>(buffer), outputLength);
}

}
