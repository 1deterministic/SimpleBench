#!/bin/bash
clang -static -O0 -m32 -pthread ../*.c -o simplebench-linux-x86-clang -lm
