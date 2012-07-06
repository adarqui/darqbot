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

#include "dlist.h"

dlist_t *
dlist_init (void *data, int type)
{
  dlist_t *d;


  d = (dlist_t *) calloc (1, sizeof (dlist_t));
  if (!d)
    return NULL;

  d->data = data;
  d->sz = 1;

/*
  if (type == DLIST_TYPE_NORMAL || type == DLIST_TYPE_CIRCULAR ||)
    d->type = type;
  else
    d->type = DLIST_TYPE_NORMAL;
*/
  if (type != DLIST_TYPE_NORMAL)
    d->type = type;
  else
    d->type = DLIST_TYPE_NORMAL;


  return d;
}





dlist_t *
dlist_tree_init (void *data,
		 int (*dl_avl_compare) (const void *, const void *, void *),
		 void (*dl_avl_free) (void *, void *))
{
  dlist_t *dptr = NULL;

  debug (NULL, "dlist_tree_init: Entered\n");

  if (!dl_avl_compare || !dl_avl_free)
    return NULL;

  dptr = dlist_init (data, DLIST_TYPE_AVL);
  if (!dptr)
    return NULL;

  dptr->avl_compare = dl_avl_compare;
  dptr->avl_free = dl_avl_free;

  dptr->avl_tree = avl_create (dptr->avl_compare, NULL, NULL);

  return dptr;
}


void *
dlist_tree_insert (dlist_t ** dl, void *data)
{
/* DO NOT CALL THIS DIRECTLY UNLESS 'DATA' IS A DLIST_T STRUCTURE */
  void *v = NULL;

//debug(NULL, "dlist_tree_insert: Entered\n");

  if (!dl || !data)
    return NULL;

  if (!(*dl))
    return NULL;

  if ((*dl)->type != DLIST_TYPE_AVL)
    return NULL;

  if (!(*dl)->avl_compare)
    return NULL;

  v = avl_insert ((*dl)->avl_tree, data);

//return v;
  return data;
}


void *
dlist_tree_delete (dlist_t ** dl, void *data)
{
/* DO NOT CALL THIS DIRECTLY UNLESS 'DATA' IS A DLIST_T STRUCTURE */
  void *v = NULL;

//debug(NULL, "dlist_tree_delete: Entered\n");

  if (!dl || !data)
    return NULL;

  if (!(*dl))
    return NULL;

  if ((*dl)->type != DLIST_TYPE_AVL)
    return NULL;

  if (!(*dl)->avl_compare)
    return NULL;

  v = avl_delete ((*dl)->avl_tree, data);

  return v;
}


void *
dlist_tree_delete_and_free (dlist_t ** dl, void *data)
{
/* DO NOT CALL THIS DIRECTLY UNLESS 'DATA' IS A DLIST_T STRUCTURE */
  void *v = NULL;

//debug(NULL, "dlist_tree_delete_and_free: Entered\n");

  if (!dl || !data)
    return NULL;

  if (!(*dl))
    return NULL;

  if ((*dl)->type != DLIST_TYPE_AVL)
    return NULL;

  v = dlist_tree_delete (dl, data);

  if (v)
    {
      if (!(*dl)->destroy)
	{
	  (*dl)->avl_free (v, (*dl)->avl_param);
	}
    }

  return NULL;
}




void *
dlist_tree_insert2 (dlist_t ** dl,
		    int (*avl_compare) (const void *, const void *, void *),
		    void *data)
{
/* DO NOT CALL THIS DIRECTLY UNLESS 'DATA' IS A DLIST_T STRUCTURE */
  dlist_t *dl_ptr = NULL;
  void *v = NULL;

//debug(NULL, "dlist_tree_insert2: Entered\n");

  if (!dl || !data)
    return NULL;

  dl_ptr = *dl;
  if (!(*dl))
    {
      dl_ptr = dlist_tree_init (NULL, avl_compare, NULL);
      if (!dl_ptr)
	return NULL;
      (*dl) = dl_ptr;
    }
  else
    {
      if (dl_ptr->type != DLIST_TYPE_AVL)
	return NULL;
    }

  v = avl_insert (dl_ptr->avl_tree, data);

  return v;
}



