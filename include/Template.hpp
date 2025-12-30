#pragma once
#include "Handler.hpp"

namespace log {

template<typename T, typename... Args>
void logarithm::formatImpl(const char* fmt, T val, Args... args) {
    while (*fmt) {
        if (fmt[0] == '{' && fmt[1] == ':' && fmt[3] == '}') {
            char spec = fmt[2];
            detail::dispatchLog(spec, val);

            fmt += 4;
            formatImpl(fmt, args...);
            return;
        }
        std::fputc(*fmt++, stdout);
    }
}

template<typename... Args>
void logarithm::logFmt(LOG_LEVEL level, const char* fmt, Args... args) {
    if (level > logLevel) return;

    printf("[%s]: ", toString(level));
    formatImpl(fmt, args...);
    std::fputc('\n', stdout);
}

} // namespace log