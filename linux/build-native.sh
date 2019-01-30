#!/bin/bash
gcc -O0 -march=native -pthread *.c -o simplebench &&
chmod +x simplebench &&
echo "Done!"
