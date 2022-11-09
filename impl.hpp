template<typename T>
void reorder(Param<T> out, const Param<T> in, const dim_t* rdims) {

    // linear index

    // 2d index to 4d index to linear index ...
    // 4d index ... to linear index ...

    // input array (p)


    const int
      Ox = out.info.dims[0], Oy = out.info.dims[1],
      Oz = out.info.dims[2], Ow = out.info.dims[3];
    const int
      Px = in.info.dims[0], Py = in.info.dims[1],
      Pz = in.info.dims[2], Pw = in.info.dims[3];

    const size_t On = Ox * Oy * Ox * Ow;
    sycl::range global{On, 1};
    /*
     */
    sycl::range local{1, 1};

    getQueue().submit([&](auto& h) {
        h.parallel_for(sycl::nd_range{global, local}, [=](auto it) {
        });
    }).wait();

    return;

    // OPEN_W("reorder-stuff");
    // WRITE(out); WRITE(in); WRITE(rdims[0]); WRITE(rdims[1]); WRITE(rdims[2]); WRITE(rdims[3]);

    // constexpr int TX    = 4;
    // constexpr int TY    = 4;
    // constexpr int TILEX = 32;
    // constexpr int TILEY = 32;

    // const int rdims0 = rdims[0];
    // const int rdims1 = rdims[1];
    // const int rdims2 = rdims[2];
    // const int rdims3 = rdims[3];

    // sycl::range local{TX, TY};

    // int blocksPerMatX = divup(out.info.dims[0], TILEX);
    // int blocksPerMatY = divup(out.info.dims[1], TILEY);
    // sycl::range global(local[0] * blocksPerMatX * out.info.dims[2],
    //                    local[1] * blocksPerMatY * out.info.dims[3]);

    // sycl::buffer<float, 2> debugBuffer{sycl::range<2>{global[0], global[1]}};

    // getQueue()
    //   .submit([&](auto &h) {
    //       sycl::accessor debugAcc{debugBuffer, h};
    //       h.parallel_for(sycl::nd_range{global, local}, [=](auto it) {
    //           debugAcc[it.get_global_id(0)][it.get_global_id(1)] = 0;
    //       });
    //   }).wait();

    // getQueue()
    //   .submit([&](auto &h) {
    //       sycl::accessor inAcc{*in.data, h, sycl::read_only};
    //       sycl::accessor outAcc{*out.data, h, sycl::write_only, sycl::no_init};
    //       // sycl::accessor debugAcc{debugBuffer, h};
    //       h.parallel_for(sycl::nd_range{global, local}, [=](auto it) {
    //           sycl::group g = it.get_group();

    //           const int oz = g.get_group_id(0) / blocksPerMatX;
    //           const int ow = g.get_group_id(1) / blocksPerMatY;

    //           const int blockIdx_x = g.get_group_id(0) - oz * blocksPerMatX;
    //           const int blockIdx_y = g.get_group_id(1) - ow * blocksPerMatY;

    //           // const int xx = it.get_local_id(0) + blockIdx_x * g.get_local_range(0);
    //           // const int yy = it.get_local_id(1) + blockIdx_y * g.get_local_range(1);

    //           const int xx = it.get_global_id(0);
    //           const int yy = it.get_global_id(1);

    //           if (xx >= out.info.dims[0] || yy >= out.info.dims[1] ||
    //               oz >= out.info.dims[2] || ow >= out.info.dims[3])
    //             return;

    //           const int incy = blocksPerMatY * g.get_local_range(1);
    //           const int incx = blocksPerMatX * g.get_local_range(0);

    //           const int o_off =
    //             ow * out.info.strides[3] + oz * out.info.strides[2];
    //           // const int rdims[] = {d0, d1, d2, d3};
    //           int ods[] = {xx, yy, oz, ow};
    //           int ids[4] = {0};

    //           ids[rdims3] = ow;
    //           ids[rdims2] = oz;

    //           for (int oy = yy; oy < out.info.dims[1]; oy += incy) {
    //             ids[rdims1] = oy;
    //             for (int ox = xx; ox < out.info.dims[0]; ox += incx) {
    //               ids[rdims0] = ox;

    //               const int oIdx = o_off + oy * out.info.strides[1] + ox;

    //               const int iIdx = in.info.offset +
    //                 ids[3] * in.info.strides[3] +
    //                 ids[2] * in.info.strides[2] +
    //                 ids[1] * in.info.strides[1] + ids[0];

    //               // const int inSize = in.info.dims[0] * in.info.dims[1]  * in.info.dims[2] * in.info.dims[3];
    //               // const int outSize = out.info.dims[0] * out.info.dims[1]  * out.info.dims[2] * out.info.dims[3];
    //               // if (oIdx > outSize)
    //               //   debugAcc[it.get_global_id(0)][it.get_global_id(1)] = 10;

    //               outAcc[oIdx] = inAcc[iIdx];
    //             }
    //           }
    //       });
    //   }).wait();

    // // OPEN_W("run-output"); WRITE(debugBuffer);

    // ONEAPI_DEBUG_FINISH(getQueue());
}
