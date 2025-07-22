/** @file deepstacks.c
 * @brief Implementation for <tt>dstacks_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <deepstacks.h>

/**
 * @brief Internal structure for <tt>dstacks_t</tt> object.
 */
struct dstacks_node_t {
  char *x;                     ///< dynamically allocated data
  struct dstacks_node_t *next; ///< pointer to next element in stack
};
typedef struct dstacks_node_t dstacks_node_t;

/**
 * @brief <tt>dstacks_t</tt> class object.
 */
struct dstacks_t {
  size_t nmems;         ///< number elements in stack
  size_t size;          ///< total size of data objects
  dstacks_node_t *head; ///< pointer to head of stack
};

dstacks_t dstacks_new(size_t size)
{
  dstacks_t s;
  s = (dstacks_t)malloc(sizeof(*s));
  s->nmems = 0;
  s->size = size;
  s->head = NULL;
  return s;
}

void dstacks_push(dstacks_t s, void *x)
{
  dstacks_node_t *node = (dstacks_node_t*)malloc(sizeof(dstacks_node_t));
  node->x = (char*)malloc(s->size);
  memcpy(node->x, x, s->size);
  node->next = s->head;
  s->head = node;
  s->nmems++;
}

char *dstacks_pop(dstacks_t s)
{
  dstacks_node_t *tmp = s->head;
  s->head = s->head->next;
  char *x = tmp->x;
  free(tmp);
  s->nmems--;
  return x;
}

void dstacks_free(dstacks_t *s)
{
  if ( *s == NULL ) return;
  while ( (*s)->head != NULL ) {
    dstacks_node_t *tmp = (*s)->head;
    (*s)->head = (*s)->head->next;
    free(tmp->x);
    free(tmp);
  }
  free(*s);
  *s = NULL;
}

int dstacks_empty(dstacks_t s)
{
  return s->nmems == 0;
}

size_t dstacks_size(dstacks_t s)
{
  return s->nmems;
}

int dstacks_map(dstacks_t s, int apply(void **x))
{
  if ( s == NULL ) return 1;
  dstacks_node_t *tmp = s->head;
  while ( tmp != NULL ) {
    if ( apply((void**)&tmp->x) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

int dstacks_map_r(dstacks_t s, int apply(void **x, void *y), void *y)
{
  if ( s == NULL ) return 1;
  dstacks_node_t *tmp = s->head;
  while ( tmp != NULL ) {
    if ( apply((void**)&tmp->x, y) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}
