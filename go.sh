#!/bin/bash
# -*- compile-command: "./go.sh"; -*-
set -ex

mkdir -p out && /usr/bin/time icpx -g -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include main.cpp -o out/main
(cd out && ./main)
octave r.m
