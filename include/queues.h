/**
 * @file queues.h
 * @brief Public interface of <tt>queues_t</tt> class
 *
 * The <tt>queues_t</tt> object instantiates shallow queue-type associations
 * between already existing data. The user is responsible for allocating the data
 * previous to envoking <tt>queues_new</tt> to generate a <tt>queues_t</tt>
 * instance. The user is also responsible for deallocating the data. The routine
 * <tt>queues_free</tt> only deallocates the links between data created by envoking
 * <tt>queues_new</tt> and <tt>queues_enqueue</tt>.
 *
 * Repetitions are not allowed, and data must be linearly ordered.
 *
 * The <tt>queues_t</tt> class is implemented as an opaque pointer.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_QUEUES_H
# define INCLUDED_QUEUES_H

# include <stdlib.h>
# include <stddef.h>
# include <errno.h>
# include <error.h>

typedef struct queues_t* queues_t;

/**
 * @brief User provided compare function. Must provide a linear ordering of the
 * data.
 */
typedef int (*queues_data_cmp)(const void*, const void*);

/**
 * @brief User provided reentrant compare function. Must provide a linear ordering
 * of the data.
 */
typedef int (*queues_data_cmp_r)(const void*, const void*, void*);

/**
 * @brief Instantiates a <tt>queues_t</tt> instance.
 *
 * Memory is allocated for a new <tt>queues_t</tt> instance. This memory needs to
 * be freed by a call to <tt>queues_free</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>One of the <tt>queues_data_cmp</tt> or <tt>queues_data_cmp_r</tt>
 * arguments must be non<tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] cmp User function to compare relative ordering of two data objects.
 * @param[in] cmp_r Reentrant version of <tt>cmp</tt>.
 *
 * @return Instance of queue object.
 */
extern queues_t queues_new(queues_data_cmp cmp, queues_data_cmp_r cmp_r);

/**
 * @brief Inserts pointer to data object into queue object.
 *
 * If a data object equal to the user provided data parameter is not present in the
 * queue object, then a pointer is created and added to the queue which points to
 * the data. The queue remains in sorted order according to the user provided
 * compare function.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_enqueue</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being added to.
 * @param[in] x Pointer to data being added to queue object.
 */
extern void queues_enqueu(queues_t q, const void *x);

/**
 * @brief Inserts pointer to data object into queue object.
 *
 * Reentrant version of <tt>queues_enqueue</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_enqueue_r</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being added to.
 * @param[in] x Pointer to data being added to queue object.
 * @param[in] y Argument to user provided reentrant compare function.
 */
extern void queues_enqueu_r(queues_t q, const void *x, void *y);

/**
 * @brief Remove object from front of queue object.
 *
 * A pointer to the data object at the head of the queue is returned. The head of
 * the queue is incremented to the next element of the list.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_dequeue_front</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being popped.
 *
 * @return pointer to data at head of queue object.
 */
extern void *queues_dequeue_front(queues_t q);

/**
 * @brief Remove object from tail of queue object.
 *
 * A pointer to the data object at the tail of the queue is returned. The tail of
 * the queue is decremented to the next element of the list.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_dequeue_back</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being popped.
 *
 * @return pointer to data at tail of queue object.
 */
extern void *queues_dequeue_back(queues_t q);

/**
 * @brief Check if data equal to user provided data object is contained in queue
 * object.
 *
 * Check if data equal to user provided data object is contained in queue
 * object. Return pointer to data if found; otherwise, <tt>NULL</tt> is returned.
 *
 * </dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_find</tt> on <tt>NULL</tt> queue object.</dd>
 * <dd>Neither the <tt>queues_data_cmp</tt> field nor the
 * <tt>queues_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] q Queue object being searched.
 * @param[in] x Data whose membership is being checked.
 *
 * @return void* Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *queues_find(queues_t q, const void *x);

/**
 * @brief Check if data equal to user provided data object is contained in queue
 * object.
 *
 * Reentrant version of <tt>queues_find</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_find_r</tt> on <tt>NULL</tt> queue object.</dd>
 * <dd>Neither the <tt>queues_data_cmp</tt> field nor the
 * <tt>queues_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] q Queue object being searched.
 * @param[in] x Data whose membership is being checked.
 * @param[in] y Argument to user provided compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *queues_find_r(queues_t q, const void *x, void *y);

/**
 * @brief Remove pointer to data object in queue object (if present) equal in value
 * to the data object parameter provided by the user.
 *
 * The queue object is searched for data equal in value to the user provided data
 * object. If found, the pointer is removed from the queue and returned to the
 * user. If not found, <tt>NULL</tt> is returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_remove</tt> on <tt>NULL</tt> queues object.</dd>
 * </dl>
 *
 * @param[in] q Queues object being altered.
 * @param[in] x Pointer to data object being searched for.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *queues_remove(queues_t q, const void *x);

/**
 * @brief Remove pointer to data object in queue object (if present) equal in value
 * to the data object parameter provided by the user.
 *
 * Reentrant version of <tt>queues_remove</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_remove_r</tt> on <tt>NULL</tt> queues object.</dd>
 * </dl>
 *
 * @param[in] q Queues object being altered.
 * @param[in] x Pointer to data object being searched for.
 * @param[in] y Argument to user provided compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *queues_remove_r(queues_t q, const void *x, void *y);

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
 * <dd>Calling <tt>queues_map</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being acted upon.
 * @param[in] apply Function being applied to members of the queue.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int queues_map(queues_t q, int apply(void **x));

/**
 * @brief Apply function to every member of queue object.
 *
 * Reentrant version of <tt>queues_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_map_r</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being acted upon.
 * @param[in] apply Function being applied to members of the queue.
 * @param[in] y Argument to user provided function apply.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int queues_map_r(queues_t q, int apply(void **x, void *y), void *y);

/**
 * @brief Free data allocated for the shallow queues-type associations.
 *
 * The shallow links constructed between the existing data allocated by the user
 * are freed. The data pointed to by the structure is not freed. It is the
 * responsibility of the user to deallocate the underlying data.
 *
 * @param[in] *q Pointer to <tt>queues_t</tt> object.
 */
extern void queues_free(queues_t *q);

/**
 * @brief Number of elements in queue.
 *
 * Number of elements in queue.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>queues_size</tt> on a <tt>NULL</tt> queue object.</dd>
 * </dl>
 *
 * @param[in] q Queue object being checked.
 *
 * @return Number of members of the queue.
 */
extern size_t queues_size(queues_t q);

/**
 * @brief Swap opaque pointers for queues.
 *
 * Swap opaque pointers for queues.
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
void queues_swap(queues_t *restrict q1, queues_t *restrict q2)
{
  volatile queues_t tmp = *q1;
  *q1 = *q2;
  *q2 = tmp;
}

# endif
