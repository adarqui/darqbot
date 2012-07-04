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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef MOD_BSDAVLTREE_H
#define MOD_BSDAVLTREE_H

#include "bot.h"

#include "bsdavltree.h"

module_t mod_bsdavltree_info;

bot_t *bsdavltree_init (dlist_t *, bot_t *);
bot_t *bsdavltree_fini (dlist_t *, bot_t *);
bot_t *bsdavltree_help (dlist_t *, bot_t *);
bot_t *bsdavltree_run (dlist_t *, bot_t *);


char *bsdavltree_change_string (bot_t *, char *, int, int);

void __bsdavltree_init__ (void) __attribute__ ((constructor));


enum bsdavltree_types
{
  BSDAVLTREE_TYPE_NUM = 1,
  BSDAVLTREE_TYPE_STR,
};

enum bsdavltree_traverse_types
{
  BSDAVLTREE_TRAVERSE_FORWARD,
  BSDAVLTREE_TRAVERSE_REVERSE
};

enum bsdavltree_opts
{
  BSDAVLTREE_OP_ADD = 1,
  BSDAVLTREE_OP_DEL,
  BSDAVLTREE_OP_INFO,
  BSDAVLTREE_OP_FIND,
  BSDAVLTREE_OP_CLEAR,
  BSDAVLTREE_OP_DEPTH,
  BSDAVLTREE_OP_TREE,
  BSDAVLTREE_OP_PREORDER,
  BSDAVLTREE_OP_INORDER,
  BSDAVLTREE_OP_POSTORDER,
};



#define xnew(T)         ((T *)calloc(1, sizeof(T)))
#define xfree(P)        (free(P))

typedef struct bsdavl_node
{
  int key_int;
  char *key_str;
  int value;
    TREE_ENTRY (bsdavl_node) tree;
} bsdavl_node_t;


typedef
TREE_HEAD (_Tree, bsdavl_node)
  Tree;
TREE_DEFINE (bsdavl_node, tree);

     typedef struct mod_bsdavl_trees
     {
/*
Tree tree = TREE_INITIALIZER(bsdavl_node_cmp);
Tree tree_str = TREE_INITIALIZER(bsdavl_node_cmp);
*/
       Tree tree;
       Tree tree_str;
       int bavlt_cnt_num;
       int bavlt_cnt_str;
     } bsdavl_trees_t;



     char *bsdavltree_op_add (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_del (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_info (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_find (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_clear (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_depth (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_tree (bsdavl_trees_t *, char *, int);
     char *bsdavltree_op_traverse (bsdavl_trees_t *, char *, int, int);


     bsdavl_node_t *bsdavl_node_new (char *, int, int);
     int bsdavl_node_cmp (bsdavl_node_t *, bsdavl_node_t *);
     void bsdavl_node_free (void *);
/*
void bsdavl_node_print(bsdavl_trees_t *, bsdavl_node_t *, FILE *);
void bsdavl_node_printer(bsdavl_trees_t *, bsdavl_node_t *, void *);
*/
     void bsdavl_node_fillbuf (bsdavl_node_t *, void *);
     void bsdavl_node_depth_fillbuf (bsdavl_node_t *, int, int, void *);
     void bsdavl_node_remove (bsdavl_node_t *, void *);
     Tree *bsdavl_tree_new (int (*cmp) (bsdavl_node_t *, bsdavl_node_t *));

     int bavlt_traverse;

     dlist_t *dl_mod_bsdavl_unique;

#endif
