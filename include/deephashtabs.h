/**
 * @file deephashtabs.h
 * @brief Public interface of <tt>dhashtabs_t</tt> class
 *
 * The <tt>dhashtabs_t</tt> object instantiates deep hash-table-type associations
 * between already existing data. The user is responsible for allocating the data
 * previous to envoking <tt>dhashtabs_new</tt> to generate a <tt>dhashtabs_t</tt>
 * instance. The function <tt>dhashtabs_insert</tt> copies the data passed to it by
 * the user.
 *
 * The <tt>dqueues_t</tt> data structure of this same library is used for the
 * hash tables' buckets.
 *
 * Repetitions are not allowed, and data must be linearly ordered.
 *
 * The <tt>dhashtabs_t</tt> class is implemented as an opaque pointer.
 *
 * @warning This class is meant to be used in time restricted settings. As such,
 * some runtime errors, such as out-of-bounds-type errors, are note explicitly
 * checked for. It is left to the user to use the interface responsibly.
 * @author Thomas Pender
 * @date 2025-07
 * @copyright GNU Public License
 */
# ifndef INCLUDED_DEEPHASHTABS_H
# define INCLUDED_DEEPHASHTABS_H

# include <stdint.h>
# include <stdlib.h>
# include <stddef.h>

typedef struct dhashtabs_t* dhashtabs_t;

/**
 * @brief User provided compare function. Must provide a linear ordering of the
 * data.
 */
typedef int (*dhashtabs_data_cmp)(const void*, const void*);

/**
 * @brief User provided reentrant compare function. Must provide a linear ordering
 * of the data.
 */
typedef int (*dhashtabs_data_cmp_r)(const void*, const void*, void*);

/**
 * @brief User provided hashing function.
 */
typedef uint64_t (*dhashtabs_hash)(const void*);

/**
 * @brief User provided reentrant hashing function.
 */
typedef uint64_t (*dhashtabs_hash_r)(const void*, const void*);

/**
 * @brief Standard hash function for dynamic arrays of <tt>uint32_t</tt> unsigned
 * integer type.
 *
 * Standard hash function for dynamic arrays of <tt>uint32_t</tt> unsigned
 * integer type. Implements a version from the Boost library.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>If the size parameter is larger than the number of elements of array, the
 * function reads past the end of the array.</dd>
 * </dl>
 *
 * @param[in] _a Pointer to array of <tt>uint32_t</tt> unsigned integers.
 * @param[in] _n Pointer to size_type containing number of elements in array.
 *
 * @return Hash value of array <tt>_a</tt>.
 */
extern uint64_t dhashtabs_stdhash(const void *_a, const void *_n);

/**
 * @brief Instantiates a <tt>dhashtabs_t</tt> instance.
 *
 * Memory is allocated for a new <tt>dhashtabs_t</tt> instance. This memory needs
 * to be freed by a call to <tt>dhashtabs_free</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Both <tt>dhashtabs_data_cmp</tt> and <tt>dhashtabs_data_cmp_r</tt>
 * arguments are <tt>NULL</tt>.</dd>
 * <dd>Both <tt>dhashtabs_hash</tt> and <tt>dhashtabs_hash_r_r</tt> arguments
 * are <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] cmp User provided compare function.
 * @param[in] cmp_r User provided reentrant compare function.
 * @param[in] hash User provided hashing function.
 * @param[in] hash User provided reentrant hashing function.
 *
 * @return Instance of hash table object.
 */
extern dhashtabs_t dhashtabs_new(dhashtabs_data_cmp cmp,
                                 dhashtabs_data_cmp_r cmp_r,
                                 dhashtabs_hash hash,
                                 dhashtabs_hash_r hash_r, size_t n, size_t size);

/**
 * @brief Inserts copy of data object into hash table object.
 *
 * If a data object equal to the user provided data parameter is not present in the
 * hash table object, then a data object is allocated, copied and added to the
 * hash table.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_insert</tt> on a <tt>NULL</tt> hash table object.</dd>
 * <dd>Bucket array remains unallocated. If it is, one needs to call
 * <tt>dhashtabs_rehash</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being added to.
 * @param[in] x Pointer to data being added to hash table object.
 */
extern void dhashtabs_insert(dhashtabs_t t, const void *x);

/**
 * @brief Inserts copy of data object into hash table object.
 *
 * Reentrant version of <tt>dhashtabs_insert</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_insert_r</tt> on a <tt>NULL</tt> hash table
 * object.</dd>
 * <dd>Bucket array remains unallocated. If it is, one needs to call
 * <tt>dhashtabs_rehash</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being added to.
 * @param[in] x Pointer to data being added to hash table object.
 * @param[in] hash_arg Argument to user provided reentrant hashing function.
 * @param[in] queue_arg Argument to user provided reentrant compare function.
 */
extern void dhashtabs_insert_r(dhashtabs_t t, const void *x,
                               const void *hash_arg, void *queue_arg);

