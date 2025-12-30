# tesseract
tesseract is a lightweight version of fmt. nothing else to put here (or is there).<br>
see inclusion directions below

# inclusion directions
* you will need cmake to use tesseract. if you don't have cmake, download it from [here](https://cmake.org/download)


### (recommended):
* if you have an existing project and want to add tesseract to it, add these rules to your CMakeLists:

```cmake
include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/external")
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(
    tesseract
    GIT_REPOSITORY https://github.com/xodys10/tesseract.git
    GIT_TAG main
)

FetchContent_MakeAvailable(tesseract)

target_link_libraries(${PROJECT_NAME} PRIVATE tesseract)

```
* then, to use it: you need to include headers:
```cpp
// ...top of your project
#include "Tesseract.hpp"
```

## api documentation
to use the api:
* all functions are contained in the `log` namespace
* format specifiers: `{:type}` (like fmt)

### available functions
* `info`: log info, used for additional information such as initializing something, or whatever else
* `debug`: like info, but use cases are to debug information not usually needed normally
* `warn`: used for warnings, usually used to warn the user and what they should do.
* `error`: used to display errors, you use this when you need to log an important error or just any error

### usage:
```cpp
#include "Tesseract.hpp"

int main(void) {
    int someVal = 0;
    if (someVal != 0) log::warn("someVal is not 0!");
    else log::debug("someVal is {:d}", someVal);

    return 0;
}
```

but if you're REALLY paranoid, you can use the internal functions:
```c++
log::logarithm::logFmt(<logLevel>, <message>);
```
* replace `<logLevel>` with your log level of choice
* replace `<message>` with your message of choice

### LOG_LEVEL usage:
* `INFO`: 0
* `WARN`: 1
* `ERROR`: 2
* `DEBUG`: 3
* `NONE`: 4 (almost never used)