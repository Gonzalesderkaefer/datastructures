
// Header file
#include "../include/vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



void test_vec(void) {
    VecOf(int) vector = vec_new(vector, malloc, free);
    vec_push(vector, 3);


    size_t veclen = vector_size(vector.vec);

    for (uint64_t i = 0; i < veclen; ++i) {
        printf("%d\n", *(int *)vec_get(vector, i));
    }

    vec_del(vector);
}
