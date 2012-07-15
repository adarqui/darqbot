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
#include "mod_bsdavltree.h"

void __bsdavltree_init__(void)
{

	strlcpy_buf(mod_bsdavltree_info.name, "mod_bsdavltree");
	strlcpy_buf(mod_bsdavltree_info.trigger, "^bsdavltree");

	module_add_subtrigs(&mod_bsdavltree_info, "^bavlt");

	mod_bsdavltree_info.init = bsdavltree_init;
	mod_bsdavltree_info.fini = bsdavltree_fini;
	mod_bsdavltree_info.help = bsdavltree_help;
	mod_bsdavltree_info.run = bsdavltree_run;

	mod_bsdavltree_info.output = NULL;
	mod_bsdavltree_info.input = NULL;

	debug(NULL, "__bsdavltree_init__: Loaded mod_bsdavltree\n");

	return;
}

bot_t *bsdavltree_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "bsdavltree_init: Entered\n");

	bavlt_traverse = BSDAVLTREE_TRAVERSE_REVERSE;

	swap_inmem_get_assign_and_remove("dl_mod_bsdavl", 0,
					 (void **)&dl_mod_bsdavl_unique);

	return NULL;
}

bot_t *bsdavltree_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "bsdavltree_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_bsdavl", 0, dl_mod_bsdavl_unique);

	return NULL;
}

bot_t *bsdavltree_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "bsdavltree_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^bsdavltree";

	return NULL;
}

