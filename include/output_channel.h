#pragma once

#include <Arduino.h>

#ifndef APP_SERIAL_ENABLE_BLUETOOTH
#define APP_SERIAL_ENABLE_BLUETOOTH 0
#endif

namespace AppSerial {

class OutputChannel {
public:
    virtual ~OutputChannel() = default;
    virtual void begin() = 0;
    virtual size_t write(uint8_t value) = 0;

    OutputChannel *nextChannel() const {
        return next;
    }

private:
    friend class Interface;
    OutputChannel *next = nullptr;
};

}
