#pragma once
#include <cstdio>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <type_traits>
#include <format>
#include <string>

namespace phosphor::log {

enum class LOG_LEVEL {
    INFO = 0,
    WARN,
    ERROR,
    DEBUG,
    NONE
};

inline LOG_LEVEL logLevel = LOG_LEVEL::INFO;

inline const char* toString(LOG_LEVEL level) {
    switch (level) {
        case LOG_LEVEL::INFO: return "INFO";
        case LOG_LEVEL::WARN: return "WARN";
        case LOG_LEVEL::ERROR: return "ERROR";
        case LOG_LEVEL::DEBUG: return "DEBUG";
        default: return "UNKNOWN"; 
    }
}

inline const std::string& getTime() {
    static thread_local char buf[16];
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = {};

#if defined(_WIN32)
    localtime_s(&tm, &time);
#elif defined(__linux__)
    localtime_r(&time, &tm);
#else
    #error i dont know how to get the time on this platform!
#endif

    return std::format(
        "{:02}:{:02}:{:02}",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );
}

class core {
public:
    static void logInt(int val);
    static void logHex(uintptr_t val);
    static void logCstr(const char* str);

    inline void formatImpl(const char* fmt) {
        while (*fmt) {
            std::fputc(*fmt, stdout);
            ++fmt;
        }   
    }

    template<typename T, typename... Args>
    void formatImpl(const char* fmt, T val, Args... args);
    
    template<typename... Args>
    void logFmt(LOG_LEVEL level, const char* fmt, Args... args);
};

namespace detail {

inline void dispatchLog(char spec, int val) {
    switch (spec) {
        case 'd': core::logInt(val); break;
        default: std::fputc('?', stdout); break;
    }
}

inline void dispatchLog(char spec, const char* val) {
    switch (spec) {
        case 's': core::logCstr(val); break;
        case 'p':
            core::logHex(reinterpret_cast<uintptr_t>(val));
            break;
        default: std::fputc('?', stdout); break;
    }
}

template<typename T>
void dispatchLog(char spec, T* val) {
    switch (spec) {
        case 'p':
        case 'x':
            core::logHex(reinterpret_cast<uintptr_t>(val));
            break;
        default: std::fputc('?', stdout); break;
    }
}

} // namespace detail   

} // namespace phosphor::log

#include "Template.hpp"