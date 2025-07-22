/**
 * @file deepqueues.h
 * @brief Public interface of <tt>dqueues_t</tt> class
 *
 * The <tt>dqueues_t</tt> object instantiates deep queue-type associations
 * between already existing data. The user is responsible for allocating the data
 * previous to envoking <tt>dqueues_new</tt> to generate a <tt>dqueues_t</tt>
 * instance. The function <tt>dqueues_enqueue</tt> copies the data passed to it by
 * the user.
 *
 * Repetitions are not allowed, and data must be linearly ordered.
 *
 * The <tt>dqueues_t</tt> class is implemented as an opaque pointer.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_DEEPQUEUES_H
# define INCLUDED_DEEPQUEUES_H

# include <stdlib.h>
# include <stddef.h>
# include <errno.h>
# include <error.h>
# include <string.h>

typedef struct dqueues_t* dqueues_t;

/**
 * @brief User provided compare function. Must provide a linear ordering of the
 * data.
 */
typedef int (*dqueues_data_cmp)(const void*, const void*);

/**
 * @brief User provided reentrant compare function. Must provide a linear ordering
 * of the data.
 */
typedef int (*dqueues_data_cmp_r)(const void*, const void*, void*);

/**
 * @brief Instantiates a <tt>dqueues_t</tt> instance.
 *
 * Memory is allocated for a new <tt>queues_t</tt> instance. This memory needs to
 * be freed by a call to <tt>queues_free</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>One of the <tt>dqueues_data_cmp</tt> or <tt>queues_data_cmp_r</tt>
 * arguments must be non<tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] cmp User function to compare relative ordering of two data objects.
 * @param[in] cmp_r Reentrant version of <tt>cmp</tt>.
 *
 * @return Instance of queue object.
 */
extern dqueues_t dqueues_new(dqueues_data_cmp cmp, dqueues_data_cmp_r cmp_r,
                             size_t size);

/**
 * @brief Inserts deep copied data object into queue object.
 *
 * If a data object equal to the user provided data parameter is not present in the
 * queue object, then a deep copied data object is created and added to the queue
 * The queue remains in sorted order according to the user provided compare
 * function.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_enqueue</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being added to.
 * @param[in] x Pointer to data being added to queue object.
 */
extern void dqueues_enqueu(dqueues_t q, const void *x);

/**
 * @brief Inserts pointer to data object into queue object.
 *
 * Reentrant version of <tt>dqueues_enqueue</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_enqueue_r</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being added to.
 * @param[in] x Pointer to data being added to queue object.
 * @param[in] y Argument to user provided reentrant compare function.
 */
extern void dqueues_enqueu_r(dqueues_t q, const void *x, void *y);

/**
 * @brief Remove object from front of queue object.
 *
 * A pointer to the data object at the head of the queue is returned. The head of
 * the queue is incremented to the next element of the list.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_dequeue_front</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being popped.
 *
 * @return Pointer to data at head of queue object.
 */
extern void *dqueues_dequeue_front(dqueues_t q);

/**
 * @brief Remove object from tail of queue object.
 *
 * A pointer to the data object at the tail of the queue is returned. The tail of
 * the queue is decremented to the next element of the list.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_dequeue_back</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being popped.
 *
 * @return Pointer to data at tail of queue object.
 */
extern void *dqueues_dequeue_back(dqueues_t q);

/**
 * @brief Check if data equal to user provided data object is contained in queue
 * object.
 *
 * Check if data equal to user provided data object is contained in queue
 * object. Return pointer to data if found; otherwise, <tt>NULL</tt> is returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_find</tt> on <tt>NULL</tt> queue object.</dd>
 * <dd>Neither the <tt>dqueues_data_cmp</tt> field nor the
 * <tt>dqueues_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] q Queue object being searched.
 * @param[in] x Data whose membership is being checked.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dqueues_find(dqueues_t q, const void *x);

/**
 * @brief Check if data equal to user provided data object is contained in queue
 * object.
 *
 * Reentrant version of <tt>dqueues_find</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_find_r</tt> on <tt>NULL</tt> queue object.</dd>
 * <dd>Neither the <tt>dqueues_data_cmp</tt> field nor the
 * <tt>dqueues_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] q Queue object being searched.
 * @param[in] x Data whose membership is being checked.
 * @param[in] y Argument to user provided compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dqueues_find_r(dqueues_t q, const void *x, void *y);

/**
 * @brief Remove data object in queue object (if present) equal in value to the
 * data object parameter provided by the user.
 *
 * The queue object is searched for data equal in value to the user provided data
 * object. If found, the data object is removed from the queue and a pointer
 * returned to the user. If not found, <tt>NULL</tt> is returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_remove</tt> on <tt>NULL</tt> queues object.</dd>
 * </dl>
 *
 * @param[in] q Queues object being altered.
 * @param[in] x Pointer to data object being searched for.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dqueues_remove(dqueues_t q, const void *x);

/**
 * @brief Remove data object in queue object (if present) equal in value to the
 * data object parameter provided by the user.
 *
 * Reentrant version of <tt>dqueues_remove</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_remove_r</tt> on <tt>NULL</tt> queues object.</dd>
 * </dl>
 *
 * @param[in] q Queues object being altered.
 * @param[in] x Pointer to data object being searched for.
 * @param[in] y Argument to user provided compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dqueues_remove_r(dqueues_t q, const void *x, void *y);

/**
 * @brief Apply function to every member of queue object.
 *
 * The function <tt>apply</tt> is applied to every member of the queue. Early
 * termination is possible if <tt>apply</tt> returns a negative <tt>int</tt>. In
 * all other cases, we continue to apply <tt>apply</tt> to the succeeding data
 * object (if any) of the queue.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_map</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being acted upon.
 * @param[in] apply Function being applied to members of the queue.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dqueues_map(dqueues_t q, int apply(void **x));

/**
 * @brief Apply function to every member of queue object.
 *
 * Reentrant version of <tt>dqueues_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_map_r</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being acted upon.
 * @param[in] apply Function being applied to members of the queue.
 * @param[in] y Argument to user provided function apply.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dqueues_map_r(dqueues_t q, int apply(void **x, void *y), void *y);

/**
 * @brief Free data allocated for the deep queues-type associations.
 *
 * The deep copied data and associations are freed.
 *
 * @param[in] *q Pointer to <tt>dqueues_t</tt> object.
 */
extern void dqueues_free(dqueues_t *q);

/**
 * @brief Number of elements in queue.
 *
 * Number of elements in queue.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dqueues_size</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being checked.
 *
 * @return Number of members of the queue.
 */
extern size_t dqueues_size(dqueues_t q);

/**
 * @brief Swap opaque pointers for deep queues.
 *
 * Swap opaque pointers for deep queues.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Queue objects are aliases.</dd>
 * </dl>
 *
 * @param[in] q1 First queue.
 * @param[in] q2 Second queue.
 */
static inline
void dqueues_swap(dqueues_t *restrict q1, dqueues_t *restrict q2)
{
  volatile dqueues_t tmp = *q1;
  *q1 = *q2;
  *q2 = tmp;
}

# endif
