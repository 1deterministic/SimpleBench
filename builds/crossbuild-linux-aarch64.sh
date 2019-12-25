#!/bin/bash
aarch64-linux-gnu-gcc -static -O0 -pthread ../*.c -o simplebench-linux-aarch64 -lm
