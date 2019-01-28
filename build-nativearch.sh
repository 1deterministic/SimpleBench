#!/bin/bash
gcc -O0 -march=native -pthread *.cpp -o simplebench &&
chmod +x simplebench &&
echo "Done!"
