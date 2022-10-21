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

// Param<int> c_;
// c_.info.dims[0] = 1;
// c_.info.dims[1] = 1;
// c_.info.dims[2] = 1;
// c_.info.dims[3] = 1;
// c_.info.strides[0] = 1;
// c_.info.strides[1] = 1;
// c_.info.strides[2] = 1;
// c_.info.strides[3] = 1;
// c_.info.offset = 0;
// sycl::buffer<int, 1> b2{sycl::range{1}};
// {
//   sycl::host_accessor<int> _br(b2);
//   _br[0] = 2056;
// }
// c_.data = &b2;

// int a__; int b__; Param<float> c__;
// {
//   OPEN_R("doo");
//   READ(a__); READ(b__); READ(c__);
// }

int main() {
    Param<float> out; Param<float> in; float theta; af_interp_type method; int order;
    OPEN_R("/home/gpryor/arrayfire/build/doo");
    READ(out); READ(in); READ(theta); READ(method); READ(order);

    printf("%lld, %lld, %lld, %lld\n", out.info.dims[0], out.info.dims[1], out.info.dims[2], out.info.dims[3]);
    printf("%f\n", theta);
}


// return 0;
// }
