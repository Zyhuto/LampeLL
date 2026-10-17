#include "serial_interface.h"

#include <cstdarg>
#include <cstdio>

namespace AppSerial {

Interface serial;

Interface::Interface() = default;

void Interface::add(OutputChannel &channel) {
    if (firstChannel == nullptr) {
        firstChannel = &channel;
        return;
    }

    OutputChannel *lastChannel = firstChannel;
    while (lastChannel->nextChannel() != nullptr) {
        lastChannel = lastChannel->nextChannel();
    }

    lastChannel->next = &channel;
}

void Interface::begin() {
    for (OutputChannel *channel = firstChannel; channel != nullptr;
         channel = channel->nextChannel()) {
        channel->begin();
    }
}

size_t Interface::write(uint8_t value) {
    size_t written = 0;

    for (OutputChannel *channel = firstChannel; channel != nullptr;
         channel = channel->nextChannel()) {
        const size_t channelWritten = channel->write(value);
        if (channelWritten > written) {
            written = channelWritten;
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
