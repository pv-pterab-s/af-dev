#!/bin/bash
set -e

if [ main.cpp -nt out/main ] || [ impl.hpp -nt out/main ]; then # only build if sources updated
    echo building

    # # link (for run)
    mkdir -p out && icpx -g -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include -L/home/gpryor/arrayfire/build/src/backend/oneapi -lafoneapi  main.cpp -o out/main

    # # compile only (fast)
    # mkdir -p out && /usr/bin/time icpx -g -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include -L/home/gpryor/arrayfire/build/src/backend/oneapi -c main.cpp -o out/main.o
    # exit
fi
cd out
rm -f run-output
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/gpryor/arrayfire/build/src/backend/oneapi
/usr/bin/time ./main $1
