
//
// list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "clist.h"

/*
 * Allocate a new clist_t. NULL on failure.
 */

clist_t *
clist_new(void) {
  clist_t *self;
  if (!(self = CLIST_MALLOC(sizeof(clist_t))))
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->free = NULL;
  self->match = NULL;
  self->len = 0;
  return self;
}

/*
 * Free the list.
 * @self: Pointer to the list 
 */

void
clist_destroy(clist_t *self) {
  unsigned int len = self->len;
  clist_node_t *next;
  clist_node_t *curr = self->head;

  while (len--) {
    next = curr->next;
    if (self->free) self->free(curr->val);
    CLIST_FREE(curr);
    curr = next;
  }

  CLIST_FREE(self);
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for popping node
 * @node: the node to push
 */

clist_node_t *
clist_rpush(clist_t *self, clist_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return / detach the last node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

clist_node_t *
clist_rpop(clist_t *self) {
  if (!self->len) return NULL;

  clist_node_t *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->tail = self->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return / detach the first node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

clist_node_t *
clist_lpop(clist_t *self) {
  if (!self->len) return NULL;

  clist_node_t *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for pushing node
 * @node: the node to push
 */

clist_node_t *
clist_lpush(clist_t *self, clist_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return the node associated to val or NULL.
 * @self: Pointer to the list for finding given value
 * @val: Value to find 
 */

clist_node_t *
clist_find(clist_t *self, void *val) {
  clist_iterator_t *it = clist_iterator_new(self, CLIST_HEAD);
  clist_node_t *node;

  while ((node = clist_iterator_next(it))) {
    if (self->match) {
      if (self->match(val, node->val)) {
        clist_iterator_destroy(it);
        return node;
      }
    } else {
      if (val == node->val) {
        clist_iterator_destroy(it);
        return node;
      }
    }
  }

  clist_iterator_destroy(it);
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 * @self: Pointer to the list for finding given index 
 * @index: the index of node in the list
 */

clist_node_t *
clist_at(clist_t *self, int index) {
  clist_direction_t direction = CLIST_HEAD;

  if (index < 0) {
    direction = CLIST_TAIL;
    index = ~index;
  }

  if ((unsigned)index < self->len) {
    clist_iterator_t *it = clist_iterator_new(self, direction);
    clist_node_t *node = clist_iterator_next(it);
    while (index--) node = clist_iterator_next(it);
    clist_iterator_destroy(it);
    return node;
  }

  return NULL;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 * @self: Pointer to the list to delete a node 
 * @node: Pointer the node to be deleted
 */

void
clist_remove(clist_t *self, clist_node_t *node) {
  node->prev
    ? (node->prev->next = node->next)
    : (self->head = node->next);

  node->next
    ? (node->next->prev = node->prev)
    : (self->tail = node->prev);

  if (self->free) self->free(node->val);

  CLIST_FREE(node);
  --self->len;
}
