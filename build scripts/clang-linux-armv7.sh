#!/bin/bash
clang -static -O0 -pthread ../*.c -o simplebench-linux-armv7-clang -lm