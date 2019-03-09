#!/bin/bash
gcc -O0 -m32 -pthread *.c -o simplebench32-linux &&
chmod +x simplebench32-linux &&
gcc -O0 -pthread *.c -o simplebench-linux &&
chmod +x simplebench-linux &&
echo "Done!"