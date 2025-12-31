@echo off
setlocal
call vcvarsall x64
cmake .
cmake --build .
.\scripts\finalize.bat
endlocal