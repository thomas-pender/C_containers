/**
 * @file deepstacks.h
 * @brief Public interface of <tt>dstacks_t</tt> class.
 *
 * The <tt>dstacks_t</tt> object instantiates deep stack-type associations
 * between already existing data. The user is responsible for allocating the data
 * previous to envoking <tt>dstacks_new</tt> to generate a <tt>dstack_t</tt>
 * instance. The function <tt>dstack_push</tt> copies the data passed to it by the
 * user.
 *
 * The <tt>dstacks_t</tt> class is implemented as an opaque pointer.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_DEEPSTACKS_H
# define INCLUDED_DEEPSTACKS_H

# include <stdlib.h>
# include <stddef.h>
# include <string.h>

typedef struct dstacks_t* dstacks_t;

/**
 * @brief Instantiate a <tt>dstacks_t</tt> instance.
 *
 * Memory is allocated for a new <tt>dstacks_t</tt> instance. This memory needs to
 * be freed by a call to <tt>dstacks_free</tt>.
 *
 * <dl>
 * <dt><strong>Uncheck Runtime Error</strong></dt>
 * <dd>The size parameter is unequal to the total size of the data.</dd>
 * </dl>
 *
 * @param[in] size The total size of the data objects.
 *
 * @return Opaque pointer to stack object.
 */
extern dstacks_t dstacks_new(size_t size);

/**
 * @brief Push existing data onto stack.
 *
 * A data object is deep copyied and placed on top of the stack.
 *
 * <dl>
 * <dt><strong>Uncheck Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_push</tt> with a <tt>NULL</tt> <tt>dstacks_t</tt>
 * parameter <tt>s</tt>.</dd>
 * </dl>
 *
 * @param[in] s Stack object being pushed upon.
 * @param[in] x Pointer to data being copied and pushed onto top of stack object
 * <tt>s</tt>.
 */
extern void dstacks_push(dstacks_t s, void *x);

/**
 * @brief Pop data object on top of stack and return to user.
 *
 * A pointer to the data object on the top of stack is returned. The head of the
 * stack moves to the next data object of the stack. If there are no further data
 * objects, the head is <tt>NULL</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_pop</tt> with a <tt>NULL</tt> <tt>dstacks_t</tt>
 * parameter <tt>s</tt>.</dd>
 * <dd>Calling <tt>dstacks_pop</tt> on an empty <tt>dstacks_t</tt> parameter
 * <tt>s</tt>.</dd>
 * </dl>
 *
 * @param[in] s Stack object being popped.
 *
 * @return Pointer to top data.
 */
extern char *dstacks_pop(dstacks_t s);

/**
 * @brief Free data allocated for the deep stacks-type associations.
 *
 * The deep copied data and associations are freed.
 *
 * @param[in] *s Pointer to <tt>dstacks_t</tt> object.
 */
extern void dstacks_free(dstacks_t *s);

/**
 * @brief Check if <tt>dstacks_t</tt> object is empty.
 *
 * Check if <tt>dstacks_t</tt> object is empty.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_empty</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being checked.
 *
 * @return True if empty. False otherwise.
 */
extern int dstacks_empty(dstacks_t s);

/**
 * @brief Number of elements in stack.
 *
 * Number of elements in stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_size</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being checked.
 *
 * @return Number of members of the stack.
 */
extern size_t dstacks_size(dstacks_t s);

/**
 * @brief Apply function to every member of stack object.
 *
 * The function <tt>apply</tt> is applied to every member of the stack. Early
 * termination is possible if <tt>apply</tt> returns a negative <tt>int</tt>. In
 * all other cases, we continue to apply <tt>apply</tt> to the succeeding data
 * object (if any) of the stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_map</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being acted upon.
 * @param[in] apply Function being applied to members of the stack.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dstacks_map(dstacks_t s, int apply(void **x));

/**
 * @brief Apply function to every member of stack object.
 *
 * Reentrant version of <tt>dstacks_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dstacks_map</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being acted upon.
 * @param[in] apply Function being applied to members of the stack.
 * @param[in] y Parameter to <tt>apply</tt> function.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dstacks_map_r(dstacks_t s, int apply(void **x, void *y), void *y);

/**
 * @brief Swap opaque pointers for deep stacks.
 *
 * Swap opaque pointers for deep stacks.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Stack objects are aliases.</dd>
 * </dl>
 *
 * @param[in] s1 First stack.
 * @param[in] s2 Second stack.
 */
static inline
void dstacks_swap(dstacks_t *restrict s1, dstacks_t *restrict s2)
{
  volatile dstacks_t tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}

# endif