bot_t *bsdavltree_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt_type, opt_op;

	char *tok_type, *tok_op;

	debug(bot, "bsdavltree_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "bsdavltree_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt_type = BSDAVLTREE_TYPE_STR;
	opt_op = BSDAVLTREE_OP_ADD;

	MOD_OPTIONS_TOP_HALF;

	if (strcasestr(dl_options_ptr, "reverse")) {
		bavlt_traverse = BSDAVLTREE_TRAVERSE_REVERSE;
	} else if (strcasestr(dl_options_ptr, "forward")) {
		bavlt_traverse = BSDAVLTREE_TRAVERSE_FORWARD;
	}

	tok_type = strtok(dl_options_ptr, ",");
	if (!tok_type)
		return NULL;
	tok_op = strtok(NULL, "");
	if (!tok_op)
		return NULL;

	if (!strcasecmp(tok_type, "num")) {
		opt_type = BSDAVLTREE_TYPE_NUM;
	} else if (!strcasecmp(tok_type, "str")) {
		opt_type = BSDAVLTREE_TYPE_STR;
	}

	if (!strcasecmp(tok_op, "add")) {
		opt_op = BSDAVLTREE_OP_ADD;
	} else if (!strcasecmp(tok_op, "del")) {
		opt_op = BSDAVLTREE_OP_DEL;
	} else if (!strcasecmp(tok_op, "info")) {
		opt_op = BSDAVLTREE_OP_INFO;
	} else if (!strcasecmp(tok_op, "find")) {
		opt_op = BSDAVLTREE_OP_FIND;
	} else if (!strcasecmp(tok_op, "clear")) {
		opt_op = BSDAVLTREE_OP_CLEAR;
	} else if (!strcasecmp(tok_op, "depth")) {
		opt_op = BSDAVLTREE_OP_DEPTH;
	} else if (!strcasecmp(tok_op, "tree")) {
		opt_op = BSDAVLTREE_OP_TREE;
	} else if (!strcasecmp(tok_op, "preorder")) {
		opt_op = BSDAVLTREE_OP_PREORDER;
	} else if (!strcasecmp(tok_op, "inorder")) {
		opt_op = BSDAVLTREE_OP_INORDER;
	} else if (!strcasecmp(tok_op, "postorder")) {
		opt_op = BSDAVLTREE_OP_POSTORDER;
	}

	tok_op = _eat_whitespace(tok_op);

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = bsdavltree_change_string(bot, l_str_ptr, opt_op, opt_type);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *bsdavltree_change_string(bot_t * bot, char *string, int opt_op,
			       int opt_type)
{
	unique_t *bu = NULL;
	bsdavl_trees_t *bt = NULL;
	char *str = NULL;
	char *sep_ptr;

	debug(NULL, "bsdavltree_change_string: Entered\n");

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bu = unique_create(bot, &dl_mod_bsdavl_unique,
			   UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
	if (!bu)
		return NULL;

	bt = (bsdavl_trees_t *) bu->data;
	if (!bt) {
		bt = (bsdavl_trees_t *) calloc(1, sizeof(bsdavl_trees_t));
		if (!bt)
			return NULL;
		bu->data = bt;

		TREE_INITIALIZER2(&bt->tree, bsdavl_node_cmp);
		TREE_INITIALIZER2(&bt->tree_str, bsdavl_node_cmp);
	}

	switch (opt_op) {
	case BSDAVLTREE_OP_ADD:
		{
			str = bsdavltree_op_add(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_DEL:
		{
			str = bsdavltree_op_del(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_INFO:
		{
			str = bsdavltree_op_info(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_FIND:
		{
			str = bsdavltree_op_find(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_CLEAR:
		{
			str = bsdavltree_op_clear(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_DEPTH:
		{
			str = bsdavltree_op_depth(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_TREE:
		{
			str = bsdavltree_op_tree(bt, string, opt_type);
			break;
		}
	case BSDAVLTREE_OP_PREORDER:
	case BSDAVLTREE_OP_INORDER:
	case BSDAVLTREE_OP_POSTORDER:
		{
			str =
			    bsdavltree_op_traverse(bt, string, opt_op,
						   opt_type);
			break;
		}
	default:
		{
			break;
		}
	}

	return str;
}

char *bsdavltree_op_find(bsdavl_trees_t * bt, char *string, int opt_type)
{
	Tree *tree_ptr = NULL;
	char *str = NULL, *tok = NULL;
	char buf[MAX_BUF_SZ];

	int x = 0;

	int *i;

	bsdavl_node_t v;
	bsdavl_node_t **vv;
	int val_int = 0;

	debug(NULL, "bsdavltree_op_find: Entered\n");
	if (!bt || !string)
		return NULL;

	tok = strtok(string, " ");
	if (!tok)
		return NULL;

	_memset(buf, 0, sizeof(buf));
	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			i = &bt->bavlt_cnt_num;
			v.key_str = NULL;
			v.key_int = atoi(tok);
			v.value = *i;
			vv = (void *)TREE_FIND_COUNT(&x, (Tree *) tree_ptr,
						     bsdavl_node, tree, &v);
			val_int = v.key_int;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			i = &bt->bavlt_cnt_str;
			v.key_str = tok;
			v.key_int = 0;
			v.value = *i;
			vv = (void *)TREE_FIND_COUNT(&x, (Tree *) tree_ptr,
						     bsdavl_node, tree, &v);
			break;
		}
	default:
		return NULL;
	}

	if (vv) {
		str = str_unite("%s found: count=%i\n", tok, x);
	}

	return str;
}

char *bsdavltree_op_depth(bsdavl_trees_t * bt, char *string, int opt_type)
{
	Tree *tree_ptr = NULL;
	char *str = NULL;

	int depth = 0;

	debug(NULL, "bsdavltree_op_depth: Entered\n");

	if (!bt)
		return NULL;

	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			break;
		}
	default:
		return NULL;
	}

	depth = TREE_DEPTH(tree_ptr, tree);

	str = str_unite("depth = %i\n", depth);

	return str;
}

char *bsdavltree_op_clear(bsdavl_trees_t * bt, char *string, int opt_type)
{
	Tree *tree_ptr = NULL;
	char *str = NULL;

	debug(NULL, "bsdavltree_op_clear: Entered\n");

	if (!bt)
		return NULL;

	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			break;
		}
	default:
		return NULL;
	}

	TREE_DESTROY(tree_ptr, bsdavl_node, tree, bsdavl_node_free);

	return str;
}

char *bsdavltree_op_info(bsdavl_trees_t * bt, char *string, int opt_type)
{
	void *tree_ptr = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	debug(NULL, "bsdavltree_op_info: Entered\n");

	if (!bt)
		return NULL;

	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			break;
		}
	default:
		return NULL;
	}

	_memset(buf, 0, sizeof(buf));
	if (bavlt_traverse == BSDAVLTREE_TRAVERSE_REVERSE) {
		TREE_REVERSE_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
				   bsdavl_node_fillbuf, buf);
	} else {
		TREE_FORWARD_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
				   bsdavl_node_fillbuf, buf);
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *bsdavltree_op_traverse(bsdavl_trees_t * bt, char *string, int opt_op,
			     int opt_type)
{
	void *tree_ptr = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	int x, y;
	x = y = 0;

	debug(NULL, "bsdavltree_op_traverse: Entered\n");

	if (!bt)
		return NULL;

	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			break;
		}
	default:
		return NULL;
	}

	_memset(buf, 0, sizeof(buf));

	switch (opt_op) {
	case BSDAVLTREE_OP_PREORDER:
		{
			TREE_DEPTH_TRAVERSE_PREORDER(x, y, (Tree *) tree_ptr,
						     bsdavl_node, tree,
						     bsdavl_node_depth_fillbuf,
						     buf);
			break;
		}
	case BSDAVLTREE_OP_INORDER:
		{
			TREE_DEPTH_TRAVERSE_INORDER(x, y, (Tree *) tree_ptr,
						    bsdavl_node, tree,
						    bsdavl_node_depth_fillbuf,
						    buf);
			break;
		}
	case BSDAVLTREE_OP_POSTORDER:
		{
			TREE_DEPTH_TRAVERSE_POSTORDER(x, y, (Tree *) tree_ptr,
						      bsdavl_node, tree,
						      bsdavl_node_depth_fillbuf,
						      buf);
			break;
		}
	default:
		break;
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *bsdavltree_op_tree(bsdavl_trees_t * bt, char *string, int opt_type)
{
	void *tree_ptr = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	int x, y;
	x = y = 0;

	debug(NULL, "bsdavltree_op_tree: Entered\n");

	if (!bt)
		return NULL;

	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			break;
		}
	default:
		return NULL;
	}

	_memset(buf, 0, sizeof(buf));
	if (bavlt_traverse == BSDAVLTREE_TRAVERSE_REVERSE) {
		TREE_DEPTH_REVERSE_APPLY(x, y, (Tree *) tree_ptr, bsdavl_node,
					 tree, bsdavl_node_depth_fillbuf, buf);
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *bsdavltree_op_add(bsdavl_trees_t * bt, char *string, int opt_type)
{
	dlist_t *dl_toks, *dptr_tok;
	char *tok = NULL;
	void *tree_ptr;
	char *str = NULL;
	char buf[MAX_BUF_SZ];
	int *i;
	bsdavl_node_t v;
	bsdavl_node_t **vv;
	int val_int = 0;

	debug(NULL, "bsdavltree_op_add: Entered\n");
	if (!bt || !string)
		return NULL;

	dl_toks =
	    tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_MATCHANY,
		     " ,./;'[]~<>\\`");
	if (!dl_toks)
		return NULL;

	_memset(buf, 0, sizeof(buf));

	dlist_fornext(dl_toks, dptr_tok) {

		tok = (char *)dlist_data(dptr_tok);

		switch (opt_type) {
		case BSDAVLTREE_TYPE_NUM:
			{
				tree_ptr = &bt->tree;
				i = &bt->bavlt_cnt_num;
				v.key_str = NULL;
				v.key_int = atoi(tok);
				v.value = *i;
				vv = (void *)TREE_FIND((Tree *) tree_ptr,
						       bsdavl_node, tree, &v);
				val_int = v.key_int;
				break;
			}
		case BSDAVLTREE_TYPE_STR:
			{
				tree_ptr = &bt->tree_str;
				i = &bt->bavlt_cnt_str;
				v.key_str = tok;
				v.key_int = 0;
				v.value = *i;
				vv = (void *)TREE_FIND((Tree *) tree_ptr,
						       bsdavl_node, tree, &v);
				break;
			}
		default:
			return NULL;
		}

		if (vv) {
		} else {
			i++;

			TREE_INSERT((Tree *) tree_ptr, bsdavl_node, tree,
				    bsdavl_node_new(v.key_str !=
						    NULL ? strdup(v.key_str) :
						    v.key_str, v.key_int,
						    v.value));
		}
	}

	if (bavlt_traverse == BSDAVLTREE_TRAVERSE_REVERSE) {
		TREE_REVERSE_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
				   bsdavl_node_fillbuf, buf);
	} else {
		TREE_FORWARD_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
				   bsdavl_node_fillbuf, buf);
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *bsdavltree_op_del(bsdavl_trees_t * bt, char *string, int opt_type)
{
	Tree *tree_ptr;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	int *i;

	bsdavl_node_t v;
	bsdavl_node_t *vv = NULL;

	int val_int = 0;

	debug(NULL, "bsdavltree_op_del: Entered\n");
	if (!bt || !string)
		return NULL;

	_memset(buf, 0, sizeof(buf));
	switch (opt_type) {
	case BSDAVLTREE_TYPE_NUM:
		{
			tree_ptr = &bt->tree;
			i = &bt->bavlt_cnt_num;
			v.key_str = NULL;
			v.key_int = atoi(string);
			v.value = *i;
			vv = TREE_FIND((Tree *) tree_ptr, bsdavl_node, tree,
				       &v);
			val_int = v.key_int;
			break;
		}
	case BSDAVLTREE_TYPE_STR:
		{
			tree_ptr = &bt->tree_str;
			i = &bt->bavlt_cnt_str;
			v.key_str = string;
			v.key_int = 0;
			v.value = *i;
			vv = TREE_FIND((Tree *) tree_ptr, bsdavl_node, tree,
				       &v);
			break;
		}
	default:
		return NULL;
	}

	if (!vv) {
	} else {
		i++;

		TREE_REMOVE(tree_ptr, bsdavl_node, tree, vv);
		bsdavl_node_free(vv);

		if (bavlt_traverse == BSDAVLTREE_TRAVERSE_REVERSE) {
			TREE_REVERSE_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
					   bsdavl_node_fillbuf, buf);
		} else {
			TREE_FORWARD_APPLY((Tree *) tree_ptr, bsdavl_node, tree,
					   bsdavl_node_fillbuf, buf);
		}
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

bsdavl_node_t *bsdavl_node_new(char *key_str, int key_int, int value)
{
	bsdavl_node_t *self = xnew(bsdavl_node_t);
	self->key_str = key_str;
	self->key_int = key_int;
	self->value = value;
	return self;
}

int bsdavl_node_cmp(bsdavl_node_t * left, bsdavl_node_t * right)
{
	if (right->key_str && left->key_str)
		return strcasecmp(right->key_str, left->key_str);

	return right->key_int - left->key_int;
}

void bsdavl_node_print(bsdavl_node_t * self, FILE * stream)
{
}

void bsdavl_node_printer(bsdavl_node_t * self, void *stream)
{
	bsdavl_node_print(self, (FILE *) stream);
	fprintf((FILE *) stream, " ");
}

void bsdavl_node_fillbuf(bsdavl_node_t * self, void *buf)
{

	char *buf_ptr = (char *)buf;

	if (!self || !buf)
		return;

	if (self->key_str) {
		strlcatfmt_safe(buf_ptr, MAX_BUF_SZ - 1, "%s,", self->key_str);
	} else {
		strlcatfmt_safe(buf_ptr, MAX_BUF_SZ - 1, "%d,", self->key_int);
	}
	return;
}

void bsdavl_node_depth_fillbuf(bsdavl_node_t * self, int x, int y, void *buf)
{

	char *buf_ptr = (char *)buf;

	if (!self || !buf)
		return;

	if (self->key_str) {
		strlcatfmt_safe(buf_ptr, MAX_BUF_SZ - 1, "%s{x=%i,y=%i},",
				self->key_str, x, y);
	} else {
		strlcatfmt_safe(buf_ptr, MAX_BUF_SZ - 1, "%d{x=%i,y=%i},",
				self->key_int, x, y);
	}
	return;
}

void bsdavl_node_remove(bsdavl_node_t * self, void *tree_arg)
{

	if (!self || !tree_arg)
		return;

	TREE_REMOVE((Tree *) tree_arg, bsdavl_node, tree, self);

	return;
}

Tree *bsdavl_tree_new(int (*cmp) (bsdavl_node_t * left, bsdavl_node_t * right))
{
	Tree *self = xnew(Tree);
	TREE_INIT(self, cmp);
	return self;
}

void bsdavl_node_free(void *arg)
{
	bsdavl_node_t *bavlt_ptr = (bsdavl_node_t *) arg;

	if (bavlt_ptr->key_str)
		free(bavlt_ptr->key_str);

	free(bavlt_ptr);

	return;
}
