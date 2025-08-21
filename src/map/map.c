// Libraries
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


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



/// This type represents functions that are used for hashing
///
/// Parameters:
///   - void *: pointer to the beginning of the key
typedef unsigned int (*MapHashFn)(void *);


Map *map_init(const MapAllocFn alloc, const MapFreeFn dealloc, const MapHashFn hash,
        const size_t key_len, const size_t val_len) {
}



/***************************** Pair declarations ******************************/
/// Pair type
///
/// Stores key, value and metadata in one block of memory
typedef struct pair Pair;


/// Initialize a pair
///
/// This function initializes a Pair by allocating memory for it 
/// according to [map]'s alloc function
///
/// Parameters:
///   - map: pointer to a map that was retunred by map_init();
///   - k_ptr: pointer to a key
///   - v_ptr: pointer to the value
///
/// Returns:
///   A pointer to a pair that needs to be freed by `pair_free`
static Pair *pair_init(const Map *map, const void *k_ptr, const void *v_ptr);

/// Free a pair
///
/// This function frees a Pair by freeing the memory allocated to it
/// according to [map]'s dealloc function
///
/// Parameters:
///   - map: pointer to a map that was retunred by map_init();
///   - pair: Pointer to a pair returned by `pair_init()`
static void pair_free(const Map *map, Pair *pair);
/***************************** Map implementation *****************************/
struct map {
    const MapAllocFn alloc; /* Nonnull */
    const MapFreeFn dealloc; /* Nonnull */
    const MapHashFn hash; /* Nonnull */
    const size_t key_len;
    const size_t val_len;
    Pair **table; /* Nonnull */
};


/**************************** Pair implementation *****************************/

struct pair {
    void *key;
    void *value;
    size_t key_len;
    size_t val_len;
};



static Pair *pair_init(const Map *map, const void *k_ptr, const void *v_ptr) {
    // If Map was NULL something went wrong
    if (map == NULL) {
        fprintf(stderr,"map was NULL in %s:%d", __FILE__, __LINE__);
        return NULL;
    }

    // Determine size of memory to be allocated
    const size_t alloc_sz = map->key_len + map->val_len + sizeof(Pair);

    Pair *new_pair = map->alloc(alloc_sz);
    if (map->alloc == NULL) {
        fprintf(stderr,"Memory allocation failed in %s:%d", __FILE__, __LINE__);
        return NULL;
    }

    // Get pointer to key
    void *key_ptr = new_pair + 1;

    // Get pointer to value
    void *val_ptr = (char *)key_ptr + map->key_len;

    // Copy key
    memcpy(key_ptr, k_ptr, map->key_len);

    // Copy value
    memcpy(val_ptr, v_ptr, map->val_len);

    // Assign values
    new_pair->val_len = map->val_len;
    new_pair->key_len = map->key_len;
    new_pair->key = key_ptr;
    new_pair->value = val_ptr;


    return new_pair;
}



static void pair_free(const Map *map, Pair *pair) {
    // If Map was NULL something went wrong
    if (map == NULL) {
        fprintf(stderr,"map was NULL in %s:%d", __FILE__, __LINE__);
        return;
    }
    // Free the pair
    map->dealloc(pair);
}











