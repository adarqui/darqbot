/*
     Copyright (C) 2012 Andrew Darqui

     This file is part of darqbot.

     darqbot is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     darqbot is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with darqbot.  If not, see <http://www.gnu.org/licenses/>.

     Contact: [website: http://www.adarq.org]
      email: andrew.darqui@g m a i l . c o m
*/
/*
                                                              + +  +
                                                           +  - -  -  +
                                                        +  -  _ _ _  -  +
                                                         -  _ . . .  _  -  +
                                                         _  . # # #  . .  -
                                                   +     .# # - - -  # #  _  +
                                                    -_    - - + + + - -  .  _-
                                                 +   .     + + .. . + + -# .   +
                                                  -_  #    . .      ..  +  #  _
                                                +   .  +               . +- #.   +
                                                 _  #-  .                . -   _-
                                                  .#  +                   .+  #
                                              +-    -+ .                    +-  _-+
                                                _.#   .                    .   #                                                                                             +     -+                        +-    +
                                              -_.#   .                      .   ._-
                                                  -+.                       .+-#
                                             +-_                                 ._-+
                                                .#-+.          2012          .+-#
                                                             adarq.org
                                              .overkill double linked list routines.
                                                   +.                        .
                                             +-_.#-                           +-#._-+
                                                    .                       .+
                                                .#-+                          -#.
                                             +-_    +.                      .+   _-+
                                                  #-                       .  -#                                                                                               -_.   +.                     +  ._-
                                              +    #-  .                  .  -#   +
                                                 _.  -+ .                . +  ._
                                                +-  #  +               . +  #   -+
                                                   _.  -   .         .  + -  ._
                                                  -  .#    + . .. . . + -  #  -
                                                 +   _    -  ++ + + + -  #  _  +
                                                    -     # - - - - -  # .   -
                                                   +     .  # # # #  # .  _  +
                                                         _  . . . .  . _   +
                                                         -  _ _ _ _  _  -
                                                        +  -  - -  - -  +
                                                           +  + +  +  +
*/

#ifndef DLIST_H
#define DLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "avl.h"

enum DLIST_TYPE
{
  DLIST_TYPE_NORMAL,
  DLIST_TYPE_CIRCULAR,
  DLIST_TYPE_AVL,
  DLIST_TYPE_ARRAY,
  DLIST_TYPE_HASH,
};

#define DLIST_TYPE_TREE DLIST_TYPE_AVL
#define DLIST_TYPE_LIST DLIST_TYPE_NORMAL

enum DLIST_SUBTYPE {
DLIST_SUBTYPE_NORMAL,
DLIST_SUBTYPE_MIRROR,
};

typedef struct avl_table avl_table_t;
typedef struct avl_node avl_node_t;

typedef struct dlist
{

  void *data;
  void (*destroy) (void *);

  struct dlist *next;
  struct dlist *prev;
  struct dlist *head;
  struct dlist *tail;

  unsigned char type;
unsigned char subtype;
  int sz;

/* dlist now has an avl tree available */
avl_table_t * avl_tree;
avl_node_t * avl_node;
int (*avl_compare)(const void *, const void *, void *);
void (*avl_free)(void *, void *);
void * avl_param;
int avl_flags;

} dlist_t;


/* dlist decls */
dlist_t *dlist_init (void *, int);
dlist_t *dlist_alloc (void *);
dlist_t *dlist_insert_after (dlist_t **, dlist_t *, dlist_t *);
dlist_t *dlist_insert_before (dlist_t **, dlist_t *, dlist_t *);
dlist_t *dlist_Dinsert_after (dlist_t **, void *);
dlist_t *dlist_Dinsert_before (dlist_t **, void *);

void dlist_remove_and_free(dlist_t **, dlist_t **, void (*destroy)(void *)) ;
void dlist_free (dlist_t **, void (*destroy) (void *));

void dlist_remove_and_free_wrapper (dlist_t **, dlist_t **, void (*destroy) (void *));
dlist_t *dlist_remove_wrapper(dlist_t **, dlist_t *);
dlist_t *dlist_remove (dlist_t **, dlist_t *);

#define dlist_delete dlist_remove
#define dlist_delete_and_free dlist_remove_and_free
 
dlist_t *dlist_Denqueue (dlist_t **, void *);
dlist_t *dlist_enqueue (dlist_t **, dlist_t *);

dlist_t *dlist_dequeue (dlist_t **);

dlist_t *dlist_Dpush (dlist_t **, void *);
dlist_t *dlist_push (dlist_t **, dlist_t *);

dlist_t *dlist_pop (dlist_t **);
void dlist_pop_and_free(dlist_t **, void (*fn)(void *));

