#ifndef VECTOR_H
#define VECTOR_H


// Libraries
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Parameters:
/// - size_t: amount of bytes needed
typedef void *(*VecAllocFn)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void *(*VecReAllocFn)(void *, size_t);

/// This type represents functions that are used to allocate initialized memory
/// the function 'calloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void (*VecCallocFn)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Parameters:
/// - void *: pointer to memory  to free
typedef void (*VecFreeFn)(void *);


/// Handle to a vector
///
/// This struct represents a handle to a vector
/// A pointer to this is used to pass into functions and
/// perform operations on it.
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
Vector *vector_init(const VecAllocFn alloc, const VecFreeFn dealloc, const size_t elemsize);



/// Push a value into the vector
///
/// This function appends a value to the end of [vec]
///
/// Parameters:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
///   - data: the value that will be appended to the vector
///   - data_len: size of the data to insert
void vector_insert(Vector *vec, void *data, size_t data_len);


/// Free up memory used by a vector
///
/// This function frees the memory used by a vector according to 'dealloc'
/// which was set in 'vec_type_init'
///
/// Parameteter:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
void vector_free(Vector *vec);



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
void *vector_at(const Vector *vec, const size_t index);



/// Get size of vector
///
/// This function gets the size of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   amount of elements in the vector. Length of NULL is 0;
size_t vector_size(const Vector *vec);




/// Get size of the elements in the vector
///
/// This function returns the size of the elements in the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   size of elements in the vector. if vec is NULL, 0 is returned;
size_t vector_elem_size(const Vector *vec);


/// Get allocate function of the vector
///
/// This function returns the allocator function of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   function pointer to the alloc function, NULL if [vec] is NULL
VecAllocFn vector_alloc_fn(const Vector *vec);



/// Get deallocate function of the vector
///
/// This function returns the allocator function of the vector
///
/// Parameters:
///   - vec: handle to a Vector that was returned by `vec_init`
///
/// Returns:
///   function pointer to the alloc function, NULL if [vec] is NULL
VecFreeFn vector_dealloc_fn(const Vector *vec);




/******************************* Macro wrapper ********************************/
/// NOTE: for the following documentation Vec refers to the 'wrapper struct' 
/// for a Vector and a buffer and Vector refers to the actual underlying Vector
/// struct.




/// Vec of a type (wrapper struct)
///
/// This macro works like a typedef with a parameter
///
/// Example:
///   VecOf(int) vector = {0}; // Vector of integers
#define VecOf(type) struct { Vector *vec; type *buf; }

/// Push value into a Vec
///
/// This macro first initializes a Vec with the default alloc and free function
/// then it pushes [value] into the Vec. If the Vec is not preinitialized with 0
/// the behavior is undefined.
///
/// Example:
///   VecOf(int) vector = {0}; // Needed to make sure that the Vec is initialized
///   vec_push(vector, 2048);
#define vec_push(vector, value) \
    if (vector.vec == NULL || vector.buf == NULL) { \
        vector.vec = (Vector *)vector_init(malloc, free, sizeof(*vector.buf)); \
        vector.buf  = vector_init_buf(vector.vec); \
    } \
    if (vector.vec != NULL && vector.buf != NULL) { \
        *vector.buf = value; \
        vector_insert(vector.vec, vector.buf, sizeof(*vector.buf)); \
    }

/// Create a new Vec
///
/// This macro creates a new Vec with a specified alloc and dealloc function
/// The filed [vector] is the name of the new variable so if you have Vec
/// called cool_vector the first field in this needs to be cool_vector
///
/// Example:
///   VecOf(int) cool_vector = vec_new(cool_vector, malloc, free);
#define vec_new(vector, alloc, dealloc) { \
    .vec = vector_init(malloc, free, sizeof(*vector.buf)), \
    .buf = vector_init_buf(vector.vec) \
}



/// Check if any field is NULL
///
/// This macro checks if any of the fields of a Vec is NULL
/// Recommeneded to be used after vec_new;
///
/// Example:
///   VecOf(int) cooler_vector = vec_new(cool_vector, malloc, free);
///   if (!vec_ok(cooler_vector)) {
///       return; // error handling
///   }
#define vec_ok(vector) vector.vec != NULL && vector.buf != NULL



/// Free up memory used by this Vec
///
/// This macro frees the underlying Vector and the buffer
///
/// Example:
///   VecOf(int) vector = {0};
///   vec_push(vector, 2048);
///   vec_push(vector, 1024);
///   /* Do something */
///   vec_del(vector);
#define vec_del(vector) \
    vector_free_buf(vector.vec, vector.buf); \
    vector_free(vector.vec)

/// Get a value at an index
///
/// This macro gets a pointer to a value at [index] if the index is value
/// else it returns NULL
/// VecOf(int) vector = {0};
/// vec_push(vector, 2048);
/// int *some_value = vec_get(vector, 0);
#define vec_get(vector, index) vector_at(vector.vec, index)


/// Get the amount of elements in the vector
#define vec_size(vector) vector_size(vector.vec)


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
void *vector_init_buf(const Vector *vec);






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
void vector_free_buf(const Vector *vec, void *buf);

#endif // VECTOR_H
