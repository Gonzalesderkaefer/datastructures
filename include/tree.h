#ifndef JAZZY_TREE_H
#define JAZZY_TREE_H

// Libraries
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Parameters:
/// - size_t: amount of bytes needed
typedef void *(*TreeAllocFn)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Parameters:
/// - void *: pointer to memory  to free
typedef void (*TreeFreeFn)(void *);

/// This type represents functions that are used to compare to
/// blocks of memory the function 'memcmp' is of this type
///
/// Parameters:
///   - void *: first block of memory
///   - void *: second block of memory
///   - size_t: how many bytes o compare
typedef int (*TreeComparator)(const void *, const void *, size_t);





/// A handle to a tree
///
/// This type represents a handle to a tree. A pointer to this is passed
/// into function that perform operations on it
typedef struct _Tree Tree;

/// Initialize a tree
///
/// This function initializes a tree. I.e allocates memory according to [alloc]
/// elements in this tree are copied which is why a elem_size should be provided
///
/// Parameters:
///   - elem_size: size of the elements that are stored in the tree
///   - alloc: memory allocator
///   - dealloc: memory free function
///   - comp: function used to compare two values
///
/// Returns:
///   A pointer to a tree or NULL if the memory allocation fails
Tree *tree_init(const size_t elem_size, const TreeAllocFn alloc, const TreeFreeFn dealloc, const TreeComparator comp);



/// Insert a value into a tree
///
/// This function inserts a value into the tree. The value is copied and should
/// have the same size that was specified in `tree_init`. The behaivor for different
/// sized values is NOT DEFINED.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be inserted
///   - value_size: size of the value that will inserted into the tree
void tree_insert(Tree *tree, const void *value, const size_t value_size);

/// Look up a value in a tree
///
/// This function looksup values in the tree and returns a pointer to the data.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be looked up
///
/// Returns:
///   a pointer to the value if it was found and NULL other wise.
const void *tree_lookup(const Tree *tree, const void *value);

/// Delete a value from the tree 
///
/// This function deletes a value from the tree if it is found.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - value: pointer to the value that needs to be deleted
void tree_delete(Tree *tree, const void *value);

/// Free the entire tree
///
/// This function frees the tree according to [dealloc] which was specified
/// in `tree_init`. The tree should not be used after it has been freed.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
void tree_free(Tree *tree);


/********************* Helper functions for macro wrapper *********************/

/// Initialize a buffer for temporary values
///
/// This function initializes a temporary buffer
/// where values can go.
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///
/// Returns:
///   A buffer that is allocated according to [tree]'s alloc function
void *tree_init_buf(const Tree *tree);



/// Free temporary buffer
///
/// This function frees a temporary buffer according to [tree]'s 
/// dealloc function
///
/// Parameters:
///   - tree: handle to a tree that was returned by `tree_init`
///   - buf: buffer that was returned by `tree_init_buf
void tree_free_buf(const Tree *tree, void *buf);



/******************************* Macro wrapper ********************************/

/// Tree of a type (wrapper struct)
///
/// This macro works like a typedef with a parameter
///
/// Example:
///   TreeOf(int) tree = {0}; // Tree of integers
#define TreeOf(T) struct { Tree *tree; T *buf }


/// Put a value into a tree
///
/// Example:
///   TreeOf(int) cool_tree = {0}; // Tree of integers
///   tree_put(cool_tree, 1024);
#define tree_put(tree, value) \
    if (tree.tree == NULL || tree.buf == NULL) { \
        tree.tree = tree_init(sizeof(*tree.buf), malloc, free, memcmp); \
        tree.buf = tree_init_buf(tree.tree); \
    } \
    if (tree.tree != NULL && tree.buf != NULL) { \
        *tree.buf = value; \
        tree_insert(tree.tree, tree.buf, sizeof(*tree.buf)); \
    }


/// Get a value from a tree
///
///
/// Example:
///   TreeOf(int) cool_tree = {0};
///   tree_put(cool_tree, 1024);
///   tree_get(int *value, cool_tree, 1024); // You can also pass in already declared variables.
///   assert(value != NULL);
#define tree_get(variable, tree, value) \
    if (tree.buf != NULL) { \
        *tree.buf = value; \
    } \
    variable = tree_lookup(tree, tree.buf)

/// Remove a value from a tree
///
/// Example:
///   TreeOf(int) cool_tree = {0};
///   tree_put(cool_tree, 1024);
///   tree_remove(cool_tree, 1024);
#define tree_remove(tree, value) \
    if (tree.buf != NULL) { \
        *tree.buf = value; \
    } \
    tree_delete(tree, tree.buf)





/// Helper function for tree_deletion/tree_freeing
static inline void _tree_del(Tree *tree, void *buf) {
    if (tree != NULL && buf != NULL) {
        tree_free_buf(tree, buf);
        tree_free(tree);
    }
}
/// Free the entire tree.
///
/// Example:
///   TreeOf(int) cool_tree = {0};
///   tree_put(cool_tree, 1024);
///   tree_remove(cool_tree, 1024);
///   tree_del(cool_tree)
#define tree_del(tree) _tree_del(tree.tree, tree.buf)




#endif
