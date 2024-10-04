
//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "clist.h"

/*
 * Allocates a new clist_node_t. NULL on failure.
 */

clist_node_t *
clist_node_new(void *val) {
  clist_node_t *self;
  if (!(self = CLIST_MALLOC(sizeof(clist_node_t))))
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}