void
dlist_tree_fini (dlist_t ** dl, void (*fn) (void *, void *))
{

  debug (NULL, "dlist_tree_fini: Entered\n");

  if (!dl)
    return;

  if (!(*dl)->avl_tree)
    return;

  if ((*dl)->type != DLIST_TYPE_AVL)
    return;

  avl_destroy ((*dl)->avl_tree, fn != NULL ? fn : (*dl)->avl_free);

/*
if(dlist_size(*dl) <=0) {
free(*dl);
*dl = NULL;
}
*/

  free (*dl);
  *dl = NULL;


  return;
}



void
dlist_tree_print (dlist_t ** dl_tree, dlist_t ** dl_text,
		  void *(*fn_printer) (void *))
{

  if (!dl_tree || !dl_text)
    return;

  if (!(*dl_tree))
    return;
  if (!(*dl_tree)->avl_tree)
    return;

  dlist_tree_print_structure (dl_tree, dl_text, fn_printer,
			      (*dl_tree)->avl_tree->avl_root, 0);

  return;
}


void
dlist_tree_print_structure (dlist_t ** dl_tree, dlist_t ** dl_text,
			    void *(*fn_printer) (void *),
			    const struct avl_node *node, int level)
{
  dlist_t *dptr;

  if (!dl_tree || node == NULL || !dl_text)
    return;

  dptr = (dlist_t *) node->avl_data;

  dl_str_unite (dl_text, "%p-%s", dptr, fn_printer (dptr));

  if (node->avl_link[0] != NULL || node->avl_link[1] != NULL)
    {
      dl_str_unite (dl_text, "(");

      dlist_tree_print_structure (dl_tree, dl_text, fn_printer,
				  node->avl_link[0], level + 1);
      if (node->avl_link[1] != NULL)
	{
	  dl_str_unite (dl_text, ",");
	  dlist_tree_print_structure (dl_tree, dl_text, fn_printer,
				      node->avl_link[1], level + 1);
	}
      dl_str_unite (dl_text, ")");
    }

  return;
}




dlist_t *
dlist_tree_mirror (dlist_t * dl)
{
  dlist_t *dl_mir = NULL;
  if (!dl)
    return NULL;
  if (!dl->avl_tree)
    return NULL;
  dlist_tree_mirror_node (&dl_mir, dl->avl_tree->avl_root, 0);

  if (dl_mir)
    {
      dl_mir->subtype = DLIST_SUBTYPE_MIRROR;
    }

  return dl_mir;
}



void
dlist_tree_mirror_node (dlist_t ** dl, const struct avl_node *node, int level)
{
  dlist_t *dptr = NULL;
  if (!dl || !node)
    return;

  dptr = (dlist_t *) node->avl_data;
  if (!dptr)
    return;

/*
dptr = (dlist_t *) dlist_data(dptr);
if(!dptr) return;
*/

  dlist_Dinsert_after (dl, dptr->data);

  if (node->avl_link[0] != NULL || node->avl_link[1] != NULL)
    {
      dlist_tree_mirror_node (dl, node->avl_link[0], level + 1);
      if (node->avl_link[1] != NULL)
	{
	  dlist_tree_mirror_node (dl, node->avl_link[1], level + 1);
	}
    }

  return;
}



void *
dlist_traverse (dlist_t ** dl, void *(*fn) (void *, void *), void *data)
{
  void *v = NULL;


  if (!dl || !fn)
    return NULL;
  if (!(*dl))
    return NULL;

  switch ((*dl)->type)
    {
    case DLIST_TYPE_NORMAL:
    case DLIST_TYPE_CIRCULAR:
      {
	v = dlist_traverse_list (dl, fn, data);
	break;
      }
    case DLIST_TYPE_TREE:
      {
	v = dlist_traverse_tree (dl, fn, data);
	break;
      }
    case DLIST_TYPE_HASH:
      {
	break;
      }
    case DLIST_TYPE_ARRAY:
      {
	break;
      }
    default:
      break;
    }

  return v;
}


void *
dlist_traverse_list (dlist_t ** dl, void *(*fn) (void *, void *), void *data)
{
  dlist_t *dptr_a, *dptr_b;
  void *v = NULL;


  if (!dl || !fn)
    return NULL;

  if (!(*dl))
    return NULL;

  dlist_fornext_retarded ((*dl), dptr_a, dptr_b)
  {
    if (!dptr_a)
      break;
    v = fn (data, dptr_a);
    if (v)
      {
	return v;
      }
  }

  return v;
}


