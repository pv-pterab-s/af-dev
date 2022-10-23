#!/bin/bash

set -e

if [ main.cpp -nt out/main ] || [ impl.hpp -nt out/main ]; then # only build if sources updated
    echo building
    mkdir -p out && /usr/bin/time icpx -g -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include main.cpp -o out/main
fi
cd out
./main $1
