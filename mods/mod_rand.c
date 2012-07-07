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
#include "mod_rand.h"

void __rand_init__(void)
{

	strlcpy_buf(mod_rand_info.name, "mod_rand");
	strlcpy_buf(mod_rand_info.trigger, "^rand");

	mod_rand_info.init = rand_init;
	mod_rand_info.fini = rand_fini;
	mod_rand_info.help = rand_help;
	mod_rand_info.run = rand_run;

	mod_rand_info.output = NULL;
	mod_rand_info.input = NULL;

	debug(NULL, "__rand_init__: Loaded mod_rand\n");

	return;
}

bot_t *rand_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rand_init: Entered\n");
	return NULL;
}

bot_t *rand_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rand_fini: Entered\n");
	return NULL;
}

bot_t *rand_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rand_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^rand || ^rand(data|word:num,val)";

	return NULL;
}

bot_t *rand_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "rand_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "rand_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = RAND_WORD;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "word"))
		opt = RAND_WORD;
	else if (!strncasecmp_len(dl_options_ptr, "num")) {
		opt = RAND_NUM;
	} else if (!strncasecmp_len(dl_options_ptr, "data")) {
		opt = RAND_DATA;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = rand_change_string(l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *rand_change_string(char *string, int opt)
{
	char *sep_ptr;
	char *str = NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	switch (opt) {
	case RAND_DATA:
		{
			str = rand_op_data(string);
			break;
		}
	case RAND_NUM:
		{
			str = rand_op_num(string);
			break;
		}
	case RAND_WORD:
		{
			str = rand_op_word(string);
			break;
		}
	default:
		break;
	}

	return str;
}

char *rand_op_word(char *string)
{
	int i, word_array[500], word_cnt, rand_val;
	char *str = NULL, *str_ptr = NULL;

	if (!sNULL(string))
		return NULL;

	memset(word_array, 0, sizeof(word_array));

/* return a random word */
	word_cnt = 0;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == ' ') {
			for (i = i; i < strlen(string) && string[i] == ' ';
			     i++) ;
			word_array[word_cnt] = i;
			word_cnt++;
		}

	}

	if (word_cnt == 0)
		return NULL;
	word_cnt++;

 reloop:
	rand_val = rand() % word_cnt;

	str_ptr = string + word_array[rand_val];
	while (*str_ptr == ' ')
		str_ptr++;
	str = strdup(str_ptr);
	str_ptr = str;
	while (*str_ptr) {
		if (*str_ptr == ' ') {
			*str_ptr = '\0';
			break;
		}
		str_ptr++;
	}

	if (strlen(str) == 0) {
		free(str);
		goto reloop;
	}

	return str;
}

char *rand_op_num(char *string)
{
	char *str = NULL;
	int rand_val = 0;

	rand_val = rand();
	str = str_unite("%i", rand_val);

	return str;
}

char *rand_op_data(char *string)
{
	int sz = 512, i = 0;
	char *data = NULL;

	debug(NULL, "rand_op_data: Entered\n");

	data = random_data(sz + 1);
	if (!data) {
		return NULL;
	}

	for (i = 0; i < sz; i++) {
		if (data[i] == '\0') {
			while (data[i] == '\0')
				data[i] = (char)rand();
		}
	}

	return data;
}
