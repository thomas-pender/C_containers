/** @file stacks.c
 * @brief Implementation for <tt>stacks_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <stacks.h>

/**
 * @brief Internal structure for <tt>stacks_t</tt> object.
 */
struct stacks_node_t {
  void *x;                    ///< pointer to data object
  struct stacks_node_t *next; ///< pointer to next element in stack
};
typedef struct stacks_node_t stacks_node_t;

/**
 * @brief <tt>stacks_t</tt> class object.
 */
struct stacks_t {
  size_t size;         ///< number of elements in stack
  stacks_node_t *head; ///< pointer to top of stack
};

stacks_t stacks_new(void)
{
  stacks_t s;
  s = (stacks_t)malloc(sizeof(*s));
  s->size = 0;
  s->head = NULL;
  return s;
}

void stacks_push(stacks_t s, void *x)
{
  stacks_node_t *tmp = (stacks_node_t*)malloc(sizeof(stacks_node_t));
  tmp->x = (void*)x;
  tmp->next = s->head;
  s->head = tmp;
  s->size++;
}

void *stacks_pop(stacks_t s)
{
  stacks_node_t *tmp = s->head;
  s->head = s->head->next;
  void *x = tmp->x;
  free(tmp);
  s->size--;
  return x;
}

void stacks_free(stacks_t *s)
{
  if ( *s == NULL ) return;
  while ( (*s)->head != NULL ) {
    stacks_node_t *tmp = (*s)->head;
    (*s)->head = (*s)->head->next;
    free(tmp);
  }
  free(*s);
  *s = NULL;
}

int stacks_empty(stacks_t s)
{
  return s->size == 0;
}

size_t stacks_size(stacks_t s)
{
  return s->size;
}

int stacks_map(stacks_t s, int apply(void **x))
{
  if ( s == NULL ) return 1;
  stacks_node_t *tmp = s->head;
  while ( tmp != NULL ) {
    if ( apply(&tmp->x) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

int stacks_map_r(stacks_t s, int apply(void **x, void *y), void *y)
{
  if ( s == NULL ) return 1;
  stacks_node_t *tmp = s->head;
  while ( tmp != NULL ) {
    if ( apply(&tmp->x, y) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}
