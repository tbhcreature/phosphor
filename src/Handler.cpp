#pragma once
#include "Handler.hpp"

namespace log {

void logarithm::logInt(int val) {
    if (val < 0) val = -val;
    long long div = 1000000000LL;
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

void logarithm::logHex(uintptr_t val) {
    std::fputs("0x", stdout);
    bool started = false;

    for (int shift = (sizeof(uintptr_t)*8) - 4; shift >= 0; shift -= 4) {
        int nibble = (val >> shift) & 0xF;
        if (nibble != 0 || started || shift == 0) {
            char ch = (nibble < 10) ? ('0' + nibble) : ('a' + (nibble - 10));
            std::fputc(ch, stdout);
            started = true;
        }
    }
}

void logarithm::logCstr(const char* str) {
    while (*str) {
        std::fputc(*str, stdout);
        ++str;
    }
}

// templated functions moved to Template.hpp
// templated functions moved to Template.hpp
// see Api.cpp for user usage

} // namespace log