#pragma once
#include "Handler.hpp"

namespace phosphor::log::detail {

template<std::size_t I = 0, typename Tuple>
void dispatchTupleImpl(const Tuple& tuple, std::size_t index, char spec, bool& dispatched) {
    if constexpr (I < std::tuple_size_v<Tuple>) {
        if (I == index) {
            dispatchLog(spec, std::get<I>(tuple));
            dispatched = true;
            return;
        }
        dispatchTupleImpl<I + 1>(tuple, index, spec, dispatched);
    }
}

template<typename Tuple>
inline void dispatchTuple(const Tuple& tuple, std::size_t index, char spec) {
    bool dispatched = false;
    dispatchTupleImpl(tuple, index, spec, dispatched);

    if (!dispatched) {
        std::fputs("\n[phosphor]: missing format arguments\n", stderr);
        std::abort();
    }
}

}; // namespace phosphor::detail;

namespace phosphor::log {

template<typename... Args>
void core::formatImpl(const char* fmt, Args&&... args) {
    auto argTuple = std::forward_as_tuple(std::forward<Args>(args)...);
    std::size_t argIndex = 0;
    
    while (*fmt) {
        if (fmt[0] == '{' && fmt[1] == ':' && fmt[2] && fmt[3] == '}') {
            if (argIndex >= sizeof...(Args)) {
                std::fputs("\n[phosphor]: missing format arguments\n", stderr);
                std::abort();
            }

            detail::dispatchTuple(argTuple, argIndex, fmt[2]);
            fmt += 4;
            ++argIndex;
        } else {
            std::fputc(*fmt++, stderr);
        }
    }
}

template<typename... Args>
void core::logFmt(LOG_LEVEL level, const char* fmt, Args... args) {
    if (level < logLevel.load(std::memory_order_relaxed))
        return;

    std::fprintf(
        stderr,
        "[%s %s]: ",
        getTime(),
        toString(level)
    );

    formatImpl(fmt, std::forward<Args>(args)...);
    std::fputc('\n', stderr);
}

} // namespace phosphor::log