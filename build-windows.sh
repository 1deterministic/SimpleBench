#!/bin/bash
x86_64-w64-mingw32-gcc -O0 *.c -o simplebench-windows.exe &&
i686-w64-mingw32-gcc -O0 *.c -o simplebench32-windows.exe