void *
dlist_traverse_tree (dlist_t ** dl, void *(*fn) (void *, void *), void *data)
{
  void *v = NULL;


  if (!dl || !fn)
    return NULL;
  if (!(*dl))
    return NULL;

  if (!(*dl)->avl_tree)
    return NULL;


  v = dlist_traverse_tree_node (dl, (*dl)->avl_tree->avl_root, fn, data);


  return v;
}



void *
dlist_traverse_tree_node (dlist_t ** dl, const struct avl_node *node,
			  void *(*fn) (void *, void *), void *data)
{
  dlist_t *dptr = NULL;
  void *v = NULL;


  if (!dl || !node || !fn)
    return NULL;

  if (!(*dl))
    return NULL;

  dptr = (dlist_t *) node->avl_data;
  if (!dptr)
    return NULL;


  v = fn (data, dptr);
  if (v)
    return v;


  if (node->avl_link[0] != NULL || node->avl_link[1] != NULL)
    {
      v = dlist_traverse_tree_node (dl, node->avl_link[0], fn, data);
      if (v)
	return v;

      if (node->avl_link[1] != NULL)
	{
	  v = dlist_traverse_tree_node (dl, node->avl_link[1], fn, data);
	  if (v)
	    return v;
	}
    }

  return v;
}




dlist_t *
dlist_tree_find_dptr (dlist_t ** dl, void *node)
{

  dlist_t *dptr = NULL;

  if (!dl || !node)
    return NULL;
  if (!(*dl))
    return NULL;
  if (!(*dl)->avl_tree)
    return NULL;

  dptr = avl_find ((*dl)->avl_tree, node);

  return dptr;
}



int
dlist_tree_size (dlist_t ** dl)
{

  if (!dl)
    return 0;

  if (!(*dl))
    return 0;

  if (!(*dl)->avl_tree)
    return 0;

  return avl_count ((*dl)->avl_tree);
}



void *
dlist_tree_find (dlist_t ** dl, void *node)
{
  dlist_t *dptr = NULL;
  void *v = NULL;


  dptr = dlist_tree_find_dptr (dl, node);


  if (dptr)
    {
      v = (void *) dlist_data (dptr);
    }

  return v;
}



void
dlist_tree_reorder (dlist_t ** dl, dlist_t ** dl_mirror)
{
  dlist_t *dptr = NULL;

  if (!dl || !dl_mirror)
    return;
  if (!(*dl))
    return;

  if (!(*dl)->type == DLIST_TYPE_AVL)
    return;

  if (!(*dl)->avl_tree)
    return;


//avl_destroy((*dl)->avl_tree, NULL);
  dlist_traverse (dl, dlist_tree_reorder_copy2mirror, dl_mirror);

  (*dl)->avl_tree = avl_create ((*dl)->avl_compare, NULL, NULL);

  dlist_fornext (*dl_mirror, dptr)
  {
    dlist_Dinsert_after (dl, dlist_data (dptr));
  }

//dlist_tree_traverse(dl, dlist_tree_reorder_copy2mirror, &dl_mirror);

  return;
}




void *
dlist_tree_reorder_copy2mirror (void *pa, void *pb)
{
  dlist_t **dl = (dlist_t **) pa;
  void *v = NULL;
  module_t *m = NULL;

  dlist_t *dptr_v = NULL;

  if (!pa || !pb)
    return NULL;

/*
v = (vid * ) pb;
if(!v) return NULL;

dptr_v = (
*/
  dptr_v = (dlist_t *) pb;
  if (!dptr_v)
    return NULL;

  v = (void *) dlist_data (dptr_v);

  m = (module_t *) v;

  dlist_Dinsert_after (dl, m);

  return NULL;
}



dlist_t *
dlist_alloc (void *data)
{
  dlist_t *d;
  d = (dlist_t *) calloc (1, sizeof (dlist_t));
  if (!d)
    return NULL;
  d->data = data;
  d->type = DLIST_TYPE_NORMAL;
  return d;
}

void
dlist_free (dlist_t ** removed_elm, void (*destroy) (void *))
{
  dlist_t *dptr;


  if (!removed_elm)
    return;

  if (!(*removed_elm))
    return;

  dptr = *removed_elm;

  if (destroy && dptr->data)
    destroy (dptr->data);

  dptr->data = NULL;

  *removed_elm = NULL;

  free (dptr);

  return;
}



