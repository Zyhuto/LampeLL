#pragma once

#include <Arduino.h>

#include "output_channel.h"

namespace AppSerial {

class Interface : public Print {
public:
    Interface();

    using Print::write;

    void add(OutputChannel &channel);
    void begin();
    size_t write(uint8_t value) override;
    size_t printf(const char *format, ...);

private:
    OutputChannel *firstChannel = nullptr;
};

extern Interface serial;

}
