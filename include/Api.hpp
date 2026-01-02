#include "Handler.hpp"

namespace phosphor::log {

inline core logger;

template<typename... Args>
inline void info(const char* fmt, Args&&... args) {
    logger.logFmt(LOG_LEVEL::INFO, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void warn(const char* fmt, Args&&... args) {
    logger.logFmt(LOG_LEVEL::WARN, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void error(const char* fmt, Args&&... args) {
    logger.logFmt(LOG_LEVEL::ERROR, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void debug(const char* fmt, Args&&... args) {
    logger.logFmt(LOG_LEVEL::DEBUG, fmt, std::forward<Args>(args)...);
}

} // namespace phosphor::log