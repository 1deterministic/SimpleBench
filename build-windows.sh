#!/bin/bash
# 32-bit
i686-w64-mingw32-gcc -O0 *.c -o simplebench32-windows.exe &&
# 64-bit
x86_64-w64-mingw32-gcc -O0 *.c -o simplebench-windows.exe &&

echo "Done!"