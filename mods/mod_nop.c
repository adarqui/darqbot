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
#include "mod_nop.h"

void __nop_init__(void)
{

	strlcpy_buf(mod_nop_info.name, "mod_nop");
	strlcpy_buf(mod_nop_info.trigger, "^nop");

	mod_nop_info.init = mod_nop_init;
	mod_nop_info.fini = mod_nop_fini;
	mod_nop_info.help = mod_nop_help;
	mod_nop_info.run = mod_nop_run;
	mod_nop_info.output = mod_nop_output;
	mod_nop_info.input = mod_nop_input;

	mod_nop_in_prob = mod_nop_out_prob = 1;

	debug(NULL, "__nop_init__: Loaded mod_nop\n");

	return;
}

bot_t *mod_nop_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_nop_init: Entered\n");
	return NULL;
}

bot_t *mod_nop_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_nop_fini: Entered\n");

	if (mod_nop_out_str_prepend)
		free(mod_nop_out_str_prepend);
	if (mod_nop_out_str_append)
		free(mod_nop_out_str_append);

	if (mod_nop_in_str_prepend)
		free(mod_nop_in_str_prepend);
	if (mod_nop_in_str_append)
		free(mod_nop_in_str_append);

	if (mod_nop_chan_match)
		free(mod_nop_chan_match);

	return NULL;
}

bot_t *mod_nop_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_nop_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^nop(outp:outa:inp:ina:coutp:couta:cina:cinp:call:match:cmatch:outprob:inprob:prob)";

	return NULL;
}

bot_t *mod_nop_run(dlist_t * dlist_node, bot_t * bot)
{

	char *dl_module_arg_after_options, *dl_options_ptr;

	int opt;

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot, "mod_nop_run: Entered\n");

	debug(bot,
	      "mod_nop_run: Entered: initial hook buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	if (!bot->isprivmsg)
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "outp")) {
		opt = 1;
	} else if (!strncasecmp_len(dl_options_ptr, "outa")) {
		opt = 2;
	}
