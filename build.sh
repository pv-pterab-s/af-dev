#!/bin/bash
# -*- compile-command: "./build.sh"; -*-
set -e

if [ main.cpp -nt out/main ]; then
    echo building
    mkdir -p out && /usr/bin/time icpx -g -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include main.cpp -o out/main
fi
cd out
./main