void
dlist_remove_and_free (dlist_t ** head, dlist_t ** elm,
		       void (*destroy) (void *))
{
  dlist_t *dptr;

  if (!head || !elm || !destroy)
    return;

  if (!(*head))
    return;

  if ((*head)->type == DLIST_TYPE_AVL)
    {
      dlist_tree_delete (head, elm);
      return;
    }

  if ((*head)->type != DLIST_TYPE_NORMAL
      && (*head)->type != DLIST_TYPE_CIRCULAR)
    return;

  if (!(*elm))
    return;

  dptr = *(elm);
  if (!dptr)
    return;

/* XXX
  dlist_remove (head, dptr);
  dlist_free (elm, destroy);
*/
  dptr = dlist_remove (head, dptr);
  dlist_free (&dptr, destroy);
//*elm=NULL;

  return;
}


dlist_t *
dlist_insert_after (dlist_t ** head, dlist_t * elm, dlist_t * elm_new)
{
  dlist_t *e;


  if (!head)
    return NULL;


  if (!elm_new)
    return NULL;

  if (!*head)
    {
/* head does not exist, this will be head */
      *head = elm_new;
      dlist_nullhtnp (elm_new);

      (*head)->sz++;

      (*head)->type = DLIST_TYPE_NORMAL;


      return elm_new;
    }

  if ((*head)->type == DLIST_TYPE_AVL)
    {
      return dlist_tree_insert (head, elm_new);
    }

  if ((*head)->type != DLIST_TYPE_NORMAL
      && (*head)->type != DLIST_TYPE_CIRCULAR)
    return NULL;


  if (!elm_new->type)
    elm_new->type = (*head)->type;

  if (!elm)
    {
/* insert at the end of the list */
      if (!(*head)->next)
	{
	  (*head)->tail = elm_new;
	  (*head)->next = elm_new;
	  elm_new->head = *head;
	  elm_new->prev = *head;

	  elm_new->type = (*head)->type;

	  if (dlist_iscirc (*head))
	    {
	      elm_new->next = *head;
	      (*head)->prev = elm_new;
	    }
	  else
	    {
	      elm_new->tail = NULL;
	      elm_new->next = NULL;
	    }

	}

      else
	{
/* next exists, so, tail exists, fix tail and head */
	reuse1:

	  e = (*head)->tail;

	  e->next = elm_new;
	  elm_new->prev = e;
	  elm_new->head = *head;

	  if (dlist_iscirc (*head))
	    {
	      (*head)->prev = elm_new;
	      elm_new->next = (*head);
	    }
	  else
	    {
	      dlist_nullht (e);
	    }
	  (*head)->tail = elm_new;
	}

      (*head)->sz++;
      return elm_new;
    }

  if (elm == (*head)->tail)
    {
/* create new tail */
      goto reuse1;
    }

  if (elm == (*head)->head)
    {
/* insert after head, no need to update tail/head ptr's */
      e = (*head)->next;
      e->prev = elm_new;
      elm_new->next = e;
      elm_new->prev = (*head);
      (*head)->next = elm_new;
      dlist_nullht (elm_new);

      (*head)->sz++;


      return elm_new;
    }

/* last scenario, insert between two nodes, no need to update tail or head */
  e = elm->next;
  elm->next = elm_new;
  elm_new->next = e;
  elm_new->prev = elm;
  e->prev = elm_new;
  dlist_nullht (elm_new);

  (*head)->sz++;


  return elm_new;
}

