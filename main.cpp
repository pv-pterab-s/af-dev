#include <CL/sycl.hpp>
#include <arrayfire.h>
#include <stdlib.h>

#define ONEAPI_DEBUG_FINISH(S) ;

#define divup(a, b) (((a) + (b)-1) / (b))

typedef unsigned char uchar;

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
using cfloat  = std::complex<float>;


// TODO AF_BATCH_UNSUPPORTED is not required and shouldn't happen
//      Code changes are required to handle all cases properly
//      and this enum value should be removed.
typedef enum {
    AF_BATCH_UNSUPPORTED = -1, /* invalid inputs */
    AF_BATCH_NONE,             /* one signal, one filter   */
    AF_BATCH_LHS,              /* many signal, one filter  */
    AF_BATCH_RHS,              /* one signal, many filter  */
    AF_BATCH_SAME,             /* signal and filter have same batch size */
    AF_BATCH_DIFF,             /* signal and filter have different batch size */
} AF_BATCH_KIND;

// #include
// ^^^^^^^^^ above is arrayfire emulation

#define READ_NOT_WRITE

#include "io.hpp"
#include "impl.hpp"

using namespace sycl;

int main(int argc, char **argv) {
    // read test data
    Param<float> out;
    Param<float> sig;
    Param<float> filt;
    int cDim;
    bool expand;

    OPEN_R("/home/gpryor/fun/test-data-1");
    READ(out);
    READ(sig);
    READ(filt);
    READ(cDim);
    READ(expand);

    convSep<float, float>(out, sig, filt, cDim, expand);

    // octave sounds great for render, but easier here w/ M(out)
    {
      OPEN_W("run-output");
      WRITE(out);
    }

#if 0
    OPEN_R("/home/gpryor/fun/test-data");
    Param<float> signal;
    Param<float> filter;
    Param<float> out;
    AF_BATCH_KIND kind;
    int rank;
    bool expand;

    READ(signal); READ(filter); READ(out); READ(kind); READ(rank); READ(expand);
    // print(signal);
    // print(filter);
    // exit(0);

    convolve_nd<float, float>(out, signal, filter, kind, rank, expand);
    // print(out);

    // print(signal);
    print(filter);
    // print(out);

    {
      OPEN_W("run-filter");
      WRITE(filter);
    }

    {
      OPEN_W("run-output");
      WRITE(out);
    }
#endif

#if 0
    // the following shows a non-trivial memcpy
    constexpr int M = 6;
    float a[M], b[M];
    for (int i = 0; i < M; i++) {
      a[i] = 10 + i;
      b[i] = 4;
    }
    sycl::buffer<float, 1> aBuf{a, range<1>{M}};
    sycl::buffer<float, 1> bBuf{b, range<1>{M}};

    // queue Q;
    // Q.submit([&](auto &h) {
    //     accessor aAcc{aBuf, h, sycl::range{M - 1}, sycl::id{0}, read_only};
    //     accessor bAcc{bBuf, h, sycl::range{M - 1}, sycl::id{1}, write_only, no_init};
    //     h.copy(aAcc, bAcc);     // from a to b
    // }).wait();
    memcpyBuffer(bBuf, aBuf, 5, 1);
    getQueue().wait();
#endif
}
