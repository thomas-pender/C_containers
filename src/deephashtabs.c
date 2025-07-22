/**
 * @file deephashtabs.c
 * @brief Implementation of <tt>dhashtabs_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <deephashtabs.h>
# include <deepqueues.h>

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
  dhashtabs_t t;   ///< hash table being rehashed
  void *hash_arg;  ///< argument to user provided reentrant hashing function
  void *queue_arg; ///< argument to user defined reentrant compare function
} rehash_t;

/**
 * @brief <tt>hashtabs_t</tt> class object.
 */
struct dhashtabs_t {
  size_t nmems;               ///< number of elements in hash table
  size_t size;                ///< total size of data objects
  size_t load;                ///< number of nonnull buckets
  size_t cap_index;           ///< index to prime array corr. to prime length
  dhashtabs_data_cmp cmp;     ///< user defined compare function
  dhashtabs_data_cmp_r cmp_r; ///< user defined reentrant compare function
  dhashtabs_hash hash;        ///< user defined hashing function
  dhashtabs_hash_r hash_r;    ///< user defined reentrant hashing function
  dqueues_t *A;               ///< bucket array
};

static inline
size_t _get_cap_index(size_t n)
{
  size_t i;
  for ( i = 0; i < NPRIMES && n >= _primes[i]; i++ );
  return i;
}

uint64_t dhashtabs_stdhash(const void *_a, const void *_n)
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

dhashtabs_t dhashtabs_new(dhashtabs_data_cmp cmp, dhashtabs_data_cmp_r cmp_r,
                        dhashtabs_hash hash, dhashtabs_hash_r hash_r,
                        size_t n, size_t size)
{
  dhashtabs_t t;
  t = (dhashtabs_t)malloc(sizeof(*t));
  t->cap_index = _get_cap_index(n);
  t->size = size;
  t->nmems = 0;
  t->load = 0;
  t->cmp = cmp;
  t->cmp_r = cmp_r;
  t->hash = hash;
  t->hash_r = hash_r;
  t->A = (dqueues_t*)calloc(_primes[t->cap_index], sizeof(dqueues_t));
  return t;
}

void dhashtabs_insert(dhashtabs_t t, const void *x)
{
  uint64_t val = t->hash(x) % _primes[t->cap_index];
  if ( t->A[val] == NULL ) {
    t->load++;
    t->A[val] = dqueues_new(t->cmp, t->cmp_r, t->size);
  }
  dqueues_enqueu(t->A[val], x);
  t->size++;
}

void dhashtabs_insert_r(dhashtabs_t t, const void *x,
                       const void *hash_arg, void *dqueues_arg)
{
  uint64_t val = t->hash_r(x, hash_arg) % _primes[t->cap_index];
  if ( t->A[val] == NULL ) {
    t->load++;
    t->A[val] = dqueues_new(t->cmp, t->cmp_r, t->size);
  }
  dqueues_enqueu_r(t->A[val], x, dqueues_arg);
  t->size++;
}

void *dhashtabs_remove(dhashtabs_t t, const void *_x)
{
  void *x;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (x = dqueues_remove(t->A[i], _x)) != NULL ) {
      t->size--;
      if ( dqueues_size(t->A[i]) == 0 ) t->load--;
      return x;
    }
  return NULL;
}

void *dhashtabs_remove_r(dhashtabs_t t, const void *_x, void *queue_arg)
{
  void *x;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (x = dqueues_remove_r(t->A[i], _x, queue_arg)) != NULL ) {
      t->size--;
      if ( dqueues_size(t->A[i]) == 0 ) t->load--;
      return x;
    }
  return NULL;
}

void *dhashtabs_find(dhashtabs_t t, const void *x)
{
  void *ptr;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (ptr = dqueues_find(t->A[i], x)) != NULL ) return ptr;
  return NULL;
}

void *dhashtabs_find_r(dhashtabs_t t, const void *x, void *queue_arg)
{
  void *ptr;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( (ptr = dqueues_find_r(t->A[i], x, queue_arg)) != NULL ) return ptr;
  return NULL;
}

int dhashtabs_map(dhashtabs_t t, int apply(void **x))
{
  if ( t == NULL ) return 1;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( dqueues_map(t->A[i], apply) < 0 ) return -1;
  return 1;
}

int dhashtabs_map_r(dhashtabs_t t,
                   int apply(void **x, void *queue_arg), void *queue_arg)
{
  if ( t == NULL ) return 1;
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    if ( dqueues_map_r(t->A[i], apply, queue_arg) < 0 ) return -1;
  return 1;
}

void dhashtabs_free(dhashtabs_t *t)
{
  if ( *t == NULL ) return;
  for ( size_t i = 0; i < _primes[(*t)->cap_index]; i++ )
    dqueues_free((dqueues_t*)(&((*t)->A[i])));
  free((dqueues_t*)(*t)->A);
  free(*t);
  *t = NULL;
}

size_t dhashtabs_capacity(dhashtabs_t t)
{
  return _primes[t->cap_index];
}

static
int _rehash(void **x, void *_y)
{
  rehash_t *y = (rehash_t*)_y;
  dhashtabs_insert(y->t, *x);
  return 1;
}

dhashtabs_t dhashtabs_rehash(dhashtabs_t t)
{
  rehash_t rehash = {
    .t = dhashtabs_new(t->cmp, t->cmp_r, t->hash, t->hash_r,
                       _primes[t->cap_index], t->size),
    .hash_arg = NULL, .queue_arg = NULL,
  };
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    (void)dqueues_map_r(t->A[i], _rehash, &rehash);
  return rehash.t;
}

static
int _rehash_r(void **x, void *_y)
{
  rehash_t *y = (rehash_t*)_y;
  dhashtabs_insert_r(y->t, *x, y->hash_arg, y->queue_arg);
  return 1;
}

dhashtabs_t dhashtabs_rehash_r(dhashtabs_t t, void *hash_arg, void *queue_arg)
{
  rehash_t rehash = {
    .t = dhashtabs_new(t->cmp, t->cmp_r, t->hash, t->hash_r,
                       _primes[t->cap_index], t->size),
    .hash_arg = hash_arg, .queue_arg = queue_arg
  };
  for ( size_t i = 0; i < _primes[t->cap_index]; i++ )
    (void)dqueues_map_r(t->A[i], _rehash_r, &rehash);
  return rehash.t;
}

size_t dhashtabs_size(dhashtabs_t t)
{
  return t->size;
}

size_t dhashtabs_loadfactor(dhashtabs_t t)
{
  return t->load == 0 ? 0 : t->size / t->load;
}
