#!/bin/bash
gcc -O0 -m32 -pthread *.c -o simplebench32 &&
chmod +x simplebench32 &&
gcc -O0 -pthread *.c -o simplebench &&
chmod +x simplebench &&
echo "Done!"