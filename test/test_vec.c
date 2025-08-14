
// Header file
#include "../include/vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define vec__new(vector, alloc, dealloc) { \
    .vec = vec_init(malloc, free, sizeof(*vector.buf)), \
    .buf = vec_init_buf(vector.vec) \
}




void test_vec(void) {
    VecOf(int) vector = vec_new(vector, malloc, free);
    vec_push(vector, 3);
    vec_push(vector, 1);
    vec_push(vector, 5);
    vec_push(vector, 4);
    vec_push(vector, 2);
    size_t veclen = vector_size(vector.vec);

    for (uint64_t i = 0; i < veclen; ++i) {
        printf("%d\n", *(int *)vec_get(vector, i));
    }

    vec_del(vector);
}
