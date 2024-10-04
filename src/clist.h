
//
// list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef __CLIBS_LIST_H__
#define __CLIBS_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// Library version

#define CLIST_VERSION "0.4.1"

// Memory management macros
#ifdef CLIST_CONFIG_H
#define _STR(x) #x
#define STR(x) _STR(x)
#include STR(LIST_CONFIG_H)
#undef _STR
#undef STR
#endif

#ifndef CLIST_MALLOC
#define CLIST_MALLOC malloc
#endif

#ifndef CLIST_FREE
#define CLIST_FREE free
#endif

/*
 * list_t iterator direction.
 */

typedef enum {
    CLIST_HEAD
  , CLIST_TAIL
} clist_direction_t;

/*
 * list_t node struct.
 */

typedef struct clist_node {
  struct clist_node *prev;
  struct clist_node *next;
  void *val;
} clist_node_t;

/*
 * list_t struct.
 */

typedef struct {
  clist_node_t *head;
  clist_node_t *tail;
  unsigned int len;
  void (*free)(void *val);
  int (*match)(void *a, void *b);
} clist_t;

/*
 * list_t iterator struct.
 */

typedef struct {
  clist_node_t *next;
  clist_direction_t direction;
} clist_iterator_t;

// Node prototypes.

clist_node_t *
clist_node_new(void *val);

// clist_t prototypes.

clist_t *
clist_new(void);

clist_node_t *
clist_rpush(clist_t *self, clist_node_t *node);

clist_node_t *
clist_lpush(clist_t *self, clist_node_t *node);

clist_node_t *
clist_find(clist_t *self, void *val);

clist_node_t *
clist_at(clist_t *self, int index);

clist_node_t *
clist_rpop(clist_t *self);

clist_node_t *
clist_lpop(clist_t *self);

void
clist_remove(clist_t *self, clist_node_t *node);

void
clist_destroy(clist_t *self);

// clist_t iterator prototypes.

clist_iterator_t *
clist_iterator_new(clist_t *clist, clist_direction_t direction);

clist_iterator_t *
clist_iterator_new_from_node(clist_node_t *node, clist_direction_t direction);

clist_node_t *
clist_iterator_next(clist_iterator_t *self);

void
clist_iterator_destroy(clist_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __CLIBS_LIST_H__ */
