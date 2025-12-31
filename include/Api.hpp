#include "Handler.hpp"

namespace phosphor::log {

inline logarithm logger;

template<typename... Args>
inline void info(const char* fmt, Args... args) {
    logger.logFmt(LOG_LEVEL::INFO, fmt, args...);
}

template<typename... Args>
inline void warn(const char* fmt, Args... args) {
    logger.logFmt(LOG_LEVEL::WARN, fmt, args...);
}

template<typename... Args>
inline void error(const char* fmt, Args... args) {
    logger.logFmt(LOG_LEVEL::ERROR, fmt, args...);
}

template<typename... Args>
inline void debug(const char* fmt, Args... args) {
    logger.logFmt(LOG_LEVEL::DEBUG, fmt, args...);
}

} // namespace phosphor::log