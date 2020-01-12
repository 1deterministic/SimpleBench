#!/bin/bash
# gcc via brew
# it seems that macOS does not allow for statically linked builds, not really sure
clang -O0 -pthread ../*.c -o simplebench-mac-amd64-clang -lm
