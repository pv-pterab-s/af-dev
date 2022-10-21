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

#include "io.hpp"

template<typename T>
void go(Param<T> out, Param<T> in, float theta, af_interp_type method, int order) {
    constexpr int TX = 16;
    constexpr int TY = 16;
    // Used for batching images
    constexpr int TI = 4;
    constexpr bool isComplex =
      static_cast<af_dtype>(af::dtype_traits<T>::af_type) == c32 ||
      static_cast<af_dtype>(af::dtype_traits<T>::af_type) == c64;

    const float c = cos(-theta), s = sin(-theta);
    const float nx = 0.5 * (in.info.dims[0] - 1);
    const float ny = 0.5 * (in.info.dims[1] - 1);
    const float mx = 0.5 * (out.info.dims[0] - 1);
    const float my = 0.5 * (out.info.dims[1] - 1);
    const float sx = (mx * c + my * -s);
    const float sy = (mx * s + my * c);
    const float tx = -(sx - nx), ty = -(sy - ny);

    // Rounding error. Anything more than 3 decimal points wont make a diff
    const float tmat[6] = {
      (float)(round(c * 1000) / 1000.0f),
      (float)(round(-s * 1000) / 1000.0f),
      (float)(round(tx * 1000) / 1000.0f),
      (float)(round(s * 1000) / 1000.0f),
      (float)(round(c * 1000) / 1000.0f),
      (float)(round(ty * 1000) / 1000.0f)
    };

    const auto local = sycl::range{TX, TY};

    size_t nimages         = in.info.dims[2];
    const size_t nbatches  = in.info.dims[3];
    size_t global_x        = local[0] * divup(out.info.dims[0], local[0]);
    size_t global_y        = local[1] * divup(out.info.dims[1], local[1]);
    const size_t blocksXPerImage = global_x / local[0];
    const size_t blocksYPerImage = global_y / local[1];

    if (nimages > TI) {
      const size_t tile_images = divup(nimages, TI);
      nimages         = TI;
      global_x        = global_x * tile_images;
    }
    global_y *= nbatches;

    const auto global = sycl::range{global_x, global_y};

    sycl::buffer<int, 2> debugBuffer{sycl::range<2>{global[0], global[1]}};

    getQueue().submit([&](sycl::handler &h) {
        auto d_out = out.data->get_access(h);
        auto d_in = in.data->get_access(h);
        auto debugBufferAcc = debugBuffer.get_access(h);

        h.parallel_for(sycl::nd_range{global, local}, [=](sycl::nd_item<2> it) {
          sycl::group g = it.get_group();

          // Compute which image set
          const int setId = g.get_group_id(0) / blocksXPerImage;
          const int blockIdx_x = g.get_group_id(0) - setId * blocksXPerImage;

          const int batch = g.get_group_id(1) / blocksYPerImage;
          const int blockIdx_y = g.get_group_id(1) - batch * blocksYPerImage;

          // Get thread indices
          const int xido = it.get_local_id(0) + blockIdx_x * g.get_local_range(0);
          const int yido = it.get_local_id(1) + blockIdx_y * g.get_local_range(1);

          const int limages = fmin((int)out_.dims[2] - setId * nimages_, nimages_);

          if (xido >= out_.dims[0] || yido >= out_.dims[1]) return;

          // debugBufferAcc[it.get_global_id(0)][it.get_global_id(1)] = it.get_global_id(1);

          // // Compute input index
          // typedef typename itype_t<T>::wtype WT;
          // WT xidi = xido * t_.tmat[0] + yido * t_.tmat[1] + t_.tmat[2];
          // WT yidi = xido * t_.tmat[3] + yido * t_.tmat[4] + t_.tmat[5];

          // int outoff =
          //   out_.offset + setId * nimages_ * out_.strides[2] + batch *
          //   out_.strides[3];
          // int inoff =
          //   in_.offset + setId * nimages_ * in_.strides[2] + batch *
          //   in_.strides[3];

          // const int loco = outoff + (yido * out_.strides[1] + xido);
        });
    }).wait();

    // write out the debug buffer...
    // the output buffer...
    {
      OPEN_W("doo");
      WRITE(debugBuffer);
    }
}

int main() {
  Param<float> in;
  Param<float> out;
  float theta;
  af_interp_type method;
  int order;
  OPEN_R("/home/gpryor/arrayfire/build/doo");
  READ(in);
  READ(out);
  READ(theta);
  READ(method);
  READ(order);
  go<float>(out, in, theta, method, order);
}
