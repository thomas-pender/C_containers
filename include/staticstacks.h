/**
 * @file staticstacks.h
 * @brief Public interface for a generic stack of fixed capacity.
 *
 * Public interface for a generic stack of fixed capacity.
 *
 * @warning This is meant to be a lightweight and fast implementation of a fixed
 * capacity stack object. As such, various checks for out-of-bounds-type errors
 * are not implemented. It is left to the user for responsible use of the data
 * structure.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_STATICSTACKS_H
# define INCLUDED_STATICSTACKS_H

# include <stdlib.h>
# include <stddef.h>
# include <string.h>

/**
 * @brief Static struct object.
 *
 * Static struct object.
 */
typedef struct {
  size_t size; ///< size in bytes of data objects
  size_t nmem; ///< number of elements currently in stack
  char *x;     ///< data array
  char *top;   ///< one past head of data array
} sstacks_t;

/**
 * @brief Instantiates a static stack instance.
 *
 * Instantiates a static stack instance.
 *
 * @param[in,out] s Static stack instance. Should be <tt>NULL</tt>.
 * @param[in] size Size in bytes of data objects.
 * @param[in] capacity Maximum number of objects the stack will hold.
 */
# define sstacks_new(s, n, capacity) do {                           \
    (s) = (sstacks_t*)malloc(sizeof(sstacks_t));                    \
    (s)->size = (n);                                                \
    (s)->nmem = 0;                                                  \
    (s)->x = (char*)malloc(((capacity) + 1) * (n) * sizeof(char));  \
    (s)->top = (s)->x;                                              \
  } while (0)

/**
 * @brief Push data object onto static stack.
 *
 * Push data object onto static stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>sstacks_push</tt> on a <tt>NULL</tt> stack.</dd>
 * <dd>Pushing onto a stack already at capacity.</dd>
 * </dl>
 *
 * @param[in] s Static stack instance.
 * @param[in] x Data object being copied onto top of stack.
 */
# define sstacks_push(s, x) ((void) (memcpy((s)->top, x, (s)->size), (s)->top += (s)->size, (s)->nmem++))

/**
 * @brief Pop and return top of static stack.
 *
 * Pop and return top of static stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>sstacks_pop</tt> on a <tt>NULL</tt> stack.</dd>
 * <dd>Calling <tt>sstacks_pop</tt> on an empty stack.</dd>
 * </dl>
 *
 * @param[in] s Static stack.
 *
 * @return <tt>char*</tt> pointer to top element of array.
 */
# define sstacks_pop(s) ((s)->nmem--, (s)->top -= (s)->size, (s)->top)

/**
 * @brief Check if static stack is empty.
 *
 * Check if static stack is empty.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>sstacks_empty</tt> on a <tt>NULL</tt> stack.</dd>
 * </dl>
 *
 * @param[in] s Static stack.
 *
 * @return Boolean value indicating whether <tt>top</tt> is the tail of the stack.
 */
# define sstacks_empty(s) (((s)->x) == ((s)->top))

/**
 * @brief Free memory allocated for stack.
 *
 * Free memory allocated for stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>sstacks_free</tt> on a <tt>NULL</tt> stack.</dd>
 * </dl>
 *
 * @param[in] s Static stack.
 */
# define sstacks_free(s) (free((s)->x), free(s))

/**
 * @brief Number of elements in a static stack.
 *
 * Number of elements in a static stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>sstacks_size</tt> on a <tt>NULL</tt> stack.</dd>
 * </dl>
 *
 * @param[in] s Static stack.
 */
# define sstacks_size(s) ((s)->nmem)

# endif
