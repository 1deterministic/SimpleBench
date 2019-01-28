#!/bin/bash
gcc -O0 -m32 -pthread *.cpp -o simplebench32 &&
chmod +x simplebench32 &&
gcc -O0 -pthread *.cpp -o simplebench &&
chmod +x simplebench &&
echo "Done!"