/**
 * @brief Remove data object in hash table object (if present) equal in value to
 * the data object parameter provided by the user.
 *
 * The hash table object is searched for data equal in value to the user provided
 * data object. If found, the data object is removed from the hash table and a
 * pointer returned to the user. If not found, <tt>NULL</tt> is returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_remove</tt> on <tt>NULL</tt> hash table object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being altered.
 * @param[in] x Pointer to data object being searched for.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dhashtabs_remove(dhashtabs_t t, const void *x);

/**
 * @brief Remove data object in hash table object (if present) equal in value to
 * the data object parameter provided by the user.
 *
 * Reentrant version of <tt>dhashtabs_remove</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_remove_r</tt> on <tt>NULL</tt> hash table object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being altered.
 * @param[in] x Pointer to data object being searched for.
 * @param[in] queue_arg Argument to user provided reentrant compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dhashtabs_remove_r(dhashtabs_t t, const void *x, void *queue_arg);

/**
 * @brief Check if data equal to user provided data object is contained in hash
 * table object.
 *
 * Check if data equal to user provided data object is contained in hash table
 * object. Return pointer to data if found; otherwise, <tt>NULL</tt> is returned.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_find</tt> on <tt>NULL</tt> hash table object.</dd>
 * <dd>Neither the <tt>dhash_data_cmp</tt> field nor the
 * <tt>dhash_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being searched.
 * @param[in] x Data whose membership is being checked.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dhashtabs_find(dhashtabs_t t, const void *x);

/**
 * @brief Check if data equal to user provided data object is contained in hash
 * table object.
 *
 * Reentrant version of <tt>dhashtabs_find</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_find_r</tt> on <tt>NULL</tt> hash table object.</dd>
 * <dd>Neither the <tt>dhash_data_cmp</tt> field nor the
 * <tt>dhash_data_cmp_r</tt> field is not <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being searched.
 * @param[in] x Data whose membership is being checked.
 * @param[in] y Argument to user provided reentrant compare function.
 *
 * @return Pointer to data object if found. <tt>NULL</tt> otherwise.
 */
extern void *dhashtabs_find_r(dhashtabs_t t, const void *x, void *y);

/**
 * @brief Apply function to every member of hash table object.
 *
 * The function <tt>apply</tt> is applied to every member of the hash table. Early
 * termination is possible if <tt>apply</tt> returns a negative <tt>int</tt>. In
 * all other cases, we continue to apply <tt>apply</tt> to the succeeding data
 * object (if any) of the hash table.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_map</tt> on a <tt>NULL</tt> hash table object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being acted upon.
 * @param[in] apply Function being applied to members of the hash table.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dhashtabs_map(dhashtabs_t t, int apply(void **x));

/**
 * @brief Apply function to every member of hash table object.
 *
 * Reentrant version of <tt>dhashtabs_map</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_map_r</tt> on a <tt>NULL</tt> hash table object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being acted upon.
 * @param[in] apply Function being applied to members of the hash table.
 * @param[in] queue_arg Argument to user provided reentrant compare function.
 *
 * @return -1 if early termination. 1 if no early termination.
 */
extern int dhashtabs_map_r(dhashtabs_t t,
                           int apply(void **x, void *queue_arg), void *queue_arg);

/**
 * @brief Free data allocated for the deep hash-table-type associations.
 *
 * The deep copied data and associations are freed.
 *
 * @param[in] *t Pointer to <tt>dhashtabs_t</tt> object.
 */
extern void dhashtabs_free(dhashtabs_t *t);

/**
 * @brief Number of spaces allocated for hash table buckets.
 *
 * Number of spaces allocated for hash table buckets.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dt>Calling <tt>dhashtabs_capacity</tt> on a <tt>NULL</tt> hash table
 * object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being checked.
 *
 * @return Number of space allocated for hash table buckets.
 */
extern size_t dhashtabs_capacity(dhashtabs_t t);

/**
 * @brief Rehash array to the next prime length.
 *
 * Resize the buckets array to the next prime length which is at least 1.5 times
 * larger than the current prime length. If the next such prime is too large, then
 * the array is resized simply to <tt>SIZE_MAX</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>The <tt>dhash_data_cmp_r</tt> field is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table being rehashed.
 *
 * @return A new hash table of larger prime length containing the same
 * pointers as <tt>t</tt>.
 */
extern dhashtabs_t dhashtabs_rehash(dhashtabs_t t);

/**
 * @brief Rehash array to the next prime length.
 *
 * Reentrant version of <tt>dhashtabs_rehash</tt>.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>The <tt>dhash_data_cmp_r</tt> field is <tt>NULL</tt>.</dd>
 * </dl>
 *
 * @param[in] t Hash table being rehashed.
 * @param[in] hash_arg Argument to user defined reentrant hash function.
 * @param[in] queue_arg Argument to user defined reentrant compare function.
 *
 * @return A new hash table of larger prime length containing the same
 * pointers as <tt>t</tt>.
 */
extern dhashtabs_t dhashtabs_rehash_r(dhashtabs_t t,
                                      void *hash_arg, void *queue_arg);

/**
 * @brief Number of elements in hash table.
 *
 * Number of elements in hash table.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_size</tt> on a <tt>NULL</tt> hash table object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being checked.
 *
 * @return Number of members of the hash table.
 */
extern size_t dhashtabs_size(dhashtabs_t t);

/**
 * @brief Load factor of hash table.
 *
 * Return ratio of number of elements with number of buckets. This is a measure of
 * the clashes and hence the efficiency of the current data structure. If the load
 * factor is high, then one may need to pass to the structure a better hash
 * function, or rehash the table.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Calling <tt>dhashtabs_loadfactor</tt> on a <tt>NULL</tt> hash table
 * object.</dd>
 * </dl>
 *
 * @param[in] t Hash table object being checked.
 *
 * @return Load factor of hash table object.
 */
extern size_t dhashtabs_loadfactor(dhashtabs_t t);

/**
 * @brief Swap opaque pointers for deep hash tables.
 *
 * Swap opaque pointers for deep hash tables.
 *
 * <dl>
 * <dt><strong>Unchecked Runtime Errors</strong></dt>
 * <dd>Hash table objects are aliases.</dd>
 * </dl>
 *
 * @param[in] t1 First hash table.
 * @param[in] t2 Second hash table.
 */
static inline
void dhashtabs_swap(dhashtabs_t *restrict t1, dhashtabs_t *restrict t2)
{
  volatile dhashtabs_t tmp = *t1;
  *t1 = *t2;
  *t2 = tmp;
}

# endif
