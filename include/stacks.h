/**
 * @file stacks.h
 * @brief Public interface of <tt>stacks_t</tt> class.
 *
 * The <tt>stacks_t</tt> object instantiates shallow stack-type associations
 * between already existing data. The user is responsible for allocating the data
 * previous to envoking <tt>stacks_new</tt> to generate a <tt>stack_t</tt>
 * instance. The user is also responsible for deallocating the data. The routine
 * <tt>stacks_free</tt> only deallocates the links between data created by envoking
 * <tt>stacks_new</tt> and <tt>stacks_push</tt>.
 *
 * The <tt>stacks_t</tt> class is implemented as an opaque pointer.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_STACKS_H
# define INCLUDED_STACKS_H

# include <stdlib.h>
# include <stddef.h>

typedef struct stacks_t* stacks_t;

/**
 * @brief Instantiate a <tt>stacks_t</tt> instance.
 *
 * Memory is allocated for a new <tt>stacks_t</tt> instance. This memory needs to
 * be freed by a call to <tt>stacks_free</tt>.
 *
 * @return Opaque pointer to stack object.
 */
extern stacks_t stacks_new(void);

/**
 * @brief Push existing data onto stack.
 *
 * A pointer is created which points to the existing data given by the user. This
 * pointer is placed on the top of stack.
 *
 * <dl>
 * <dt><strong>Uncheck Runtime Errors</strong></dt>
 * <dd>Calling <tt>stacks_push</tt> with a <tt>NULL</tt> <tt>stacks_t</tt>
 * parameter <tt>s</tt>.</dd>
 * </dl>
 *
 * @param[in] s Stack object being pushed upon.
 * @param[in] x Pointer to data being pushed onto top of stack object
 * <tt>s</tt>.
 */
extern void stacks_push(stacks_t s, void *x);

/**
 * @brief Pop data object on top of stack and return to user.
 *
 * A pointer to the data object on the top of stack is returned. The head of the
 * stack moves to the next data object of the stack. If there are no further data
 * objects, the head is <tt>NULL</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>stacks_pop</tt> with a <tt>NULL</tt> <tt>stacks_t</tt>
 * parameter <tt>s</tt>.</dd>
 * <dd>Calling <tt>stacks_pop</tt> on an empty <tt>stacks_t</tt> parameter
 * <tt>s</tt>.</dd>
 * </dl>
 *
 * @param[in] s Stack object being popped.
 *
 * @return Pointer to top data.
 */
extern void *stacks_pop(stacks_t s);

/**
 * @brief Free data allocated for the shallow stacks-type associations.
 *
 * The shallow links constructed between the existing data allocated by the user
 * are freed. The data pointed to by the structure is not freed. It is the
 * responsibility of the user to deallocate the underlying data.
 *
 * @param[in] *s Pointer to <tt>stacks_t</tt> object.
 */
extern void stacks_free(stacks_t *s);

/**
 * @brief Check if <tt>stacks_t</tt> object is empty.
 *
 * Check if <tt>stacks_t</tt> object is empty.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>stacks_empty</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being checked.
 *
 * @return True if empty. False otherwise.
 */
extern int stacks_empty(stacks_t s);

/**
 * @brief Number of elements in stack.
 *
 * Number of elements in stack.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>stacks_size</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being checked.
 *
 * @return Number of members of the stack.
 */
extern size_t stacks_size(stacks_t s);

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
 * <dd>Calling <tt>stacks_map</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being acted upon.
 * @param[in] apply Function being applied to members of the stack.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int stacks_map(stacks_t s, int apply(void **x));

/**
 * @brief Apply function to every member of stack object.
 *
 * Reentrant version of <tt>stacks_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>stacks_map</tt> on a <tt>NULL</tt> stack object.</dd>
 * </dl>
 *
 * @param[in] s Stack object being acted upon.
 * @param[in] apply Function being applied to members of the stack.
 * @param[in] y Parameter to <tt>apply</tt> function.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int stacks_map_r(stacks_t s, int apply(void **x, void *y), void *y);

/**
 * @brief Swap opaque pointers for stacks.
 *
 * Swap opaque pointers for stacks.
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
void stacks_swap(stacks_t *restrict s1, stacks_t *restrict s2)
{
  volatile stacks_t tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}

# endif
