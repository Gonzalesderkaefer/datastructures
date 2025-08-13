// Header file
#include "../../include/vector.h"

// Libraries
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


/********************************** Private ***********************************/


#define VEC_INIT_SIZE 16



struct _vector {
    size_t cap;
    size_t len;
    size_t elem_size;
    AllocFunc alloc;
    FreeFunc dealloc;
    void *stroage;
};



/// Initialize a vector
///
/// This function initializes a vector. It allocates memory
/// according to [alloc].
///
/// Parameters:
///   - alloc: an allocator function the function malloc is of this type
///   - dealloc: a function that frees memory
///   - elemsize: sizeof the elements that will be stored
Vector *vec_init(const AllocFunc alloc, const FreeFunc dealloc, const size_t elemsize) {
    // check if input is valid
    AllocFunc local_all = alloc;
    FreeFunc local_dea = dealloc;
    if (alloc == NULL || dealloc == NULL) {
        local_all = malloc;
        local_dea = free;
    }

    // Allocate struct
    Vector *vector = local_all(sizeof(Vector));
    if (vector == NULL) {
        return NULL;
    }

    // Allocate storage
    void *new_storage = local_all(elemsize * VEC_INIT_SIZE);
    if (new_storage == NULL) {
        local_dea(vector);
        return NULL;
    }
    // Zero out
    memset(new_storage, 0, elemsize * VEC_INIT_SIZE);

    // Assign fields
    vector->cap = VEC_INIT_SIZE;
    vector->len = 0;
    vector->elem_size = elemsize;
    vector->alloc = alloc;
    vector->dealloc = dealloc;
    vector->stroage = new_storage;

    return vector;
}





/// Push a value into the vector
///
/// This function appends a value to the end of [vec]
///
/// Parameters:
///   - vec: a handle to a vector that was returned by 'vec_init'
///   - data: the value that will be appended to the vector
///   - data_len: size of the data to insert
void vec_insert(Vector *vec, void *data, size_t data_len) {
    // Check if the size matches
    if (data_len != vec->elem_size) {
        return;
    }
    if (vec->len >= vec->cap - 1) {
        // Allocate new storage
        void *new_stroage = vec->alloc(vec->cap * 2 * vec->elem_size);
        if (new_stroage == NULL) {
            return;
        }

        // Copy old storage
        memcpy(new_stroage, vec->stroage, vec->elem_size * vec->cap);

        // calculate the border pointer
        void *border_ptr = (char *)new_stroage + vec->cap *vec->elem_size;

        // Determine restsize (which is still the half)
        const size_t rest_size =  vec->cap * vec->elem_size;

        // Set the rest to 0
        memset(border_ptr, 0, rest_size);

        // Update vec's capacity
        vec->cap *= 2;

        // Free old storage
        vec->dealloc(vec->stroage);
    }

    // Determine pointer to write at
    void *write_ptr = (char *)vec->stroage + (vec->elem_size * vec->len);

    // Copy the value
    memcpy(write_ptr, data, vec->elem_size);

    // Increment length
    vec->len++;
}


/// Free up memory used by a vector
///
/// This function frees the memory used by a vector according to 'dealloc'
/// which was set in 'vec_type_init'
///
/// Parameteter:
///   - vec: a handle to a vector that was returned by `vec_init`
void vec_free(Vector *vec) {
    // Store pointer to free function
    FreeFunc freefunc = vec->dealloc;

    // Free storage first 
    freefunc(vec->stroage);

    // Free actual struct
    freefunc(vec);
}


/// Get value at an index
///
/// This function returns a pointer to a value at [index]
/// if that index is valid.
///
/// Parameters:
///   - vec: a handle to a vector that was returned by `vec_init`
///   - index: index to look at
///
/// Returns:
///   pointer to the value at [index] if index is valid, NULL else
void *vec_at(const Vector *vec, const size_t index) {
    // Check if index is correct
    if (index >= vec->len) {
        return NULL;
    }
    // Determine pointer to look for
    void *lookup_ptr = (char *)vec->stroage + (vec->elem_size * index);

    return lookup_ptr;
}



/// Get size of vector
///
/// This function gets the size of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   amount of elements in the vector. Length of NULL is 0;
size_t vec_size(const Vector *vec) {
    if (vec == NULL) {
        return 0;
    }
    return vec->len;
}

/// Get size of the elements in the vector
///
/// This function returns the size of the elements in the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   size of elements in the vector. if vec is NULL, 0 is returned;
size_t vec_elem_size(const Vector *vec) {
    if (vec == NULL) {
        return 0;
    }
    return vec->elem_size;
}


/// Get allocate function of the vector
///
/// This function returns the allocator function of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   function pointer to the alloc function, NULL if [vec] is NULL
AllocFunc vec_alloc_fn(const Vector *vec) {
    if (vec == NULL) {
        return NULL;
    }
    return vec->alloc;
}


/// Get deallocate function of the vector
///
/// This function returns the allocator function of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   function pointer to the alloc function, NULL if [vec] is NULL
FreeFunc vec_dealloc_fn(const Vector *vec) {
    if (vec == NULL) {
        return NULL;
    }
    return vec->dealloc;
}


/// Initialize a vector buffer that stores temporary values
///
/// This function allocates memory according to [vec's] alloc
/// function to store values temporarily. This function is actually
/// not meant to be used by end users. It serves as a helper function
/// for the macro wrapper
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   a pointer to a buffer that stores temporary values
void *vec_init_buf(const Vector *vec) {
    // Sanity check
    if (vec == NULL) {
        return NULL;
    }

    void *buf = vec->alloc(vec->elem_size);
    if (buf == NULL) {
        return NULL;
    }
    return buf;
}

/// Free a temporary buffer
///
/// This function frees up the memory used by a temporary
/// buffer for a vector. This function is actually
/// not meant to be used by end users. It serves as a helper function
/// for the macro wrapper
///
/// Parameters
///   - vec: handle to a Vector that was returned by `vec_init`
///   - buf: a pointer to a buffer that was retuned by `vec_init_buf`
void vec_free_buf(const Vector *vec, void *buf) {
    // Sanity check
    if (vec == NULL || vec == NULL) {
        return;
    }

    // Store pointer to free function
    FreeFunc freefunc = vec->dealloc;

    // Free storage first 
    freefunc(buf);
}


