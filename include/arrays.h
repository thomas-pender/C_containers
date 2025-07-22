/**
 * @file arrays.h
 * @brief Public interface of <tt>arrays_t</tt> class
 *
 * The <tt>arrays_t</tt> object instantiates a dynamic array. The user is
 * responsible for allocating the data previous to envoking <tt>dhashtabs_new</tt>
 * to generate an <tt>arrays_t</tt> instance. The function <tt>arrays_push</tt>
 * copies the data passed to it by the user.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_ARRAYS_H
# define INCLUDED_ARRAYS_H

# include <stddef.h>
# include <stdlib.h>
# include <errno.h>
# include <error.h>
# include <string.h>

typedef struct arrays_t* arrays_t;

/**
 * @brief Instantiates an <tt>arrays_t</tt> instance.
 *
 * Memory is allocated for a new <tt>arrays_t</tt> instance. This memory needs
 * to be freed by a call to <tt>arrays_free</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Size parameter is not equal to totoal size of data.</dd>
 * </dl>
 *
 * @param[in] size Total size of data objects.
 * @param[in] capacity Initialize array to hold <tt>capacity</tt> elements.
 *
 * @return Instance of an allocated array object.
 */
extern arrays_t arrays_new(size_t size, size_t capacity);

/**
 * @brief Copies data object to array.
 *
 * The supplied data object is deep copied to the end of the array object. If the
 * copy was successfull, 1 is returned. -1 is returned if the array object is at
 * capacity.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_push</tt> on a <tt>NULL</tt> array object.</dd>
 * </dl>
 *
 * @param[in] a Array object being push onto.
 * @param[in] x Data object being copied into array.
 *
 * @return 1 upon successfull pushing. -1 if array is at capacity.
 */
extern int arrays_push(arrays_t a, const void *x);

/**
 * @brief Copies data object to array.
 *
 * The supplied data object is deep copied to the end of the array object. If the
 * array is already at capacity, then the array is resized before pushing the new
 * data object onto the tail of the array.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_push</tt> on a <tt>NULL</tt> array object.</dd>
 * </dl>
 *
 * @param[in] a Array object being push onto.
 * @param[in] x Data object being copied into array.
 */
extern void arrays_dynpush(arrays_t a, const void *x);

/**
 * @brief Applies user defined function to each element of the array.
 *
 * Applies user defined function to each element of the array. Early termination is
 * possible when <tt>apply</tt> return a negative value. Otherwise, the function
 * continues by applying <tt>apply</tt> to the next (if any) element of the array.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_map</tt> on a <tt>NULL</tt> array object.</dd>
 * </dl>
 *
 * @param[in] a Array object being acted upon.
 * @param[in] apply User define function being applied to every element of the
 * array.
 *
 * @return -1 if early termination. 1 if completed without terminating early.
 */
extern int arrays_map(arrays_t a, int apply(void *x));

/**
 * @brief Applies user defined function to each element of the array.
 *
 * Reentrant version of <tt>arrays_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_map_r</tt> on a <tt>NULL</tt> array object.</dd>
 * </dl>
 *
 * @param[in] a Array object being acted upon.
 * @param[in] apply User define function being applied to every element of the
 * array.
 * @param[in] y Argument to user defined function <tt>apply</tt>.
 *
 * @return -1 if early termination. 1 if completed without terminating early.
 */
extern int arrays_map_r(arrays_t a, int apply(void *x, void *y), void *y);

/**
 * @brief Returns pointer to element of array.
 *
 * Returns pointer to element of array.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_at</tt> on a <tt>NULL</tt> array object.</dd>
 * <dd>Position index out of bounds.</dd>
 * </dl>
 *
 * @param[in] a Array object being searched.
 * @param[in] i Position index.
 *
 * @return Pointer to <tt>i</tt>th element of array object.
 */
extern void *arrays_at(arrays_t a, size_t i);

/**
 * @brief Binary search of array.
 *
 * Binary search of array. The array should already be sorted in ascending order
 * with respect to user provided compare function. If found, a pointer to the
 * data object is returned. If not found, <tt>NULL</tt> returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_bsearch</tt> on a <tt>NULL</tt> array object.</dd>
 * <dd>Key is <tt>NULL</tt>.</dd>
 * <dd>Compare function is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] a Array object being searched.
 * @param[in] x Key of object being searched for.
 * @param[in] cmp User defined compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> if not found.
 */
extern void *arrays_bsearch(arrays_t a, const void *x,
                            int cmp(const void *x, const void *y));

/**
 * @brief Sort contends of the array according to user defined compare function.
 *
 * Sort contends of the array according to user defined compare function.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_qsort</tt> on a <tt>NULL</tt> array object.</dd>
 * <dd>Compare function is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] a Array object being sorted.
 * @param[in] cmp User defined compare function.
 */
extern void arrays_sort(arrays_t a, int cmp(const void *x, const void *y));

/**
 * @brief Sort contends of the array according to user defined compare function.
 *
 * Reentrant version of <tt>arrays_qsort</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_qsort_r</tt> on a <tt>NULL</tt> array object.</dd>
 * <dd>Compare function is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] a Array object being sorted.
 * @param[in] cmp User defined compare function.
 * @param[in] z Argument to reentrant user defined compare function.
 */
extern void arrays_sort_r(arrays_t a,
                          int cmp(const void *x, const void *y, void *z), void *z);

/**
 * @brief Change size of dynamic array object.
 *
 * Change size of dynamic array object. If changed to a larger size, the contents
 * are copied. If changed to a smaller size, the elements are truncated.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_resize</tt> on a <tt>NULL</tt> array object.</dd>
 * </dl>
 *
 * @param[in] a Array object being resized.
 * @param[in] nmem Number of possible elements in new array object.
 */
extern void arrays_resize(arrays_t a, size_t nmem);

/**
 * @brief Free data allocated for the dynamic array.
 *
 * Free data allocated for the dynamic array.
 *
 * @param[in] *a Pointer to <tt>arrays_t</tt> object.
 */
extern void arrays_free(arrays_t *a);

/**
 * @brief Return number of elements in array object.
 *
 * Return number of elements in array object.
 *
 * @param[in] a Array object.
 *
 * @return Number of elements in array.
 */
extern size_t arrays_nmem(arrays_t a);

/**
 * @brief Return number of possible elements in array object.
 *
 * Return number of possible elements in array object.
 *
 * @param[in] a Array object.
 *
 * @return Capacity of array object.
 */
extern size_t arrays_capacity(arrays_t a);

/**
 * @brief Return size of elements in array object.
 *
 * Return size of elements in array object.
 *
 * @param[in] a Array object.
 *
 * @return Size of array elements.
 */
extern size_t arrays_size(arrays_t a);

/**
 * @brief Check if contents of two arrays are equal in value.
 *
 * Check if contents of two arrays are equal in value.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>arrays_equal</tt> when any parameter is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] a1 First array.
 * @param[in] a2 Second array.
 *
 * @return -1 if not equal. 1 if equal.
 */
extern int arrays_equal(arrays_t a1, arrays_t a2);

/**
 * @brief Swap opaque pointers for arrays.
 *
 * Swap opaque pointers for arrays.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Array objects are aliases.</dd>
 * </dl>
 *
 * @param[in] a1 First array.
 * @param[in] a2 Second array.
 */
static inline
void arrays_swap(arrays_t *restrict a1, arrays_t *restrict a2)
{
  volatile arrays_t tmp = *a1;
  *a1 = *a2;
  *a2 = tmp;
}

# endif
