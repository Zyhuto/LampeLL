#include "bluetooth_output.h"

#if APP_SERIAL_ENABLE_BLUETOOTH

#include <BluetoothSerial.h>

namespace {

constexpr char BLUETOOTH_NAME[] = "LampeLL";

class BluetoothOutput final : public AppSerial::OutputChannel {
public:
    void begin() override {
        bluetoothSerial.begin(BLUETOOTH_NAME);
    }

    size_t write(uint8_t value) override {
        return bluetoothSerial.write(value);
    }

private:
    BluetoothSerial bluetoothSerial;
};

}

namespace AppSerial {

OutputChannel &bluetoothOutput() {
    static BluetoothOutput output;
    return output;
}

}

#endif
