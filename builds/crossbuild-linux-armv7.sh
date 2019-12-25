#!/bin/bash
arm-linux-gnueabihf-gcc -static -O0 -pthread ../*.c -o simplebench-linux-armv7 -lm
