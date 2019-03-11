#!/bin/bash
# 32-bit
gcc -O0 -m32 -pthread *.c -o simplebench32-linux -lm &&
chmod +x simplebench32-linux &&
# 64-bit
gcc -O0 -pthread *.c -o simplebench-linux -lm &&
chmod +x simplebench-linux &&

echo "Done!"