#include "usb_output.h"

namespace {

constexpr unsigned long USB_BAUDRATE = 115200;

class UsbOutput final : public AppSerial::OutputChannel {
public:
    void begin() override {
        ::Serial.begin(USB_BAUDRATE);
    }

    size_t write(uint8_t value) override {
        return ::Serial.write(value);
    }
};

}

namespace AppSerial {

OutputChannel &usbOutput() {
    static UsbOutput output;
    return output;
}

}