dlist_t *
dlist_insert_before (dlist_t ** head, dlist_t * elm, dlist_t * elm_new)
{
  dlist_t *e;

  if (!head)
    return NULL;

  if (!elm_new)
    return NULL;

  if (!*head)
    {
/* head does not exist, this will be head */
      *head = elm_new;
      dlist_nullht (elm_new);
      (*head)->sz++;

      (*head)->type = DLIST_TYPE_NORMAL;

      return elm_new;
    }

  if ((*head)->type == DLIST_TYPE_AVL)
    {
      return dlist_tree_insert (head, elm_new);
    }

  if ((*head)->type != DLIST_TYPE_NORMAL
      && (*head)->type != DLIST_TYPE_CIRCULAR)
    return NULL;

  if (!elm_new->type)
    elm_new->type = (*head)->type;

  if (!elm)
    {
/* insert at the beginning of the list, new head */
      if (!(*head)->next)
	{
	  e = *head;

	  *head = elm_new;
	  elm_new->next = e;
	  e->prev = elm_new;
	  elm_new->tail = e;
	  e->head = elm_new;
	  elm_new->type = e->type;

	  if (dlist_iscirc (*head))
	    {
	      elm_new->prev = e;
	      e->next = elm_new;
	    }
	  else
	    {
	      elm_new->prev = NULL;
	      elm_new->head = NULL;
	    }

	  (*head)->sz = e->sz;
	}

      else
	{
/* next exists, so, tail exists, fix tail and head */
	reuse1:

	  e = *head;

	  *head = elm_new;
	  elm_new->next = e;
	  elm_new->tail = e->tail;
	  e->prev = elm_new;
	  elm_new->tail->head = elm_new;
	  elm_new->type = e->type;

	  dlist_nullht (e);
	  e->tail = NULL;

	  if (dlist_iscirc (elm_new))
	    {
	      elm_new->prev = elm_new->tail;
	      elm_new->tail->next = elm_new;
	    }
	  else
	    {
	      elm_new->prev = NULL;
	      elm_new->head = NULL;
	    }

	  (*head)->sz = e->sz;
	}

      (*head)->sz++;
      return elm_new;
    }

  if (elm == (*head))
    {
/* create new head (insert before head) */
      goto reuse1;
    }

/* last scenario, insert between two nodes, no need to update tail or head */
  e = elm->prev;

  e->next = elm_new;
  elm_new->next = elm;
  elm_new->prev = e;
  elm->prev = elm_new;

  dlist_nullht (elm_new);

  (*head)->sz++;
  return elm_new;
}

dlist_t *
dlist_Dinsert_after (dlist_t ** head, void *data)
{
  dlist_t *dptr;

  dptr = dlist_alloc (data);
  if (!dptr)
    return NULL;

  if (!(*head))
    {
      dptr->type = DLIST_TYPE_NORMAL;
    }
  else
    {
      dptr->type = (*head)->type;
    }

  dptr = dlist_insert_after (head, NULL, dptr);

  return dptr;
}

dlist_t *
dlist_Dinsert_before (dlist_t ** head, void *data)
{
  dlist_t *dptr;

  dptr = dlist_alloc (data);
  if (!dptr)
    return NULL;

  if (!(*head))
    {
      dptr->type = DLIST_TYPE_NORMAL;
    }
  else
    {
      dptr->type = (*head)->type;
    }

  return dlist_insert_before (head, NULL, dptr);
}


void
dlist_remove_and_free_wrapper (dlist_t ** head, dlist_t ** elm,
			       void (*destroy) (void *))
{

  dlist_remove_and_free (head, elm, destroy);

  return;
}

dlist_t *
dlist_remove_wrapper (dlist_t ** head, dlist_t * elm)
{
  dlist_t *dptr = NULL;

  dptr = dlist_remove (head, elm);

  return dptr;
}


