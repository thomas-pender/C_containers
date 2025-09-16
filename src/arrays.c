/**
 * @file arrays.c
 * @brief Implementation of <tt>arrays_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <arrays.h>

# include <stdio.h>

/**
 * @brief <tt>arrays_t</tt> class object.
 */
struct arrays_t {
  size_t size;     ///< size of elements of array
  size_t capacity; ///< number of possible elements of the array
  size_t nmem;     ///< number of elements currently in array
  char *x;         ///< data array
};

arrays_t arrays_new(size_t size, size_t capacity)
{
  arrays_t a;
  a = (arrays_t)malloc(sizeof(*a));
  a->size = size;
  a->nmem = 0;
  a->capacity = capacity;
  if ( capacity == 0 ) a->x = NULL;
  else {
    if ( (a->x = (char*)malloc(a->size * a->capacity)) == NULL )
      error(1, errno, "malloc failure");
  }
  return a;
}

int arrays_push(arrays_t a, const void *x)
{
  if ( a->nmem == a->capacity ) return -1;
  memcpy(a->x + (a->nmem * a->size), x, a->size);
  a->nmem++;
  return 1;
}

void arrays_dynpush(arrays_t a, const void *x)
{
  if ( a->nmem == a->capacity ) {
    size_t nmem = ((a->nmem==0) ? 1 : ((a->nmem==1) ? 2 : ((3*a->nmem)>>1UL)));
    arrays_resize(a, nmem);
  }
  memcpy(a->x + (a->nmem * a->size), x, a->size);
  a->nmem++;
}

int arrays_map(arrays_t a, int apply(void *x))
{
  for ( size_t i = 0; i < a->nmem; i++ )
    if ( apply(a->x + (a->size * i)) < 0 ) return -1;
  return 1;
}

int arrays_map_r(arrays_t a, int apply(void *x, void *y), void *y)
{
  for ( size_t i = 0; i < a->nmem; i++ )
    if ( apply(a->x + (a->size * i), y) < 0 ) return -1;
  return 1;
}

void *arrays_at(arrays_t a, size_t i)
{
  return a->x + (a->size * i);
}

void *arrays_bsearch(arrays_t a, const void *x,
                     int cmp(const void *x, const void *y))
{
  return bsearch(x, a->x, a->nmem, a->size, cmp);
}

void arrays_sort(arrays_t a, int cmp(const void *x, const void *y))
{
  qsort(a->x, a->nmem, a->size, cmp);
}

void arrays_sort_r(arrays_t a, int cmp(const void *x, const void *y, void *z),
                   void *z)
{
  qsort_r(a->x, a->nmem, a->size, cmp, z);
}

void arrays_resize(arrays_t a, size_t nmem)
{
  a->capacity = nmem;
  char *tmp;
  if ( (tmp = (char*)realloc(a->x, a->size * a->capacity)) == NULL )
    error(1, errno, "realloc failure");
  a->x = tmp;
}

void arrays_free(arrays_t *a)
{
  if ( a == NULL || *a == NULL ) return;
  if ( (*a)->x != NULL ) free((*a)->x);
  free(*a);
  *a = NULL;
}

size_t arrays_nmem(arrays_t a)
{
  return a->nmem;
}

size_t arrays_capacity(arrays_t a)
{
  return a->capacity;
}

size_t arrays_size(arrays_t a)
{
  return a->size;
}

int arrays_equal(arrays_t a1, arrays_t a2)
{
  if ( a1->nmem != a2->nmem || a1->size != a2->size ) return -1;

  size_t i, len = a1->nmem * a1->size;
  for ( i = 0; i < len; i++ ) if ( a1->x[i] != a2->x[i] ) return -1;

  return 1;
}

void arrays_reindex(arrays_t a, size_t nmem)
{
  if ( a == NULL || nmem >= a->nmem ) return;
  a->nmem = nmem;
}
