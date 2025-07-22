/**
 * @file deepqueues.c
 * @brief Implementation of <tt>dqueues_t</tt> class.
 * @author Thomas Pender
 */
# include <config.h>
# include <deepqueues.h>

/**
 * @brief Internal structure for <tt>dqueues_t</tt> object.
 */
struct dqueues_node_t {
  char *x;                     ///< pointer to data object
  struct dqueues_node_t *prev; ///< pointer to previous element of queue
  struct dqueues_node_t *next; ///< pointer to next element of queue
};
typedef struct dqueues_node_t dqueues_node_t;

/**
 * @brief <tt>dqueues_t</tt> class object.
 */
struct dqueues_t {
  size_t size;              ///< total size of elements of queue
  size_t nmems;             ///< number of elements of queue
  dqueues_data_cmp cmp;     ///< user provided compare function
  dqueues_data_cmp_r cmp_r; ///< user provided reentrant compare function
  dqueues_node_t *head;     ///< pointer to head of queue
  dqueues_node_t *tail;     ///< pointer to tail of queue
};

dqueues_t dqueues_new(dqueues_data_cmp cmp, dqueues_data_cmp_r cmp_r, size_t size)
{
  if ( cmp == NULL && cmp_r == NULL ) error(1, errno, "cmp pointers null");
  dqueues_t q;
  q = (dqueues_t)malloc(sizeof(*q));
  q->cmp = cmp;
  q->cmp_r = cmp_r;
  q->size = size;
  q->nmems = 0;
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void dqueues_enqueu(dqueues_t q, const void *x)
{
  /* queue is empty */
  if ( q->head == NULL ) {
    q->head = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    q->head->x = (char*)malloc(q->size);
    memcpy(q->head->x, x, q->size);
    q->head->prev = NULL;
    q->head->next = NULL;
    q->tail = q->head;
    q->nmems = 1;
    return;
  }

  /* check tail */
  switch ( q->cmp(x, q->tail->x) ) {
  case 0: return;
  case 1: {
    dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    new->x = (char*)malloc(q->size);
    memcpy(new->x, x, q->size);
    new->next = NULL;
    new->prev = q->tail;
    q->tail->next = new;
    q->tail = new;
    q->nmems++;
    return;
  }
  }

  /* check head */
  switch ( q->cmp(x, q->head->x) ) {
  case -1: {
    dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    new->x = (char*)malloc(q->size);
    memcpy(new->x, x, q->size);
    new->prev = NULL;
    new->next = q->head;
    q->head->prev = new;
    q->head = new;
    q->nmems++;
    return;
  }
  case 0: return;
  }

  /* check body */
  dqueues_node_t *tmp = q->head;
  while ( tmp->next != NULL ) {
    switch ( q->cmp(x, tmp->next->x) ) {
    case -1: {
      dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
      new->x = (char*)malloc(q->size);
      memcpy(new->x, x, q->size);
      new->prev = tmp;
      new->next = tmp->next;
      tmp->next->prev = new;
      tmp->next = new;
      q->nmems++;
      return;
    }
    case 0: return;
    case 1: tmp = tmp->next; break;
    }
  }

  return;
}

void dqueues_enqueu_r(dqueues_t q, const void *x, void *y)
{
  /* queue is empty */
  if ( q->head == NULL ) {
    q->head = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    q->head->x = (char*)malloc(q->size);
    memcpy(q->head->x, x, q->size);
    q->head->prev = NULL;
    q->head->next = NULL;
    q->tail = q->head;
    q->nmems = 1;
    return;
  }

  /* check tail */
  switch ( q->cmp_r(x, q->tail->x, y) ) {
  case 0: return;
  case 1: {
    dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    new->x = (char*)malloc(q->size);
    memcpy(new->x, x, q->size);
    new->next = NULL;
    new->prev = q->tail;
    q->tail->next = new;
    q->tail = new;
    q->nmems++;
    return;
  }
  }

  /* check head */
  switch ( q->cmp_r(x, q->head->x, y) ) {
  case -1: {
    dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
    new->x = (char*)malloc(q->size);
    memcpy(new->x, x, q->size);
    new->prev = NULL;
    new->next = q->head;
    q->head->prev = new;
    q->head = new;
    q->nmems++;
    return;
  }
  case 0: return;
  }

  /* check body */
  dqueues_node_t *tmp = q->head;
  while ( tmp->next != NULL ) {
    switch ( q->cmp_r(x, tmp->next->x, y) ) {
    case -1: {
      dqueues_node_t *new = (dqueues_node_t*)malloc(sizeof(dqueues_node_t));
      new->x = (char*)malloc(q->size);
      memcpy(new->x, x, q->size);
      new->prev = tmp;
      new->next = tmp->next;
      tmp->next->prev = new;
      tmp->next = new;
      q->nmems++;
      return;
    }
    case 0: return;
    case 1: tmp = tmp->next; break;
    }
  }

  return;
}

void *dqueues_dequeue_front(dqueues_t q)
{
  if ( q->head == NULL ) return NULL;
  if ( q->head->next == NULL ) q->tail = NULL;
  dqueues_node_t *tmp = q->head;
  char *x = tmp->x;
  q->head = q->head->next;
  free(tmp);
  if ( q->head != NULL ) q->head->prev = NULL;
  q->nmems--;
  return x;
}

void *dqueues_dequeue_back(dqueues_t q)
{
  if ( q->tail == NULL ) return NULL;
  if ( q->tail->prev == NULL ) q->head = NULL;
  dqueues_node_t *tmp = q->tail;
  char *x = tmp->x;
  q->tail = q->tail->prev;
  free(tmp);
  if ( q->tail != NULL ) q->tail->next = NULL;
  q->nmems--;
  return x;
}

void *dqueues_find(dqueues_t q, const void *x)
{
  if ( q == NULL ) return NULL;
  dqueues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    switch ( q->cmp(x, tmp->x) ) {
    case -1: return NULL;
    case 0: return tmp->x;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void *dqueues_find_r(dqueues_t q, const void *x, void *y)
{
  if ( q == NULL ) return NULL;
  dqueues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    switch ( q->cmp_r(x, tmp->x, y) ) {
    case -1: return NULL;
    case 0: return tmp->x;
    }
    tmp = tmp->next;
  }
  return NULL;
}

void *dqueues_remove(dqueues_t q, const void *x)
{
  if ( q == NULL || q->head == NULL ) return NULL;

  char *_x;
  dqueues_node_t *tmp;

  /* check head */
  tmp = q->head;
  switch ( q->cmp(x, tmp->x) ) {
  case -1: return NULL;
  case 0:
    if ( q->head->next != NULL ) q->head->next->prev = NULL;
    q->head = q->head->next;
    _x = tmp->x;
    free(tmp);
    q->nmems--;
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
    q->nmems--;
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
      q->nmems--;
      return _x;
    case 1: tmp = tmp->next; break;
    }

  return NULL;
}

void *dqueues_remove_r(dqueues_t q, const void *x, void *y)
{
  if ( q == NULL || q->head == NULL ) return NULL;

  char *_x;
  dqueues_node_t *tmp;

  /* check head */
  tmp = q->head;
  switch ( q->cmp_r(x, tmp->x, y) ) {
  case -1: return NULL;
  case 0:
    if ( q->head->next != NULL ) q->head->next->prev = NULL;
    q->head = q->head->next;
    _x = tmp->x;
    free(tmp);
    q->nmems--;
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
    q->nmems--;
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
      q->nmems--;
      return _x;
    case 1: tmp = tmp->next; break;
    }

  return NULL;
}

int dqueues_map(dqueues_t q, int apply(void **x))
{
  if ( q == NULL ) return 1;
  dqueues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    if ( apply((void**)&tmp->x) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

int dqueues_map_r(dqueues_t q, int apply(void **x, void *y), void *y)
{
  if ( q == NULL ) return 1;
  dqueues_node_t *tmp = q->head;
  while ( tmp != NULL ) {
    if ( apply((void**)&tmp->x, y) < 0 ) return -1;
    tmp = tmp->next;
  }
  return 1;
}

void dqueues_free(dqueues_t *q)
{
  if ( *q == NULL ) return;
  while ( (*q)->head != NULL ) {
    dqueues_node_t *tmp = (*q)->head;
    (*q)->head = (*q)->head->next;
    free(tmp);
  }
  free(*q);
  *q = NULL;
}

size_t dqueues_size(dqueues_t q)
{
  return q->nmems;
}