dlist_t *
dlist_remove (dlist_t ** head, dlist_t * elm)
{
  dlist_t *e1, *e2;


  if (!head || !elm)
    return NULL;
  if (!(*head))
    return NULL;

  if ((*head)->type == DLIST_TYPE_AVL)
    {
      return dlist_tree_delete (head, elm);
    }

  if ((*head)->type != DLIST_TYPE_NORMAL
      && (*head)->type != DLIST_TYPE_CIRCULAR)
    return NULL;

  if (elm == *head)
    {
/* remove the head, make new head, fix tail */
      if (elm->next == NULL)
	{
/* head is the only element, removing it, list is now completely empty */
	  (*head)->sz--;

	  dlist_nullhtnp (elm);
	  *head = NULL;

	  return elm;
	}

      else if (elm->next == elm->tail)
	{
/* only one node will be in the list now */
	  e1 = elm->tail;
	  *head = e1;

	  if (dlist_iscirc (*head))
	    {
	      (*head)->next = *head;
	      (*head)->prev = *head;
	    }
	  else
	    {
	      dlist_nullhtnp (e1);
	      dlist_nullhtnp (elm);
	    }

	  (*head)->sz--;

	  return elm;
	}

      else
	{
/* multiple nodes in the list, just relink head & tail */
	  e1 = elm->tail;
	  e2 = elm->next;

	  e1->head = e2;
	  e2->tail = e1;
	  e2->prev = NULL;

	  e2->sz = (*head)->sz;

	  e2->type = (*head)->type;
	  e1->type = (*head)->type;

	  if (dlist_iscirc (*head))
	    {
	      e1->head->prev = e2->tail;
	      e2->tail->next = e1->head;
	    }
	  else
	    {
	      dlist_nullhtnp (elm);
	    }

	  *head = e2;
	  (*head)->sz--;
	  return elm;
	}
    }

  if (elm == (*head)->tail)
    {
/* remove the tail, make new tail, fix tail */
      if (elm->prev == *head)
	{
/* head is the only node left */

	  if (dlist_iscirc (*head))
	    {
	      (*head)->next = *head;
	      (*head)->prev = *head;
	    }
	  else
	    {
	      dlist_nullhtnp (*head);
	    }

	  (*head)->sz--;
	  return elm;
	}
      else
	{
/* remove tail, make new tail */
	  e1 = elm->prev;

	  e1->head = *head;
	  e1->next = NULL;
	  (*head)->tail = e1;

	  e1->type = elm->type;

	  if (dlist_iscirc (*head))
	    {
	      (*head)->prev = e1;
	      e1->next = *head;
	    }
	  else
	    {
	      dlist_nullhtnp (elm);
	    }

	  (*head)->sz--;
	  return elm;
	}

    }


/* remove a node between head & tail, no relinking of head & tail */
  e1 = elm->prev;
  e2 = elm->next;

  e1->next = e2;
  e2->prev = e1;

  dlist_nullhtnp (elm);

  (*head)->sz--;
  return elm;
}

dlist_t *
dlist_enqueue (dlist_t ** head, dlist_t * elm)
{
  if (!head || !elm)
    return NULL;
  if (!(*head))
    return NULL;

  return dlist_insert_before (head, *head, DLIST_TYPE_NORMAL);
}

dlist_t *
dlist_Denqueue (dlist_t ** head, void *data)
{
  dlist_t *dptr;

  dptr = dlist_alloc (data);
  if (!dptr)
    return NULL;

  if (!(*head))
    {
      dptr->type = DLIST_TYPE_NORMAL;
    }
  else
    {
      dptr->type = (*head)->type;
    }

  return dlist_insert_before (head, NULL, dptr);
}

dlist_t *
dlist_dequeue (dlist_t ** head)
{
  dlist_t *tail;
  if (!head)
    return NULL;

  if (!(*head))
    return NULL;

  tail = dlist_tail ((*head));
  if (tail == NULL || dlist_size ((*head)) == 1)
    tail = *head;

  return dlist_remove (head, tail);
}

dlist_t *
dlist_push (dlist_t ** head, dlist_t * elm)
{

  if (!head || !elm)
    return NULL;
  if (!(*head))
    return NULL;

  return dlist_insert_before (head, *head, DLIST_TYPE_NORMAL);
}

dlist_t *
dlist_pop (dlist_t ** head)
{

  if (!head)
    return NULL;

  return dlist_remove (head, *head);
}


void
dlist_pop_and_free (dlist_t ** head, void (*fn) (void *))
{
  dlist_t *dptr = NULL;

  if (!head)
    return;

  dptr = dlist_remove (head, *head);
  if (!dptr)
    return;

  if (fn)
    {
      fn (dlist_data (dptr));
    }

  free (dptr);

  return;
}


dlist_t *
dlist_Dpush (dlist_t ** head, void *data)
{

  dlist_t *dptr;

  dptr = dlist_alloc (data);
  if (!dptr)
    return NULL;

  if (!(*head))
    {
      dptr->type = DLIST_TYPE_NORMAL;
    }
  else
    {
      dptr->type = (*head)->type;
    }

  return dlist_insert_before (head, NULL, dptr);
}


