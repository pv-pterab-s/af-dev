1. Put this at top of function in `impl.h`

        char fn[32]; snprintf(fn, 32, "test-data-%0.3d", 0);
        printf("!!WRITE %s\n", fn);
        OPEN_W(fn);
        WRITE(in); WRITE(out); WRITE(theta); WRITE(method); WRITE(order);

2. Implement in `impl.h`. Include in arrayfire kernel file.

3. Include `#include "/home/gpryor/dev/io.hpp"` in arrayfire kernel file.
