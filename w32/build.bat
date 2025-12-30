@echo off
setlocal
vcvarsall x64
cmake .
cmake --build .
finalize
endlocal