int
dlist_fini (dlist_t ** head, void (*destroy) (void *))
{
  dlist_t *dptr, *dptr_2;


  if (!head)
    return -1;
  if (!(*head))
    return -1;

/*
  dptr = dptr_2 = NULL;
  do
    {
      dptr = dlist_remove (head, *head);
      if (dptr != NULL)
	{
* FIXED: 04/22/2012, dlist_free(dptr, free) <-- wrong, changed to destroy *
	  dlist_free (dptr, destroy);
	  dptr = NULL;
	}
    }
  while (dptr != NULL);
*/

  if ((*head)->type == DLIST_TYPE_AVL)
    {
      dlist_tree_fini (head, (*head)->avl_free);
      return 0;
    }


  if ((*head)->type != DLIST_TYPE_NORMAL
      && (*head)->type != DLIST_TYPE_CIRCULAR)
    return -1;

  dptr = dptr_2 = NULL;
  dlist_fornext_retarded (*head, dptr, dptr_2)
  {
    if (dptr == NULL)
      break;
    dlist_remove (head, dptr);
    dlist_free (&dptr, destroy);
  }

  *head = NULL;

  return 0;
}




int
dlist_make_circular (dlist_t ** head_or_tail)
{
  dlist_t *head, *tail;

  if (!head_or_tail)
    return -1;
  if (!(*head_or_tail))
    return -1;

  if ((*head_or_tail)->type != DLIST_TYPE_NORMAL
      && (*head_or_tail)->type != DLIST_TYPE_CIRCULAR)
    return -1;

  if (dlist_iscirc (*head_or_tail))
    return -1;

  head =
    (*head_or_tail)->tail ? (*head_or_tail) : (*head_or_tail)->
    head ? (*head_or_tail)->head : NULL;
  tail =
    (*head_or_tail)->head ? (*head_or_tail) : (*head_or_tail)->
    tail ? (*head_or_tail)->tail : NULL;

  if (head == NULL)
    return -1;


  head->type = DLIST_TYPE_CIRCULAR;
  if (tail == NULL)
    {
      head->next = head;
      head->prev = head;
      head->tail = head;
      head->head = head;
    }
  else
    {
      head->prev = tail;
      tail->next = head;
      head->tail = tail;
      tail->head = head;
    }

  return 0;
}

int
dlist_make_uncircular (dlist_t ** head_or_tail)
{
  dlist_t *head, *tail;

  if (!head_or_tail)
    return -1;
  if (!(*head_or_tail))
    return -1;

  if ((*head_or_tail)->type != DLIST_TYPE_NORMAL
      && (*head_or_tail)->type != DLIST_TYPE_CIRCULAR)
    return -1;

  if (!dlist_iscirc (*head_or_tail))
    return -1;

  head = (*head_or_tail)->tail ? (*head_or_tail) : (*head_or_tail)->head;
  tail = (*head_or_tail)->head ? (*head_or_tail) : (*head_or_tail)->tail;

  head->prev = NULL;
  tail->next = NULL;
  head->type = tail->type = DLIST_TYPE_NORMAL;

  return 0;
}



int
dlist_relink (dlist_t * new_head, dlist_t * old_head)
{

  return 0;
}



int
dlist_next_retarded (dlist_t ** c, dlist_t * b)
{
  if (!b)
    return 1;

  *c = b->next;
  return 1;
}

int
dlist_prev_retarded (dlist_t ** c, dlist_t * b)
{
  if (!b)
    return 1;
  *c = b->prev;
  return 1;
}







void **
dlist_convert_dlist_to_array (dlist_t * dl)
{
  dlist_t *dptr;
  void **array;
  int i;

  if (!dl)
    return NULL;

  if (dlist_size (dl) <= 0)
    return NULL;


  array = (void **) calloc (dlist_size (dl) + 1, sizeof (void *));
  if (!array)
    return NULL;

  i = 0;
  dlist_fornext (dl, dptr)
  {
    array[i] = dlist_data (dptr);
    i++;
  }

  return array;
}




void **
dlist_convert_dlist_to_array_new (dlist_t * dl)
{
  dlist_t *dptr;
  void **array;
  int i;
  char *str = NULL;

  if (!dl)
    return NULL;

  if (dlist_size (dl) <= 0)
    return NULL;


  array = (void **) calloc (dlist_size (dl) + 1, sizeof (void *));
  if (!array)
    return NULL;

  i = 0;
  dlist_fornext (dl, dptr)
  {
    str = (char *) dlist_data (dptr);
    array[i] = strdup (str);
    i++;
  }

  return array;
}



