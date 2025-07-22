/**
 * @file hashtabs.c
 * @brief Implementation of <tt>hashtabs_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <hashtabs.h>
# include <queues.h>

# define NPRIMES 45

static volatile
size_t _primes[] = {
  11, 17, 29, 43, 67, 101, 151, 227, 347, 521, 787, 1181, 1777, 2671, 4007,
  6011, 9029, 13553, 20333, 30509, 45763, 68659, 103001, 154501, 231779, 347671,
  521519, 782297, 1173463, 1760203, 2640317, 3960497, 5940761, 8911141, 13366711,
  20050081, 30075127, 45112693, 67669079, 101503627, 152255461, 228383273,
  342574909, 513862367, 770793589, SIZE_MAX,
};

/**
 * @brief Structure for rehashing hash table.
 */
typedef struct {
  hashtabs_t t;    ///< hash table being rehashed
  void *hash_arg;  ///< argument to user provided reentrant hashing function
  void *queue_arg; ///< argument to user defined reentrant compare function
} rehash_t;

/**
 * @brief <tt>hashtabs_t</tt> class object.
 */
struct hashtabs_t {
  size_t size;               ///< number of elements in hash table
  size_t load;               ///< number of nonnull buckets
  size_t cap_index;          ///< index to prime array corr. to prime length
  hashtabs_data_cmp cmp;     ///< user defined compare function
  hashtabs_data_cmp_r cmp_r; ///< user defined reentrant compare function
  hashtabs_hash hash;        ///< user defined hashing function
  hashtabs_hash_r hash_r;    ///< user defined reentrant hashing function
  queues_t *A;               ///< bucket array
};

static inline
size_t _get_cap_index(size_t n)
{
  size_t i;
  for ( i = 0; i < NPRIMES && n >= _primes[i]; i++ );
  return i;
}

uint64_t hashtabs_stdhash(const void *_a, const void *_n)
{
  size_t n = *(size_t*)_n;
  uint64_t seed = *(uint64_t*)_n;
  uint32_t *a = (uint32_t*)_a;
  for ( size_t i = 0; i < n; i++ ) {
    uint64_t x = a[i];
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

hashtabs_t hashtabs_new(hashtabs_data_cmp cmp, hashtabs_data_cmp_r cmp_r,
                        hashtabs_hash hash, hashtabs_hash_r hash_r, size_t n)
{
  hashtabs_t t;
  t = (hashtabs_t)malloc(sizeof(*t));
  t->cap_index = _get_cap_index(n);
  t->size = 0;
  t->load = 0;
  t->cmp = cmp;
  t->cmp_r = cmp_r;
  t->hash = hash;
  t->hash_r = hash_r;
  t->A = (queues_t*)calloc(_primes[t->cap_index], sizeof(queues_t));
  return t;
}

void hashtabs_insert(hashtabs_t t, const void *x)
{
  uint64_t val = t->hash(x) % _primes[t->cap_index];
  if ( t->A[val] == NULL ) {
    t->load++;
    t->A[val] = queues_new(t->cmp, t->cmp_r);
  }
  queues_enqueu(t->A[val], x);
  t->size++;
}

void hashtabs_insert_r(hashtabs_t t, const void *x,
                       const void *hash_arg, void *queues_arg)
{
  uint64_t val = t->hash_r(x, hash_arg) % _primes[t->cap_index];
  if ( t->A[val] == NULL ) {
    t->load++;
    t->A[val] = queues_new(t->cmp, t->cmp_r);
  }
  queues_enqueu_r(t->A[val], x, queues_arg);
  t->size++;
}

void *hashtabs_remove(hashtabs_t t, const void *_x)
{
  void *x;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (x = queues_remove(t->A[i], _x)) != NULL ) {
      t->size--;
      if ( queues_size(t->A[i]) == 0 ) t->load--;
      return x;
    }
  return NULL;
}

void *hashtabs_remove_r(hashtabs_t t, const void *_x, void *queue_arg)
{
  void *x;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (x = queues_remove_r(t->A[i], _x, queue_arg)) != NULL ) {
      t->size--;
      if ( queues_size(t->A[i]) == 0 ) t->load--;
      return x;
    }
  return NULL;
}

void *hashtabs_find(hashtabs_t t, const void *x)
{
  void *ptr;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (ptr = queues_find(t->A[i], x)) != NULL ) return ptr;
  return NULL;
}

void *hashtabs_find_r(hashtabs_t t, const void *x, void *queue_arg)
{
  void *ptr;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (ptr = queues_find_r(t->A[i], x, queue_arg)) != NULL ) return ptr;
  return NULL;
}

int hashtabs_map(hashtabs_t t, int apply(void **x))
{
  if ( t == NULL ) return 1;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( queues_map(t->A[i], apply) < 0 ) return -1;
  return 1;
}

int hashtabs_map_r(hashtabs_t t,
                   int apply(void **x, void *queue_arg), void *queue_arg)
{
  if ( t == NULL ) return 1;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( queues_map_r(t->A[i], apply, queue_arg) < 0 ) return -1;
  return 1;
}

void hashtabs_free(hashtabs_t *t)
{
  if ( *t == NULL ) return;
  for ( size_t i = 0; i < _primes[(*t)->cap_index]; i++ )
    queues_free((queues_t*)(&((*t)->A[i])));
  free((queues_t*)(*t)->A);
  free(*t);
  *t = NULL;
}

size_t hashtabs_capacity(hashtabs_t t)
{
  return _primes[t->cap_index];
}

static
int _rehash(void **x, void *_y)
{
  rehash_t *y = (rehash_t*)_y;
  hashtabs_insert(y->t, *x);
  return 1;
}

hashtabs_t hashtabs_rehash(hashtabs_t t)
{
  rehash_t rehash = {
    .t = hashtabs_new(t->cmp, t->cmp_r, t->hash, t->hash_r, _primes[t->cap_index]),
    .hash_arg = NULL, .queue_arg = NULL,
  };
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    (void)queues_map_r(t->A[i], _rehash, &rehash);
  return rehash.t;
}

static
int _rehash_r(void **x, void *_y)
{
  rehash_t *y = (rehash_t*)_y;
  hashtabs_insert_r(y->t, *x, y->hash_arg, y->queue_arg);
  return 1;
}

hashtabs_t hashtabs_rehash_r(hashtabs_t t, void *hash_arg, void *queue_arg)
{
  rehash_t rehash = {
    .t = hashtabs_new(t->cmp, t->cmp_r, t->hash, t->hash_r, _primes[t->cap_index]),
    .hash_arg = hash_arg, .queue_arg = queue_arg
  };
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    (void)queues_map_r(t->A[i], _rehash_r, &rehash);
  return rehash.t;
}

size_t hashtabs_size(hashtabs_t t)
{
  return t->size;
}

size_t hashtabs_loadfactor(hashtabs_t t)
{
  return t->load == 0 ? 0 : t->size / t->load;
}
