
//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "clist.h"

/*
 * Allocate a new clist_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */

clist_iterator_t *
clist_iterator_new(clist_t *list, clist_direction_t direction) {
  clist_node_t *node = direction == CLIST_HEAD
    ? list->head
    : list->tail;
  return clist_iterator_new_from_node(node, direction);
}

/*
 * Allocate a new clist_iterator_t with the given start
 * node. NULL on failure.
 */

clist_iterator_t *
clist_iterator_new_from_node(clist_node_t *node, clist_direction_t direction) {
  clist_iterator_t *self;
  if (!(self = CLIST_MALLOC(sizeof(clist_iterator_t))))
    return NULL;
  self->next = node;
  self->direction = direction;
  return self;
}

/*
 * Return the next clist_node_t or NULL when no more
 * nodes remain in the list.
 */

clist_node_t *
clist_iterator_next(clist_iterator_t *self) {
  clist_node_t *curr = self->next;
  if (curr) {
    self->next = self->direction == CLIST_HEAD
      ? curr->next
      : curr->prev;
  }
  return curr;
}

/*
 * Free the list iterator.
 */

void
clist_iterator_destroy(clist_iterator_t *self) {
  CLIST_FREE(self);
  self = NULL;
}
