#include <CL/sycl.hpp>
#include <arrayfire.h>
#include <stdlib.h>

// #define MSG(S)                                  \
//   { printf("S##\n"); }
// #define M(S)                                    \
//   { printf("%lld\n", (S)); }

#define divup(a, b) (((a) + (b)-1) / (b))

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

template <typename T>
void M(Param<T> p) {
  printf("%lld, %lld, %lld, %lld\n",
         p.info.dims[0], p.info.dims[1],
         p.info.dims[2], p.info.dims[3]);
  printf("%lld, %lld, %lld, %lld\n",
         p.info.strides[0], p.info.strides[1],
         p.info.strides[2], p.info.strides[3]);
  // i,j,k,l
  sycl::host_accessor<T> _p(*p.data);
  for (int l = 0; l < p.info.dims[3]; l++) {
    for (int k = 0; k < p.info.dims[2]; k++) {
      for (int i = 0; i < p.info.dims[0]; i++) {
        for (int j = 0; j < p.info.dims[1]; j++) {
          const int idx =
            i * p.info.strides[0] +
            j * p.info.strides[1] +
            k * p.info.strides[2] +
            l * p.info.strides[3];
          printf("%d", (int)_p[idx]);
        }
        printf("\n");
      }
    }
  }
}

sycl::queue getQueue() {
    return sycl::queue();
}

using cdouble = std::complex<double>;
using cfloat  = std::complex<float>;     // above is arrayfire emulation

#include "io.hpp"

#include "impl.hpp"

int main(int argc, char **argv) {
  Param<float> in;
  Param<float> out;
  float theta;
  af_interp_type method;
  int order;
  OPEN_R(argv[1]);
  READ(in);
  READ(out);
  READ(theta);
  READ(method);
  READ(order);
  rotate<float>(out, in, theta, method, order);
  // {
  //   OPEN_W("run-output");
  //   WRITE(out);
  // }
}
