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
#ifndef MOD_AVL_H
#define MOD_AVL_H

#include "bot.h"
#include "texilib.h"

/*
#include "helvetica.inc"
#define font helv_font
*/

enum mod_avl_stuff
{
  MOD_AVL_INSERT = 1,
  MOD_AVL_DELETE,
  MOD_AVL_CLEAR,
  MOD_AVL_SIZE,
  MOD_AVL_LIST,
  MOD_AVL_TREE,
};

dlist_t *dl_mod_avl_unique;

module_t mod_avl_info;

bot_t *avl_init (dlist_t *, bot_t *);
bot_t *avl_fini (dlist_t *, bot_t *);
bot_t *avl_help (dlist_t *, bot_t *);
bot_t *avl_run (dlist_t *, bot_t *);


char *avl_change_string (bot_t *, char *, int);
void __avl_init__ (void) __attribute__ ((constructor));

char *avl_op_insert (struct avl_table **, char *);
char *avl_op_delete (struct avl_table **, char *);
char *avl_op_clear (struct avl_table **, char *);
char *avl_op_size (struct avl_table **, char *);
char *avl_op_list (struct avl_table **, char *);
char *avl_op_tree (struct avl_table **, char *);

void avl_list_whole_tree (dlist_t **, const struct avl_table *);
static void avl_list_tree_structure (dlist_t **, const struct avl_node *,
				     int);
int avl_compare (const void *, const void *, void *);
void avl_freex (void *, void *);

#endif
