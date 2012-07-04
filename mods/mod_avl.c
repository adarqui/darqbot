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
#include "mod_avl.h"

void
__avl_init__ (void)
{

strlcpy_buf(mod_avl_info.name, "mod_avl");
strlcpy_buf(mod_avl_info.trigger, "^avl");

  module_add_subtrigs (&mod_avl_info, "^avl_ins");
  module_add_subtrigs (&mod_avl_info, "^avl_del");
  module_add_subtrigs (&mod_avl_info, "^avl_clear");
  module_add_subtrigs (&mod_avl_info, "^avl_list");
  module_add_subtrigs (&mod_avl_info, "^avl_tree");

  mod_avl_info.init = avl_init;
  mod_avl_info.fini = avl_fini;
  mod_avl_info.help = avl_help;
  mod_avl_info.run = avl_run;


  mod_avl_info.output = NULL;
  mod_avl_info.input = NULL;


  debug (NULL, "__avl_init__: Loaded mod_avl\n");

  return;
}



bot_t *
avl_init (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "avl_init: Entered\n");

  swap_inmem_get_assign_and_remove ("dl_mod_avl", 0,
					(void **) &dl_mod_avl_unique);

  return NULL;
}

bot_t *
avl_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "avl_fini: Entered\n");

  swap_inmem_create_and_add ("dl_mod_avl", 0, dl_mod_avl_unique);

  return NULL;
}

bot_t *
avl_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "avl_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^avl";

  return NULL;
}

bot_t *
avl_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "avl_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "avl_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  if (!strcasecmp (bot->trig_called, "^avl_ins"))
    {
      opt = MOD_AVL_INSERT;
    }
  else if (!strcasecmp (bot->trig_called, "^avl_del"))
    {
      opt = MOD_AVL_DELETE;
    }
else if(!strcasecmp(bot->trig_called, "^avl_clear")) {
opt=MOD_AVL_CLEAR;
}
else if(!strcasecmp(bot->trig_called, "^avl_size")) {
opt = MOD_AVL_SIZE;
}
else if(!strcasecmp(bot->trig_called, "^avl_list")) {
opt = MOD_AVL_LIST;
}
else if(!strcasecmp(bot->trig_called, "^avl_tree")) {
opt = MOD_AVL_TREE;
}


  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "ins"))
    {
      opt = MOD_AVL_INSERT;
    }
  else if (!strncasecmp_len (dl_options_ptr, "del"))
    {
      opt = MOD_AVL_DELETE;
    }
