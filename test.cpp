// -*- compile-command: "mkdir -p out && /usr/bin/time icpx -fsycl -I/home/gpryor/arrayfire/include -I/home/gpryor/arrayfire/build/include test.cpp -o out/test && cd out && ./test"; -*-
#include <CL/sycl.hpp>
#include <arrayfire.h>
#include <stdlib.h>

#define MSG(S)                                  \
  { printf("S##\n"); }
#define M(S)                                    \
  { printf("%lld\n", (S)); }

typedef struct {
    dim_t dims[4];
    dim_t strides[4];
    dim_t offset;
} KParam;

template<typename T>
struct Param {
  sycl::buffer<T> *data;
  KParam info;
};

#include "io.hpp"

int main() {
    Param<int> c_;
    c_.info.dims[0] = 1;
    c_.info.dims[1] = 1;
    c_.info.dims[2] = 1;
    c_.info.dims[3] = 1;
    c_.info.strides[0] = 1;
    c_.info.strides[1] = 1;
    c_.info.strides[2] = 1;
    c_.info.strides[3] = 1;
    c_.info.offset = 0;
    sycl::buffer<int, 1> b2{sycl::range{1}};
    {
      sycl::host_accessor<int> _br(b2);
      _br[0] = 2056;
    }
    c_.data = &b2;

    int a_ = 16; int b_ = 32;
    {
      OPEN_W("doo");
      WRITE(a_); WRITE(b_); WRITE(c_);
    }

    int a__; int b__; Param<float> c__;
    {
      OPEN_R("doo");
      READ(a__); READ(b__); READ(c__);
    }
    // check "doo" by hand
    return 0;
}
