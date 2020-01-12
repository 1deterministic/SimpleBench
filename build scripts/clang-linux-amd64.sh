#!/bin/bash
clang -static -O0 -pthread ../*.c -o simplebench-linux-amd64-clang -lm