/*
else if(!strncasecmp_len(str, "inp")) {
opt = 3;
}
*/
	else if (!strncasecmp_len(dl_options_ptr, "ina")) {
		opt = 4;
	} else if (!strncasecmp_len(dl_options_ptr, "match")) {
		opt = 5;
	} else if (!strncasecmp_len(dl_options_ptr, "outprob")) {
		opt = 6;
	} else if (!strncasecmp_len(dl_options_ptr, "inprob")) {
		opt = 7;
	} else if (!strncasecmp_len(dl_options_ptr, "prob")) {
		opt = 8;
	}

	else if (!strncasecmp_len(dl_options_ptr, "coutp")) {
		if (mod_nop_out_str_prepend)
			free(mod_nop_out_str_prepend);
		mod_nop_out_str_prepend = NULL;
	} else if (!strncasecmp_len(dl_options_ptr, "couta")) {
		if (mod_nop_out_str_append)
			free(mod_nop_out_str_append);
		mod_nop_out_str_append = NULL;
	} else if (!strncasecmp_len(dl_options_ptr, "cinp")) {
		if (mod_nop_in_str_prepend)
			free(mod_nop_in_str_prepend);
		mod_nop_in_str_prepend = NULL;
	} else if (!strncasecmp_len(dl_options_ptr, "cina")) {
		if (mod_nop_in_str_append)
			free(mod_nop_in_str_append);
		mod_nop_in_str_append = NULL;
	} else if (!strncasecmp_len(dl_options_ptr, "cmatch")) {
		if (mod_nop_chan_match)
			free(mod_nop_chan_match);
		mod_nop_chan_match = NULL;
	} else if (!strncasecmp_len(dl_options_ptr, "call")) {

		if (mod_nop_out_str_prepend)
			free(mod_nop_out_str_prepend);
		mod_nop_out_str_prepend = NULL;
		if (mod_nop_out_str_append)
			free(mod_nop_out_str_append);
		mod_nop_out_str_append = NULL;
		if (mod_nop_in_str_prepend)
			free(mod_nop_in_str_prepend);
		mod_nop_in_str_prepend = NULL;
		if (mod_nop_in_str_append)
			free(mod_nop_in_str_append);
		mod_nop_in_str_append = NULL;
		if (mod_nop_chan_match)
			free(mod_nop_chan_match);
		mod_nop_chan_match = NULL;

		mod_nop_out_prob = mod_nop_in_prob = 1;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	if (!opt)
		return NULL;

	MOD_PARSE_TOP_HALF;

/* why did i not make OPT_COMMANDS for these numbers? odd */
	if (l_str_ptr) {
		switch (opt) {
		case 1:
			{
				if (mod_nop_out_str_prepend)
					free(mod_nop_out_str_prepend);
				mod_nop_out_str_prepend = strdup(l_str_ptr);
				debug(bot, "mod_nop_run: out_prepend = %s\n",
				      mod_nop_out_str_prepend);
				break;
			}

		case 2:
			{
				if (mod_nop_out_str_append)
					free(mod_nop_out_str_append);
				mod_nop_out_str_append = strdup(l_str_ptr);
				debug(bot, "mod_nop_run: out_append = %s\n",
				      mod_nop_out_str_append);
				break;
			}

		case 3:
			{
				if (mod_nop_in_str_prepend)
					free(mod_nop_in_str_prepend);
				mod_nop_in_str_prepend = strdup(l_str_ptr);
				debug(bot, "mod_nop_run: in_prepend = %s\n",
				      mod_nop_in_str_prepend);
				break;
			}

		case 4:
			{
				if (mod_nop_in_str_append)
					free(mod_nop_in_str_append);
				mod_nop_in_str_append = strdup(l_str_ptr);
				debug(bot, "mod_nop_run: in_append = %s\n",
				      mod_nop_in_str_append);
				break;
			}
		case 5:
			{
/* match channel or pm's */
				if (mod_nop_chan_match)
					free(mod_nop_chan_match);
				mod_nop_chan_match = strdup(l_str_ptr);
				debug(bot, "mod_nop_run: match = %s\n",
				      mod_nop_chan_match);
				break;
			}
		case 6:
			{
				mod_nop_out_prob = atoi(l_str_ptr);
				if (mod_nop_out_prob == 0)
					mod_nop_out_prob = 1;
			}
		case 7:
			{
				mod_nop_in_prob = atoi(l_str_ptr);
				if (mod_nop_in_prob == 0)
					mod_nop_in_prob = 1;
			}
		case 8:
			{
				mod_nop_out_prob = atoi(l_str_ptr);
				if (mod_nop_out_prob == 0)
					mod_nop_out_prob = 1;
				mod_nop_in_prob = mod_nop_out_prob;
			}

		default:
			break;
		}
	}

	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

bot_t *mod_nop_output(dlist_t * dlist_node, bot_t * bot)
{

	if (!dlist_node || !bot)
		return NULL;

	debug(bot, "mod_nop_output: Entered\n");

	if (mod_nop_chan_match) {
		if (!xstrcasestr(mod_nop_chan_match, bot->txt_to))
			return NULL;
	}

	if (mod_nop_out_prob > 1) {
		if ((rand() % mod_nop_out_prob) != 0) {
			return NULL;
		}
	}

	if (mod_nop_out_str_prepend) {
		strprepend_bot(bot->txt_data_in, mod_nop_out_str_prepend);
		str_transform_pipes(bot->txt_data_in);
	}

	if (mod_nop_out_str_append) {
		strlcat_bot(bot->txt_data_in, mod_nop_out_str_append);
		str_transform_pipes(bot->txt_data_in);
	}

	return NULL;
}

bot_t *mod_nop_input(dlist_t * dlist_node, bot_t * bot)
{

	if (!dlist_node || !bot)
		return NULL;

	debug(bot, "mod_nop_input: Entered\n");

	if (mod_nop_chan_match) {
		if (!xstrcasestr(mod_nop_chan_match, bot->txt_to))
			return NULL;
	}

	if (mod_nop_in_prob > 1) {
		if ((rand() % mod_nop_in_prob) != 0) {
			return NULL;
		}
	}

	if (mod_nop_in_str_prepend) {
		strprepend_bot(bot->txt_data_in, mod_nop_in_str_prepend);
		str_transform_pipes(bot->txt_data_in);
	}

	if (mod_nop_in_str_append) {
		strlcat_bot(bot->txt_data_in, mod_nop_in_str_append);
		str_transform_pipes(bot->txt_data_in);
	}

	return NULL;
}
