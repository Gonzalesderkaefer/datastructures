
// Header file
#include "../include/vector.h"
#include <stdlib.h>


void test_vec(void) {
    VecOf(int) vector = {0};
    vec_push(vector, 3);
    vec_del(vector);
}
