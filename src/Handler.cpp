#pragma once
#include "Handler.hpp"

namespace phosphor::log {

void core::logInt(int val) {
    if (val < 0) val = -val;
    std::int64_t div = 1'000'000'000;
    bool started = false;

    for (int i = 0; i < 10; ++i) {
        int digit = val / div;
        if (digit != 0 || started || div == 1) {
            std::putc('0' + digit, stdout);
            started = true;
        }

        val %= div;
        div /= 10;
    }
}

void core::logHex(uintptr_t val) {
    std::fputs("0x", stdout);
    bool started = false;

    for (int shift = (sizeof(uintptr_t)*8) - 4; shift >= 0; shift -= 4) {
        int nibble = static_cast<int>((val >> shift) & 0xF);
        if (nibble != 0 || started || shift == 0) {
            std::fputc((nibble < 10) ? '0' + nibble : 'a' + nibble - 10, stdout);
            started = true;
        }
    }
}

void core::logCstr(const char* str) {
    while (*str) {
        std::fputc(*str, stdout);
        ++str;
    }
}

} // namespace phosphor::log