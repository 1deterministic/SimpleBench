#!/bin/bash
# 64-bit (install gcc via brew)
/usr/local/bin/gcc-9 -O0 -pthread *.c -o simplebench-mac -lm &&
chmod +x simplebench-mac &&

echo "Done!"
