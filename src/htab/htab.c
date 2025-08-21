// Libraries
#include <stdbool.h>
#include <stddef.h>


/*********************************** Public ***********************************/


/// Handle to a map
///
/// This struct represents a handle to a map
/// A pointer to this is used to pass into functions and
/// perform operations on it.
typedef struct map Map;


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Parameters:
/// - size_t: amount of bytes needed
typedef void *(*MapAllocFn)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void *(*MapReAllocFn)(void *, size_t);

/// This type represents functions that are used to allocate initialized memory
/// the function 'calloc' is of this type
///
/// Parameters:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void (*MapCallocFn)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Parameters:
/// - void *: pointer to memory  to free
typedef void (*MapFreeFn)(void *);




/********************************** Private ***********************************/
struct map {
    const size_t key_len;
    const size_t val_len;
};


/**************************** Pair implementation *****************************/

typedef struct {
    void *key;
    void *value;
    size_t key_len;
    size_t val_len;
    bool occupied;
} Pair;



static Pair *pair_init(const Map *map) {
}