void **
dlist_convert_dlist_to_dlistarray (dlist_t * dl)
{
  dlist_t *dptr;
  void **array;
  int i;

  if (!dl)
    return NULL;

  if (dlist_size (dl) <= 0)
    return NULL;


  array = (void **) calloc (dlist_size (dl) + 1, sizeof (dlist_t *));
  if (!array)
    return NULL;

  i = 0;
  dlist_fornext (dl, dptr)
  {
    array[i] = dptr;
    i++;
  }

  return array;
}





void
dlist_array_fini (void **array, void (*fn) (void *))
{
  int i;

  if (!array)
    return;

  for (i = 0; array[i] != NULL; i++)
    {

      if (fn)
	{
	  fn (array[i]);
	}

    }

  free (array);

  return;
}










/* fix these to support any type */


dlist_t *
dlist_node_rand (dlist_t * dl)
{
  dlist_t *dptr = NULL, *dptr_ret = NULL;
  int rand_index = 0;
  int dl_sz = 0;
  int i = 0;

  if (!dl)
    return NULL;

  dl_sz = dlist_size (dl);

  if (dl_sz > 1)
    {
      rand_index = rand () % (dl_sz);
    }
  else
    {
      rand_index = 0;
    }

  i = 0;
  dlist_fornext (dl, dptr)
  {
    if (i == rand_index)
      {
	dptr_ret = dptr;
	break;
      }
    i++;
  }

  return dptr_ret;
}




char *
array_to_str (void **array, int count, char *(*fn) (void *))
{
  dlist_t *dl = NULL;
  char *str = NULL;
  int i = 0;

  for (i = 0; i < count; i++)
    {
      if (array[i] == NULL)
	break;

      dlist_Dinsert_after (&dl, fn (array[i]));
    }

  str = dlist_to_str (dl);
  dlist_fini (&dl, free);

  return str;
}


char *
arraystr_to_str (char **array, int count, char *append)
{
  char *str = NULL, *tmp_str = NULL, *tmp_str_2 = NULL;
  int i = 0;

  debug (NULL, "arraystr_to_str: Entered\n");

  if (!array)
    return NULL;

  for (i = 0; i < count && array[i] != NULL; i++)
    {
      tmp_str = (char *) array[i];
      if (!tmp_str)
	continue;

      if (str)
	{
	  tmp_str_2 = str;
	  str = str_unite ("%s%s%s", str, append, tmp_str);
	  free (tmp_str_2);
	}
      else
	{
	  str = strdup (tmp_str);
	}

    }

  return str;
}




char *
dlist_to_str (dlist_t * dl)
{
/* assumes dl is made up of strings */
  dlist_t *dptr = NULL;
  char *str = NULL, *tmp_str = NULL, *tmp_str_2 = NULL;

  debug (NULL, "dlist_to_str: Entered\n");

  if (!dl)
    return NULL;
  dlist_fornext (dl, dptr)
  {
    tmp_str = (char *) dlist_data (dptr);
    if (!tmp_str)
      continue;

    if (str)
      {
	tmp_str_2 = str;
	str = str_unite ("%s%s", str, tmp_str);
	free (tmp_str_2);
      }
    else
      {
	str = strdup (tmp_str);
      }

  }

  return str;
}






char *
dlist_to_str_fix (dlist_t * dl, char *prefix, char *suffix)
{
/* assumes dl is made up of strings */
  dlist_t *dptr = NULL;
  char *str = NULL, *tmp_str = NULL, *tmp_str_2 = NULL;

  debug (NULL, "dlist_to_str_fix: Entered\n");

  if (!dl)
    return NULL;
  dlist_fornext (dl, dptr)
  {
    tmp_str = (char *) dlist_data (dptr);

    if (str)
      {
	tmp_str_2 = str;


/*
        str = str_unite ("%s%s", str, tmp_str);
*/
	str =
	  str_unite ("%s%s%s%s", str, prefix != NULL ? prefix : "", tmp_str,
		     suffix != NULL ? suffix : "");
	free (tmp_str_2);
      }
    else
      {
/*
        str = strdup (tmp_str);
*/
	str =
	  str_unite ("%s%s%s", prefix != NULL ? prefix : "", tmp_str,
		     suffix != NULL ? suffix : "");
      }

  }

  return str;
}
