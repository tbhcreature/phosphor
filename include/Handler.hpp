#pragma once
#include <cstdio>
#include <cstdint>
#include <type_traits>

namespace log {

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

class logarithm {
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
    if (spec == 'd') logarithm::logInt(val);
    else std::fputc('?', stdout);
}

inline void dispatchLog(char spec, const char* val) {
    if (spec == 's') logarithm::logCstr(val);
    else if (spec == 'p')
        logarithm::logHex(reinterpret_cast<uintptr_t>(val));
    else std::fputc('?', stdout);

}

template<typename T>
void dispatchLog(char spec, T* val) {
    if (spec == 'p' || spec == 'x')
        logarithm::logHex(reinterpret_cast<uintptr_t>(val));
    else std::fputc('?', stdout);
}

} // namespace detail   

} // namespace log

#include "Template.hpp"