else if(!strncasecmp_len(dl_options_ptr,"size")) {
opt = MOD_AVL_SIZE;
}
  else if (!strncasecmp_len (dl_options_ptr, "clear"))
    {
      opt = MOD_AVL_CLEAR;
    }
  else if (!strncasecmp_len (dl_options_ptr, "tree"))
    {
      opt = MOD_AVL_TREE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "list"))
    {
      opt = MOD_AVL_LIST;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = avl_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
avl_change_string (bot_t * bot, char *string, int opt)
{
struct avl_table **tree=NULL;
  unique_t *bu = NULL;
  char *str = NULL;
  char buf[MAX_BUF_SZ];

debug(NULL, "avl_change_string: Entered\n");

  char *sep_ptr;

  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  bu =
    unique_create (bot, &dl_mod_avl_unique,
		       UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
  if (!bu)
    return NULL;

tree = (struct avl_table **) &bu->data;

switch(opt) {
case MOD_AVL_INSERT: {
str= avl_op_insert(tree, string);
break;
}
case MOD_AVL_DELETE: {
str = avl_op_delete(tree, string);
break;
}
case MOD_AVL_CLEAR:{
str=avl_op_clear(tree,string);
break;
}
case MOD_AVL_SIZE: {
str = avl_op_size(tree,string);
break;
}
case MOD_AVL_LIST: {
str = avl_op_list(tree,string);
break;
}
case MOD_AVL_TREE: {
str = avl_op_tree(tree,string);
break;
}
default: {
break;
}
}

  return str;
}




char * avl_op_insert(struct avl_table **tree, char * string ) {
dlist_t * dl=NULL, *dptr;
char * str=NULL, *dup=NULL;

debug(NULL, "avl_op_insert: Entered: tree=%p\n", tree);

if(!tree || !string) return NULL;

if(!(*tree)) {
*tree = avl_create(avl_compare, NULL, NULL);
if(!(*tree)) return NULL;
}

dl = tokenize(NULL, string,TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
if(!dl) return NULL;

dlist_fornext(dl,dptr) {
dup = (char *)dlist_data(dptr);
dup = strdup(dup);
if(!dup)
continue;
str_apply_is(dup, isalnum);
if(!sNULL(dup)) continue;
avl_insert(*tree, dup);
}

tokenize_destroy(NULL, &dl);

return str;
}





char * avl_op_delete(struct avl_table **tree, char * string ) {
char * str=NULL;

debug(NULL, "avl_op_delete: Entered: tree=%p\n", tree);

if(!tree || !string) return NULL;

if(!(*tree)) return NULL;

avl_delete(*tree, string);

return str;
}



char  * avl_op_clear(struct avl_table **tree, char * string ) {
char * str=NULL;

debug(NULL, "avl_op_clear: Entered\n");

if(!tree || !string) return NULL;

if(!(*tree)) return  NULL;

avl_destroy(*tree, avl_freex);
*tree= NULL;

return str;
}



char * avl_op_size(struct avl_table **tree, char * string ) {
char * str=NULL;
int sz;

debug(NULL, "avl_op_size: Entered: tree=%p\n",tree);

if(!tree || !string) return NULL;

if(!(*tree)) return NULL;

sz = avl_count(*tree);
str = str_unite("%i",sz); 

return str;
}


char * avl_op_list(struct avl_table **tree, char * string ) {
dlist_t *dl_text=NULL;
char * str=NULL;

debug(NULL, "avl_op_list: Entered\n");

if(!tree || !string) return NULL;

if(!(*tree)) return NULL;

avl_list_whole_tree(&dl_text, *tree);

str = dlist_to_str(dl_text);

dl_str_destroy(&dl_text);

return str;
}



char * avl_op_tree(struct avl_table **tree, char * string ) {
char * str=NULL, *str_tree_list=NULL;

debug(NULL, "avl_op_tree: Entered\n");

if(!tree || !string) return NULL;

if(!(*tree)) return NULL;

str_tree_list = avl_op_list(tree, string);
if(!str_tree_list) return NULL;

str=texitree_str(str_tree_list);
free(str_tree_list);

str = dlist_to_str(g_dl_texilib);
dl_str_destroy(&g_dl_texilib);

return str;
}



int avl_compare(const void * pa, const void * pb, void * param) {
const char *a=pa, *b=pb;

return strcasecmp(a,b);
}


void avl_freex (void *avl_item, void *avl_param) {
char * str=NULL;

str = (char *) avl_item;
if(!str) return;

free(str);

return ;
}









void avl_list_whole_tree (dlist_t ** dl_text, const struct avl_table *tree) {
  avl_list_tree_structure (dl_text, tree->avl_root, 0);
return;
}


static void
avl_list_tree_structure (dlist_t ** dl_text, const struct avl_node *node, int level)
{
/*
  if (level > 16)
    {
      printf ("[...]");
      return;
    }
*/

  if (node == NULL || !dl_text)
    return;

dl_str_unite(dl_text, "%s", (char *)node->avl_data);

  if (node->avl_link[0] != NULL || node->avl_link[1] != NULL)
    {
//      putchar ('(');
dl_str_unite(dl_text, "(");

      avl_list_tree_structure (dl_text, node->avl_link[0], level + 1);
      if (node->avl_link[1] != NULL)
        {
          //putchar (',');
dl_str_unite(dl_text, ",");
          avl_list_tree_structure (dl_text, node->avl_link[1], level + 1);
        }

      //putchar (')');
//dlist_Dinsert_after(l_text, bot_strup(")"));
dl_str_unite(dl_text, ")");
    }
}

