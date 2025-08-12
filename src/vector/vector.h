#ifndef JAZZY_ALLOC_TYPES
#define JAZZY_ALLOC_TYPES

// Libraries
#include<stddef.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Parameters:
/// - size_t: amount of bytes needed
typedef void *(*AllocFunc)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void *(*ReAllocFunc)(void *, size_t);

/// This type represents functions that are used to allocate initialized memory
/// the function 'calloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void (*CallocFunc)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Parameters:
/// - void *: pointer to memory  to free
typedef void (*FreeFunc)(void *);




#endif // JAZZY_ALLOC_TYPES


#ifndef VECTOR_H
#define VECTOR_H

// Libraries
#include <stddef.h>
#include <stdlib.h>
#include <string.h>



typedef struct _vector Vector;


/// Initialize a vector
///
/// This function initializes a vector. It allocates memory
/// according to [alloc].
///
/// Parameters:
///   - alloc: an allocator function the function malloc is of this type
///   - dealloc: a function that frees memory
///   - elemsize: sizeof the elements that will be stored
Vector *vec_init(const AllocFunc alloc, const FreeFunc dealloc, const size_t elemsize);



/// Push a value into the vector
///
/// This function appends a value to the end of [vec]
///
/// Parameters:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
///   - data: the value that will be appended to the vector
///   - data_len: size of the data to insert
void vec_insert(Vector *vec, void *data, size_t data_len);


/// Free up memory used by a vector
///
/// This function frees the memory used by a vector according to 'dealloc'
/// which was set in 'vec_type_init'
///
/// Parameteter:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
void vec_free(Vector *vec);



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
void *vec_at(const Vector *vec, const size_t index);





/******************************* Macro wrapper ********************************/






#define VecOf(type) struct { Vector *vec; (type) *buf }


#define vec_push(vector) \
    if (vector.vec == NULL || vector.buf == NULL) { \
        vector.vec = vec_init(malloc, free, sizeof(*vector.buf)); \
        vector.buf  = vec_init_buf(vector.vec); \
    } \
    if (vector.vec != NULL && vector.buf != NULL) { \
        vec_insert(vector.vec, vector.buf, sizeof(*vector.buf)); \
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
void *vec_init_buf(const Vector *vec);






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
void vec_free_buf(const Vector *vec, void *buf);

#endif // VECTOR_H
