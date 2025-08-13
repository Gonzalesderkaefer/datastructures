
// Header file
#include "../include/vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void test_vec(void) {
    VecOf(int) vector = {0};
    vec_push(vector, 3);
    vec_push(vector, 1);
    vec_push(vector, 5);
    vec_push(vector, 4);
    vec_push(vector, 2);
    size_t veclen = vec_size(vec_get_vec(vector));

    for (uint64_t i = 0; i < veclen; ++i) {
        printf("%d\n", *(int *)vec_get(vector, i));
    }



    vec_del(vector);
}
