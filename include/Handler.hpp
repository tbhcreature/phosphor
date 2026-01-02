#pragma once
#include <cstdio>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <type_traits>
#include <format>
#include <string>
#include <atomic>

namespace phosphor::log {

enum class LOG_LEVEL {
    INFO = 0,
    WARN,
    ERROR,
    DEBUG,
    NONE
};

inline std::atomic<LOG_LEVEL> logLevel{LOG_LEVEL::INFO};

inline const char* toString(LOG_LEVEL level) {
    switch (level) {
        case LOG_LEVEL::INFO: return "INFO";
        case LOG_LEVEL::WARN: return "WARN";
        case LOG_LEVEL::ERROR: return "ERROR";
        case LOG_LEVEL::DEBUG: return "DEBUG";
        default: return "NONE"; 
    }
}

[[nodiscard]] inline const char* getTime() {
    thread_local char buf[9];
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = {0};

#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::snprintf(
        buf,
        sizeof(buf),
        "%02d:%02d:%02d",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );
    return buf;
}

class core {
public:
    static void logInt(int val);
    static void logHex(uintptr_t val);
    static void logCstr(const char* str);

    inline void formatImpl(const char* fmt) {
        while (*fmt)
            std::fputc(*fmt++, stderr);
    }

    template<typename... Args>
    void formatImpl(const char* fmt, Args&&... args);
    template<typename... Args>
    void logFmt(LOG_LEVEL level, const char* fmt, Args... args);
};

namespace detail {

inline void dispatchLog(char spec, int val) {
    switch (spec) {
        case 'd': core::logInt(val); break;
        default: std::fputc('?', stderr); break;
    }
}

inline void dispatchLog(char spec, const char* val) {
    switch (spec) {
        case 's': core::logCstr(val); break;
        case 'p':
            core::logHex(reinterpret_cast<uintptr_t>(val));
            break;
        default: std::fputc('?', stderr); break;
    }
}

template<typename T>
void dispatchLog(char spec, T* val) {
    switch (spec) {
        case 'p':
        case 'x':
            core::logHex(reinterpret_cast<uintptr_t>(val));
            break;
        default: std::fputc('?', stderr); break;
    }
}

template <size_t I, typename Tuple>
void dispatchTupleImpl(const Tuple& tuple, size_t index, char spec, bool& dispatched);

template <typename Tuple>
void dispatchTuple(const Tuple& tuple, size_t index, char spec);

} // namespace detail   

} // namespace phosphor::log

#include "Template.hpp"