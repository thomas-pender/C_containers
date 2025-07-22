/**
 * @file queues.c
 * @brief Implementation of <tt>queues_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <queues.h>

/**
 * @brief Internal structure for <tt>queues_t</tt> object.
 */
struct queues_node_t {
  void *x;                    ///< pointer to data object
  struct queues_node_t *prev; ///< pointer to previous element of queue
  struct queues_node_t *next; ///< pointer to next element of queue
};
typedef struct queues_node_t queues_node_t;

/**
 * @brief <tt>queues_t</tt> class object.
 */
struct queues_t {
  size_t size;             ///< number of elements in queue
  queues_data_cmp cmp;     ///< user provided compare function
  queues_data_cmp_r cmp_r; ///< user provided reentrant compare function
  queues_node_t *head;     ///< pointer to head of queue
  queues_node_t *tail;     ///< pointer to tail of queue
};

queues_t queues_new(queues_data_cmp cmp, queues_data_cmp_r cmp_r)
{
  if ( cmp == NULL && cmp_r == NULL ) error(1, errno, "cmp pointers null");
  queues_t q;
  q = (queues_t)malloc(sizeof(*q));
  q->cmp = cmp;
  q->cmp_r = cmp_r;
  q->size = 0;
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void queues_enqueu(queues_t q, const void *x)
{
  /* queue is empty */
  if ( q->head == NULL ) {
    q->head = (queues_node_t*)malloc(sizeof(queues_node_t));
    q->head->x = (void*)x;
    q->head->prev = NULL;
    q->head->next = NULL;
    q->tail = q->head;
    q->size = 1;
    return;
  }

  /* check tail */
  switch ( q->cmp(x, q->tail->x) ) {
  case 0: return;
  case 1: {
    queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
    new->x = (void*)x;
    new->next = NULL;
    new->prev = q->tail;
    q->tail->next = new;
    q->tail = new;
    q->size++;
    return;
  }
  }

  /* check head */
  switch ( q->cmp(x, q->head->x) ) {
  case -1: {
    queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
    new->x = (void*)x;
    new->prev = NULL;
    new->next = q->head;
    q->head->prev = new;
    q->head = new;
    q->size++;
    return;
  }
  case 0: return;
  }

  /* check body */
  queues_node_t *tmp = q->head;
  while ( tmp->next != NULL ) {
    switch ( q->cmp(x, tmp->next->x) ) {
    case -1: {
      queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
      new->x = (void*)x;
      new->prev = tmp;
      new->next = tmp->next;
      tmp->next->prev = new;
      tmp->next = new;
      q->size++;
      return;
    }
    case 0: return;
    case 1: tmp = tmp->next; break;
    }
  }

  return;
}

void queues_enqueu_r(queues_t q, const void *x, void *y)
{
  /* queue is empty */
  if ( q->head == NULL ) {
    q->head = (queues_node_t*)malloc(sizeof(queues_node_t));
    q->head->x = (void*)x;
    q->head->prev = NULL;
    q->head->next = NULL;
    q->tail = q->head;
    q->size = 1;
    return;
  }

  /* check tail */
  switch ( q->cmp_r(x, q->tail->x, y) ) {
  case 0: return;
  case 1: {
    queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
    new->x = (void*)x;
    new->next = NULL;
    new->prev = q->tail;
    q->tail->next = new;
    q->tail = new;
    q->size++;
    return;
  }
  }

  /* check head */
  switch ( q->cmp_r(x, q->head->x, y) ) {
  case -1: {
    queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
    new->x = (void*)x;
    new->prev = NULL;
    new->next = q->head;
    q->head->prev = new;
    q->head = new;
    q->size++;
    return;
  }
  case 0: return;
  }

  /* check body */
  queues_node_t *tmp = q->head;
  while ( tmp->next != NULL ) {
    switch ( q->cmp_r(x, tmp->next->x, y) ) {
    case -1: {
      queues_node_t *new = (queues_node_t*)malloc(sizeof(queues_node_t));
      new->x = (void*)x;
      new->prev = tmp;
      new->next = tmp->next;
      tmp->next->prev = new;
      tmp->next = new;
      q->size++;
      return;
    }
    case 0: return;
    case 1: tmp = tmp->next; break;
    }
  }

  return;
}

void *queues_dequeue_front(queues_t q)
{
  if ( q->head == NULL ) return NULL;
  if ( q->head->next == NULL ) q->tail = NULL;
  queues_node_t *tmp = q->head;
  void *x = tmp->x;
  q->head = q->head->next;
  free(tmp);
  if ( q->head != NULL ) q->head->prev = NULL;
  q->size--;
  return x;
}

void *queues_dequeue_back(queues_t q)
{
  if ( q->tail == NULL ) return NULL;
  if ( q->tail->prev == NULL ) q->head = NULL;
  queues_node_t *tmp = q->tail;
  void *x = tmp->x;
  q->tail = q->tail->prev;
  free(tmp);
  if ( q->tail != NULL ) q->tail->next = NULL;
  q->size--;
  return x;
}

void *queues_find(queues_t q, const void *x)
{
  if ( q == NULL ) return NULL;
  queues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    switch ( q->cmp(x, tmp->x) ) {
    case -1: return NULL;
    case 0: return tmp->x;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void *queues_find_r(queues_t q, const void *x, void *y)
{
  if ( q == NULL ) return NULL;
  queues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    switch ( q->cmp_r(x, tmp->x, y) ) {
    case -1: return NULL;
    case 0: return tmp->x;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void *queues_remove(queues_t q, const void *x)
{
  if ( q == NULL || q->head == NULL ) return NULL;

  void *_x;
  queues_node_t *tmp;

  /* check head */
  tmp = q->head;
  switch ( q->cmp(x, tmp->x) ) {
  case -1: return NULL;
  case 0:
    if ( q->head->next != NULL ) q->head->next->prev = NULL;
    q->head = q->head->next;
    _x = tmp->x;
    free(tmp);
    q->size--;
    return _x;
  }

  /* check tail */
  tmp = q->tail;
  switch ( q->cmp(x, tmp->x) ) {
  case 0:
    if ( q->tail->prev != NULL ) q->tail->prev->next = NULL;
    q->tail = q->tail->prev;
    _x = tmp->x;
    free(tmp);
    q->size--;
    return _x;
  case 1: return NULL;
  }

  /* check body */
  tmp = q->head->next;
  while ( tmp != q->tail )
    switch ( q->cmp(x, tmp->x) ) {
    case -1: return NULL;
    case 0:
      tmp->prev->next = tmp->next;
      tmp->next->prev = tmp->prev;
      _x = tmp->x;
      free(tmp);
      q->size--;
      return _x;
    case 1: tmp = tmp->next; break;
    }

  return NULL;
}

void *queues_remove_r(queues_t q, const void *x, void *y)
{
  if ( q == NULL || q->head == NULL ) return NULL;

  void *_x;
  queues_node_t *tmp;

  /* check head */
  tmp = q->head;
  switch ( q->cmp_r(x, tmp->x, y) ) {
  case -1: return NULL;
  case 0:
    if ( q->head->next != NULL ) q->head->next->prev = NULL;
    q->head = q->head->next;
    _x = tmp->x;
    free(tmp);
    q->size--;
    return _x;
  }

  /* check tail */
  tmp = q->tail;
  switch ( q->cmp_r(x, tmp->x, y) ) {
  case 0:
    if ( q->tail->prev != NULL ) q->tail->prev->next = NULL;
    q->tail = q->tail->prev;
    _x = tmp->x;
    free(tmp);
    q->size--;
    return _x;
  case 1: return NULL;
  }

  /* check body */
  tmp = q->head->next;
  while ( tmp != q->tail )
    switch ( q->cmp_r(x, tmp->x, y) ) {
    case -1: return NULL;
    case 0:
      tmp->prev->next = tmp->next;
      tmp->next->prev = tmp->prev;
      _x = tmp->x;
      free(tmp);
      q->size--;
      return _x;
    case 1: tmp = tmp->next; break;
    }

  return NULL;
}

int queues_map(queues_t q, int apply(void **x))
{
  if ( q == NULL ) return 1;
  queues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    if ( apply(&tmp->x) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

int queues_map_r(queues_t q, int apply(void **x, void *y), void *y)
{
  if ( q == NULL ) return 1;
  queues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    if ( apply(&tmp->x, y) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

void queues_free(queues_t *q)
{
  if ( *q == NULL ) return;
  while ( (*q)->head != NULL ) {
    queues_node_t *tmp = (*q)->head;
    (*q)->head = (*q)->head->next;
    free(tmp);
  }
  free(*q);
  *q = NULL;
}

size_t queues_size(queues_t q)
{
  return q->size;
}
