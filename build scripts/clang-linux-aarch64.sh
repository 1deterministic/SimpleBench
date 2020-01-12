#!/bin/bash
clang -static -O0 -pthread ../*.c -o simplebench-linux-aarch64-clang -lm