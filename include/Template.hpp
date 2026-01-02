#pragma once
#include "Handler.hpp"

namespace phosphor::log {

template<typename T, typename... Args>
void core::formatImpl(const char* fmt, T val, Args... args) {
    while (*fmt) {
        if (
            fmt[0] == '{' &&
            fmt[1] == ':' &&
            fmt[2] &&
            fmt[3] == '}'
        ) {
            detail::dispatchLog(fmt[2], val);
            fmt += 4;
            
            if constexpr (sizeof...(Args) > 0)
                formatImpl(fmt, args...);
            else {
                std::fputs("\n[phosphor]: missing format arguments\n", stderr);
                std::abort();
            }
            return;
        }
        std::fputc(*fmt++, stderr);
    }

    std::fputs("\n[phosphor]: too many arguments\n", stderr);
    std::abort();
}

template<typename... Args>
void core::logFmt(LOG_LEVEL level, const char* fmt, Args... args) {
#if defined(__GNUC__) || defined(__clang__)
    if constexpr (__builtin_constant_p(fmt))
        static_assert(
            detail::ctGetSpecs(fmt) == sizeof...(Args),
            "[phosphor]: format/argument count mismatch"
        );
    else
#endif
    {
        if (detail::rtGetSpecs(fmt) != sizeof...(Args)) {
            std::fputs("[phosphor]: format/argument count mismatch\n", stderr);
            std::abort();
        }
    }

    if (level < logLevel.load(std::memory_order_relaxed))
        return;

    std::fprintf(
        stderr,
        "[%s %s]: ",
        getTime(),
        toString(level)
    );

    formatImpl(fmt, args...);
    std::fputc('\n', stderr);
}

} // namespace phosphor::log