int dlist_make_circular (dlist_t **);
int dlist_make_uncircular (dlist_t **);

dlist_t * dlist_node_rand(dlist_t *);

int dlist_fini (dlist_t **, void (*destroy) (void *));

int dlist_next_retarded(dlist_t **, dlist_t * ) ;
int dlist_prev_retarded(dlist_t **, dlist_t *);

void ** dlist_convert_dlist_to_array(dlist_t *);
void dlist_array_fini(void **, void (*fn)(void *));

void ** dlist_convert_dlist_to_array_new(dlist_t *);
void dlist_array_fini_new(void **, void (*fn)(void *));

char * dlist_to_str(dlist_t *);
char * dlist_to_str_fix (dlist_t * , char * , char * );

char * array_to_str(void ** , int, char * (*fn)(void *));
char * arraystr_to_str(char **, int, char *);

#define dlist_data(a) ((a)!=NULL ? (a)->data : NULL)
#define dlist_destroy(a) ((a)->destroy)
#define dlist_fornext(a, b) for ((b) = (a); (b)!=NULL; (b)=(b)->next)
#define dlist_forprev(a, b) for((b) = (a); (b)!=NULL; (b)=(b)->prev)
/* FIX ME */
//#define dlist_fornext_safe(a, b, c) for((b)=(a); (b)!= NULL && ((b)==NULL) ? : ((c)=dlist_next_safe((b))) ; (b)=(c))
#define dlist_fornext_retarded(a, b, c) for((b)=(a); dlist_next_retarded((&c), (b)) ; (b)=(c))
#define dlist_forprev_retarded(a,b,c)   for((b)=(a); dlist_prev_retarded((&c), (b)) ; (b)=(c))
//#define dlist_forprev_safe(a, b, c) for((b)=(a); (b)!= NULL && ((b)==NULL) ? : ((c)=dlist_next_prev((b))) ; (b)=(c))
#define dlist_nullht(a) (a)->head = NULL; (a)->tail = NULL;
#define dlist_nullnp(a) (a)->next = NULL; (a)->prev = NULL;
#define dlist_nullhtnp(a) dlist_nullht((a)); dlist_nullnp((a));
#define dlist_size(a) ((a)!=NULL ? (a)->sz : 0)

#define dlist_next(a) ((a)!=NULL ? (a)->next : NULL)
#define dlist_next_safe(a) (a) ? (a)->next : NULL

#define dlist_prev(a) ((a)!= NULL ? (a)->prev : NULL)
#define dlist_prev_safe(a) (a) ? (a)->prev : NULL

/* only to be used on head */
#define dlist_tail(a) ((a)!=NULL ? (a)->tail : NULL)

/* only to be used on tail */
#define dlist_head(a) ((a)!=NULL ? (a)->head : NULL)

#define dlist_iscirc(a) ((a)->type == DLIST_TYPE_CIRCULAR)
#define dlist_type(a) ((a)->type)




/* AVL STUFF */
dlist_t * dlist_tree_init(void * , int (*dl_avl_compare)(const void *, const void *, void *), void (*dl_avl_free)(void *,void *));
void * dlist_tree_insert(dlist_t ** , void *) ;
void * dlist_tree_delete(dlist_t ** , void *);
void * dlist_tree_delete_and_free(dlist_t ** , void * ) ;
void * dlist_tree_insert2(dlist_t ** , int (*avl_compare)(const void *, const void *, void *), void *) ;
void dlist_tree_fini(dlist_t **, void (*fn)(void *,void*)) ;
void dlist_tree_print (dlist_t ** , dlist_t **, void *(*fn)(void *) ) ;
void dlist_tree_print_structure (dlist_t **, dlist_t **, void *(*fn)(void *), const struct avl_node *, int);

dlist_t * dlist_tree_mirror(dlist_t *);
void dlist_tree_mirror_node(dlist_t **, const struct avl_node *, int ) ;

dlist_t * dlist_tree_find_dptr(dlist_t **, void *) ;
void * dlist_tree_find(dlist_t ** , void * ) ;
int dlist_tree_size(dlist_t **) ;
void dlist_tree_reorder(dlist_t **, dlist_t **) ;
void * dlist_tree_reorder_copy2mirror(void *, void *);


void * dlist_traverse(dlist_t **, void *(*fn)(void *, void *), void *);
void * dlist_traverse_list(dlist_t **, void *(*fn)(void *, void *), void *);
void * dlist_traverse_tree(dlist_t ** , void *(*fn)(void *, void *), void *) ;
void * dlist_traverse_tree_node(dlist_t ** , const struct avl_node * , void *(*fn)(void *, void *), void * ) ;

#include "bot.h"

#endif
