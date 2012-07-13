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
#include "mod_env.h"

void __env_init__(void)
{

	strlcpy_buf(mod_env_info.name, "mod_env");
	strlcpy_buf(mod_env_info.trigger, "^env");

	module_add_subtrigs(&mod_env_info, "^setenv");
	module_add_subtrigs(&mod_env_info, "^unsetenv");
	module_add_subtrigs(&mod_env_info, "^getenv");

	mod_env_info.init = env_init;
	mod_env_info.fini = env_fini;
	mod_env_info.help = env_help;
	mod_env_info.run = env_run;

	mod_env_info.output = NULL;
	mod_env_info.input = NULL;

	debug(NULL, "__env_init__: Loaded mod_env\n");

	return;
}

bot_t *env_init(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "env_init: Entered\n");

	swap_inmem_get_assign_and_remove("dl_mod_env", 0,
					 (void **)&dl_mod_env_unique);

	return NULL;
}

bot_t *env_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "env_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_env", 0, dl_mod_env_unique);

	return NULL;
}

bot_t *env_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "env_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^env || ^env(setenv,unsetenv,size,list,clear)";

	return NULL;
}

bot_t *env_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "env_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "env_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = MOD_ENV_GETENV;

	if (!strcasecmp(bot->trig_called, "^setenv")) {
		opt = MOD_ENV_SETENV;
	} else if (!strcasecmp(bot->trig_called, "^unsetenv")) {
		opt = MOD_ENV_UNSETENV;
	} else if (!strcasecmp(bot->trig_called, "getenv")) {
		opt = MOD_ENV_GETENV;
	}

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "setenv")) {
		opt = MOD_ENV_SETENV;
	} else if (!strncasecmp_len(dl_options_ptr, "unsetenv")) {
		opt = MOD_ENV_UNSETENV;
	} else if (!strncasecmp_len(dl_options_ptr, "getenv")) {
		opt = MOD_ENV_GETENV;
	} else if (!strncasecmp_len(dl_options_ptr, "size")) {
		opt = MOD_ENV_SIZE;
	} else if (!strncasecmp_len(dl_options_ptr, "clear")) {
		opt = MOD_ENV_CLEAR;
	} else if (!strncasecmp_len(dl_options_ptr, "list")) {
		opt = MOD_ENV_LIST;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = env_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *env_change_string(bot_t * bot, char *string, int opt)
{
	struct avl_table **tree = NULL;
	unique_t *bu = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	debug(NULL, "env_change_string: Entered\n");

	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	bu = unique_create(bot, &dl_mod_env_unique,
			   UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
	if (!bu)
		return NULL;

	tree = (struct avl_table **)&bu->data;

	switch (opt) {
	case MOD_ENV_SETENV:
		{
			str = env_op_setenv(tree, string);
			break;
		}
	case MOD_ENV_UNSETENV:
		{
			str = env_op_unsetenv(tree, string);
			break;
		}
	case MOD_ENV_CLEAR:
		{
			str = env_op_clear(tree, string);
			break;
		}
	case MOD_ENV_SIZE:
		{
			str = env_op_size(tree, string);
			break;
		}
	case MOD_ENV_LIST:
		{
			str = env_op_list(tree, string);
			break;
		}
	case MOD_ENV_GETENV:
		{
			str = env_op_getenv(tree, string);
			break;
		}
	default:
		{
			break;
		}
	}

	return str;
}

char *env_op_setenv(struct avl_table **tree, char *string)
{
	env_t *e = NULL, *e2 = NULL;
	char *tok_name = NULL, *tok_value = NULL;

	debug(NULL, "env_op_setenv: Entered: tree=%p\n", tree);

	if (!tree || !string)
		return NULL;

	if (!(*tree)) {
		*tree = avl_create(env_compare, NULL, NULL);
		if (!(*tree))
			return NULL;
	}

	tok_name = strtok(string, "=");
	if (!sNULL(tok_name))
		return NULL;

	tok_value = strtok(NULL, "");
	if (!sNULL(tok_value))
		return NULL;

	e = (env_t *) calloc(1, sizeof(env_t));
	if (!e)
		return NULL;

	strlcpy_buf(e->name, tok_name);
	strlcpy_buf(e->value, tok_value);

	e2 = avl_find(*tree, e);
	if (e2) {
		strlcpy_buf(e2->value, tok_value);
		free(e);
		return NULL;
	}

	avl_insert(*tree, e);

	return NULL;
}

char *env_op_getenv(struct avl_table **tree, char *string)
{
	env_t *e = NULL, e_tmp;
	char *str = NULL;

	if (!tree || !string)
		return NULL;
	if (!(*tree))
		return NULL;

	bz2(e_tmp);
	strlcpy_buf(e_tmp.name, string);

	e = (env_t *) avl_find(*tree, &e_tmp);
	if (e) {
		str = strdup(e->value);
	}
	return str;
}

char *env_op_unsetenv(struct avl_table **tree, char *string)
{
	env_t e;
	char *str = NULL;

	debug(NULL, "env_op_unsetenv: Entered: tree=%p\n", tree);

	if (!tree || !string)
		return NULL;

	if (!(*tree))
		return NULL;

	bz2(e);
	strlcpy_buf(e.name, string);
	avl_delete(*tree, &e);

	return str;
}

char *env_op_clear(struct avl_table **tree, char *string)
{
	char *str = NULL;

	debug(NULL, "env_op_clear: Entered\n");

	if (!tree || !string)
		return NULL;

	if (!(*tree))
		return NULL;

	avl_destroy(*tree, env_freex);
	*tree = NULL;

	return str;
}

char *env_op_size(struct avl_table **tree, char *string)
{
	char *str = NULL;
	int sz;

	debug(NULL, "env_op_size: Entered: tree=%p\n", tree);

	if (!tree || !string)
		return NULL;

	if (!(*tree))
		return NULL;

	sz = avl_count(*tree);
	str = str_unite("%i", sz);

	return str;
}

char *env_op_list(struct avl_table **tree, char *string)
{
	dlist_t *dl_text = NULL;
	char *str = NULL;

	debug(NULL, "env_op_list: Entered\n");

	if (!tree || !string)
		return NULL;

	if (!(*tree))
		return NULL;

	env_list_whole_tree(&dl_text, *tree);

	str = dlist_to_str(dl_text);

	dl_str_destroy(&dl_text);

	return str;
}

int env_compare(const void *pa, const void *pb, void *param)
{
	env_t *a = (env_t *) pa, *b = (env_t *) pb;

	if (!a || !b)
		return -1;

	return strcasecmp(a->name, b->name);
}

void env_freex(void *avl_item, void *avl_param)
{
	env_t *e = NULL;

	e = (env_t *) avl_item;
	if (!e)
		return;

	free(e);

	return;
}

void env_list_whole_tree(dlist_t ** dl_text, const struct avl_table *tree)
{
	env_list_tree_structure(dl_text, tree->avl_root, 0);
	return;
}

static void
env_list_tree_structure(dlist_t ** dl_text, const struct avl_node *node,
			int level)
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

	if (node->avl_link[0] != NULL || node->avl_link[1] != NULL) {
//      putchar ('(');
		dl_str_unite(dl_text, "(");

		env_list_tree_structure(dl_text, node->avl_link[0], level + 1);
		if (node->avl_link[1] != NULL) {
			//putchar (',');
			dl_str_unite(dl_text, ",");
			env_list_tree_structure(dl_text, node->avl_link[1],
						level + 1);
		}
		//putchar (')');
//dlist_Dinsert_after(l_text, bot_strup(")"));
		dl_str_unite(dl_text, ")");
	}
}
