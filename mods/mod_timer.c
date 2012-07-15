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
#include "mod_timer.h"

void __timer_init__(void)
{

	strlcpy_buf(mod_timer_info.name, "mod_timer");
	strlcpy_buf(mod_timer_info.trigger, "^timer");

	mod_timer_info.init = timer_init;
	mod_timer_info.fini = timer_fini;
	mod_timer_info.help = timer_help;
	mod_timer_info.run = timer_run;
	mod_timer_info.timer = timer_timer;

	mod_timer_info.output = NULL;
	mod_timer_info.input = NULL;

	debug(NULL, "__timer_init__: Loaded mod_timer\n");

	return;
}

bot_t *timer_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "timer_init: Entered\n");

	swap_inmem_get_assign_and_remove("dl_mod_timer", 0,
					 (void **)&dl_mod_timer);
	swap_inmem_get_assign_and_remove("timer_bv", 0, (void **)&timer_bv);

	if (!timer_bv) {
		timer_bv = bit_vec_init(8);
	}

	return NULL;
}

bot_t *timer_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "timer_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_timer", 0, dl_mod_timer);
	swap_inmem_create_and_add("timer_bv", 0, timer_bv);

	return NULL;
}

bot_t *timer_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "timer_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^timer";

	return NULL;
}

bot_t *timer_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "timer_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "timer_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	if (!strcasecmp(dl_options_ptr, "add")) {
		opt = TIMER_OPT_ADD;
	} else if (!strcasecmp(dl_options_ptr, "del")) {
		opt = TIMER_OPT_DEL;
	} else if (!strcasecmp(dl_options_ptr, "get")) {
		opt = TIMER_OPT_GET;
	} else if (!strcasecmp(dl_options_ptr, "list")) {
		opt = TIMER_OPT_LIST;
	} else if (!strcasecmp(dl_options_ptr, "clear")) {
		opt = TIMER_OPT_CLEAR;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = timer_change_string(dlist_node, bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *timer_change_string(dlist_t * dlist_node, bot_t * bot, char *string,
			  int opt)
{
	char *str = NULL;

	char *sep_ptr;

	if (!dlist_node || !bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	switch (opt) {
	case TIMER_OPT_ADD:
		{
			str = timer_op_add(bot, string);
			break;
		}
	case TIMER_OPT_DEL:
		{
			str = timer_op_del(string);
			break;
		}
	case TIMER_OPT_GET:
		{
			str = timer_op_get(string);
			break;
		}
	case TIMER_OPT_LIST:
		{
			str = timer_op_list(string);
			break;
		}
	case TIMER_OPT_CLEAR:
		{
			str = timer_op_clear(string);
			break;
		}
	default:
		break;
	}

	return str;
}

bot_t *timer_timer(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr, *dptr_tmp;
	bot_t *bot_copy = NULL;
	modtimer_t *t;
	time_t time_cur;

	bot_t *(*orig_pmod_cur) (dlist_t *, bot_t *);
	dlist_t *orig_pmod_cur_dlist_node;
	bot_t *orig_pmod_cur_bot;

	debug(bot, "mod_timer: Entered: %p, %p\n", dlist_node, bot);

	orig_pmod_cur = gi->pmod_cur;
	orig_pmod_cur_dlist_node = gi->pmod_cur_dlist_node;
	orig_pmod_cur_bot = gi->pmod_cur_bot;

	dlist_fornext_retarded(dl_mod_timer, dptr, dptr_tmp) {
		if (!dptr)
			break;
		t = (modtimer_t *) dlist_data(dptr);
		if (!t)
			continue;

		time(&time_cur);

		if ((time_cur - t->last_run) > t->interval && t->bot->on) {
/* timer expired, run cmd */
			bot_copy = t->bot;
			gi->pmod_cur = t->parser;
			gi->pmod_cur_dlist_node = t->dlist_node;
			gi->pmod_cur_bot = bot_copy;
			bot_copy_values(bot_copy, "TIMER", "TIMER", "TIMER.com",
					t->chan, t->cmd, t->cmd);
			time(&t->last_run);
			bot_copy->isprivmsg = 1;
			bot_copy->var_nesting_cur = 0;
			bot_copy->mute = 0;
			pmodule_cur_run2(bot_copy);
			gmodule_down(t->dlist_node, bot_copy);

			if (t->count > 0)
				t->count = t->count - 1;

			if (t->count == 0) {
				dlist_remove_and_free(&dl_mod_timer, &dptr,
						      timer_free);
			}

/* will this do it?? - y e s */
			if (bot_copy->isforked) {
				bot_fork_clean_exit(bot_copy);
			}
		}
	}

	gi->pmod_cur = orig_pmod_cur;
	gi->pmod_cur_dlist_node = orig_pmod_cur_dlist_node;
	gi->pmod_cur_bot = orig_pmod_cur_bot;

	return bot;
}

char *timer_op_add(bot_t * bot, char *string)
{
	modtimer_t *t;
	dlist_t *dl = NULL;
	char **array = NULL;
	char *str = NULL;

	int interval, count;

	interval = count = 0;

	debug(bot, "timer_op_add: Entered\n");

	if (!bot || !string)
		return NULL;

	dl = tokenize(bot, string,
		      TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE |
		      TOKENIZE_IGNORECASE, ":ttt:");
	if (!dl)
		return NULL;

	array = (char **)dlist_convert_dlist_to_array(dl);
	if (!array)
		goto cleanup;

	if (!array[0])
		goto cleanup;
	if (!array[1])
		goto cleanup;
	if (!array[2])
		goto cleanup;

	interval = atoi(array[0]);

	if (!strcasecmp(array[1], "forever")) {
		count = -1;
	} else {
		count = atoi(array[1]);
	}

	t = timer_alloc(bot, interval, count, array[2]);

 cleanup:
	if (dl)
		tokenize_destroy(bot, &dl);

	if (array)
		free(array);

	return str;
}

char *timer_op_del(char *string)
{
	dlist_t *dptr, *dptr_tmp;
	modtimer_t *t;
	int id;
	char *str = NULL;

	debug(NULL, "timer_op_del: Entered\n");
	if (!string)
		return NULL;

	id = atoi(string);

	dlist_fornext_retarded(dl_mod_timer, dptr, dptr_tmp) {
		if (!dptr)
			break;
		t = (modtimer_t *) dlist_data(dptr);
		if (!t)
			continue;

		if (t->id == id) {
			dlist_remove_and_free(&dl_mod_timer, &dptr, timer_free);
		}

	}

	return str;
}

char *timer_op_get(char *string)
{
	char *str = NULL;

	debug(NULL, "timer_op_get: Entered\n");
	if (!string)
		return NULL;
	return str;
}

char *timer_op_clear(char *string)
{
	char *str = NULL;

	debug(NULL, "timer_op_clear: Entered\n");

	if (!string)
		return NULL;

	dlist_fini(&dl_mod_timer, timer_free);

	if (timer_bv)
		bit_vec_fini(&timer_bv);

	return str;
}

char *timer_op_list(char *string)
{
	dlist_t *dptr;
	modtimer_t *t;
	char *str = NULL;
	char buf[MAX_BUF_SZ + 1];

	debug(NULL, "timer_op_list: Entered\n");

	if (!string)
		return NULL;

	_memset(buf, 0, sizeof(buf));

	dlist_fornext(dl_mod_timer, dptr) {
		t = (modtimer_t *) dlist_data(dptr);
		if (!t)
			continue;

		strlcatfmt_buf(buf,
			       "id=%i {interval=%i, count=%i, chan=%s, cmd=%s}\n",
			       t->id, t->interval, t->count, t->chan, t->cmd);
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

modtimer_t *timer_alloc(bot_t * bot, int interval, int count, char *string)
{
	modtimer_t *t;

	if (!bot || !string)
		return NULL;

	t = (modtimer_t *) calloc(1, sizeof(modtimer_t));
	if (!t)
		return NULL;

	t->id = bit_vec_get_and_set(timer_bv);
	printf("xxx t->id=%i\n", t->id);
	t->interval = interval;
	t->count = count;
	time(&t->last_run);
	t->cmd = strdup(string);
	t->chan = strdup(bot->txt_to);
	t->parser = gi->pmod_cur;
	t->dlist_node = gi->pmod_cur_dlist_node;
	t->bot = gi->pmod_cur_bot;

	dlist_Dinsert_after(&dl_mod_timer, t);

	return t;
}

void timer_free(void *arg)
{
	modtimer_t *t = (modtimer_t *) arg;

	if (!t)
		return;

	if (t->cmd)
		free(t->cmd);
	if (t->chan)
		free(t->chan);

	bit_vec_clr(timer_bv, t->id);

	free(t);

	return;
}
