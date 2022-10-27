// -*- compile-command: "mkdir -p out && icpx -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include types.cpp -o out/types -L/home/gpryor/arrayfire/build/src/backend/oneapi -lafoneapi && LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/gpryor/arrayfire/build/src/backend/oneapi ./out/types"; -*-
#include <stdio.h>
#include <arrayfire.h>

using namespace af;

int main() {
    af::cfloat _signal[3];
    for (int i = 0; i < 3; i++) _signal[i] = {1, 1};
    af::cfloat _filter[3];
    for (int i = 0; i < 3; i++) _filter[i] = {1, 1};
    auto signal = array(3, 1, _signal);
    auto filter = array(3, 1, _filter);
    auto c = convolve1(signal, filter);
    print("c", c);
    return 0;
}
