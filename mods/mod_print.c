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
#include "mod_print.h"

void __print_init__(void)
{

	strlcpy_buf(mod_print_info.name, "mod_print");
	strlcpy_buf(mod_print_info.trigger, "^print");

	module_add_subtrigs(&mod_print_info, "^p");

	mod_print_info.init = print_init;
	mod_print_info.fini = print_fini;
	mod_print_info.help = print_help;
	mod_print_info.run = print_run;

	mod_print_info.output = NULL;
	mod_print_info.input = NULL;

	debug(NULL, "__print_init__: Loaded mod_print\n");

	return;
}

bot_t *print_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "print_init: Entered\n");
	return NULL;
}

bot_t *print_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "print_fini: Entered\n");
	return NULL;
}

bot_t *print_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "print_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^print || ^p(b/c) ... ^p(rb/c) || ^p(b/n) ... ^p(rb/n)";

	return NULL;
}

bot_t *print_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *str;
	int opt, opt_2, c, c_op;

	debug(bot, "print_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "print_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = opt_2 = 0;

	MOD_OPTIONS_TOP_HALF;

	str = _eat_whitespace(dl_options_ptr);
	if (strlen(str) == 0)
		return NULL;

	if (tolower(str[0]) == 'r') {
		opt_2 = PRINT_OPT_REVERSE;
		c = tolower(str[1]);
		c_op = str[3];
	} else {
		opt_2 = PRINT_OPT_FORWARD;
		c = tolower(str[0]);
		c_op = str[2];
	}

	switch (c) {
	case 'b':
		{
/* BINARY */
			opt = PRINT_OPT_BINARY;
			break;
		}
	case 'h':
	case 'x':
		{
/* HEX */
			opt = PRINT_OPT_HEX;
			break;
		}
	case 'c':
		{
/* CHAR */
			opt = PRINT_OPT_CHAR;
			break;
		}

	default:
		break;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = print_change_string(l_str_ptr, opt, opt_2, c_op);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *print_change_string(char *string, int opt, int opt_2, int c_op)
{
	char *str = NULL;
	char *sep_ptr;

	debug(NULL, "print_change_string: Entered\n");

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	switch (opt) {
	case PRINT_OPT_BINARY:
		{
			str = print_op_binary(opt_2, c_op, string);
			break;
		}
	case PRINT_OPT_HEX:
		{
			break;
		}
	case PRINT_OPT_CHAR:
		{
			break;
		}
	default:
		break;
	}

	return str;
}

char *print_op_binary(int for_or_rev, int c_op, char *string)
{

	debug(NULL, "print_op_binary: Entered\n");

	if (for_or_rev < 0 || c_op < 0 || !string)
		return NULL;

	switch (for_or_rev) {
	case PRINT_OPT_FORWARD:
		{
			return print_op_binary_forward(c_op, string);
			break;
		}
	case PRINT_OPT_REVERSE:
		{
			return print_op_binary_reverse(c_op, string);
			break;
		}
	default:
		return NULL;
	}

	return NULL;
}

char *print_op_binary_forward(int c_op, char *string)
{
	char *str = NULL;

	debug(NULL, "print_op_binary_forward: Entered\n");

	if (c_op < 0 || !_sNULL(string))
		return NULL;

	switch (c_op) {
	case 'c':
		{
			str = print_op_binary_forward_char(string);
			break;
		}
	case 'n':
		{
			str = print_op_binary_forward_num(string);
			break;
		}
	default:
		break;
	}

	return str;
}

char *print_op_binary_forward_char(char *string)
{
	int i, j, res;
	char *str = NULL;
	char buf[MAX_BUF_SZ + 1];

	debug(NULL, "print_op_binary_forward_char: Entered\n");

	if (!string)
		return NULL;

	_memset(buf, 0, sizeof(buf));

	for (i = 0; i < strlen(string); i++) {
		for (j = 0; j < 8; j++) {
			res = bit_extract_bit_char(string[i], j);
			strlcatfmt_buf(buf, "%i", res);
		}
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *print_op_binary_forward_num(char *num)
{
	bit_vector_t *bv = NULL;
	char *str = NULL, *b_index_ptr = NULL;
	int b_index = 0, i;

	debug(NULL, "print_op_binary_forward_num: Entered\n");

	bv = bit_vec_init(8);
	if (!bv)
		return NULL;

	bit_vec_from_str(bv, num);

	str = bit_vec_to_str(bv);

	bit_vec_fini(&bv);

	if (str) {
		b_index_ptr = _strrchr(str, '1');
		if (b_index_ptr) {
			b_index = b_index_ptr - str;
			for (i = b_index; str[i] != '\0'; i++) {

				if (!((i + 1) % 8)) {
					str[i + 1] = '\0';
					break;
				}

			}

		} else {
			str[1] = '\0';
		}

	}

	return str;
}

char *print_op_binary_reverse(int c_op, char *string)
{
	char *str = NULL;

	debug(NULL, "print_op_binary_reverse: Entered\n");

	if (c_op < 0 || !string)
		return NULL;

	switch (c_op) {
	case 'c':
		{
			str = print_op_binary_reverse_char(string);
			break;
		}
	case 'n':{
			str = print_op_binary_reverse_num(string);
			break;
		}
	default:
		break;
	}

	return str;
}

char *print_op_binary_reverse_num(char *num)
{

	debug(NULL, "print_op_binary_reverse_num: Entered\n");

	return NULL;
}

char *print_op_binary_reverse_char(char *string)
{
	int i, j, res;
	char *str = NULL;
	char buf[MAX_BUF_SZ + 1];
	unsigned char byte;
	int binary_val;

	debug(NULL, "print_op_binary_reverse_char: Entered\n");

	if (!string)
		return NULL;

	_memset(buf, 0, sizeof(buf));

	byte = 0;
	j = 0;
	for (i = 0; i < strlen(string); i++) {
		if (!isbinary(string[i])) {
			if (j > 0) {
				charcat_safe(buf, byte, sizeof(buf) - 1);
			}
			j = 0;
			byte = 0;
			continue;
		}

		if (string[i] == '1')
			binary_val = 1;
		else
			binary_val = 0;

		res = bit_pack_bit_char(&byte, j, binary_val);
		j++;

		if (j > 7) {
			charcat_safe(buf, byte, sizeof(buf) - 1);
			j = 0;
			byte = 0;
		}

	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}
