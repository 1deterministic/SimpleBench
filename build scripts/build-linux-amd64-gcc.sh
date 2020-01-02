#!/bin/bash
gcc -static -O0 -pthread ../*.c -o simplebench-linux-amd64 -lm