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
#include "mod_dumbgames.h"

void __dumbgames_init__(void)
{
	strlcpy_buf(mod_dumbgames_info.name, "mod_dumbgames");
	strlcpy_buf(mod_dumbgames_info.trigger, "^dumbgames");

	module_add_subtrigs(&mod_dumbgames_info, "^dice");
	module_add_subtrigs(&mod_dumbgames_info, "^roulette");

	mod_dumbgames_info.init = dumbgames_init;
	mod_dumbgames_info.fini = dumbgames_fini;
	mod_dumbgames_info.help = dumbgames_help;
	mod_dumbgames_info.run = dumbgames_run;

	mod_dumbgames_info.output = NULL;
	mod_dumbgames_info.input = NULL;

	debug(NULL, "__dumbgames_init__: Loaded mod_dumbgames\n");

	return;
}

bot_t *dumbgames_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "dumbgames_init: Entered\n");
	return NULL;
}

bot_t *dumbgames_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "dumbgames_fini: Entered\n");
	return NULL;
}

bot_t *dumbgames_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "dumbgames_help: Entered\n");

	if (!bot)
		return NULL;

	if (str_match(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^dice", NULL)) {
		bot->dl_module_help = "^dice ::: rolls two random dice";
	} else
	    if (str_match
		(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^roulette", NULL))
	{
		bot->dl_module_help =
		    "^roulette(numtriggerpulls) ::: pulls the \"trigger\" numtriggerpulls times and hopefully doesn't return a BANG!";
	} else {
		bot->dl_module_help =
		    "^dumbgames ::: ^dice || ^roulette(numtriggerpulls)";
	}

	return NULL;
}

bot_t *dumbgames_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt, sub;

	debug(bot, "dumbgames_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "dumbgames_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	sub = MOD_DUMBGAMES;

	if (!strcasecmp(bot->trig_called, "^dice"))
		sub = MOD_DUMBGAMES_DICE;
	else if (!strcasecmp(bot->trig_called, "^roulette"))
		sub = MOD_DUMBGAMES_ROULETTE;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	opt = atoi(bot->dl_module_arg + 1);
	if (opt > 6)
		opt = 6;
	else if (opt <= 0)
		opt = 1;

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;

	switch (sub) {
	case MOD_DUMBGAMES_DICE:
		{
			l_new_str = dice_change_string(opt);
			break;
		}
	case MOD_DUMBGAMES_ROULETTE:
		{
			l_new_str = roulette_change_string(opt);
			break;
		}
	default:
		break;
	}

	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *dice_change_string(int opt)
{
	int d1, d2, i;
	char *str = NULL;

	d1 = d2 = i = 0;
	while (1) {
		if (d1 && d2)
			break;
		if (!d1) {
			d1 = rand() % 7;
		}

		if (!d2) {
			d2 = rand() % 7;
		}

	}

	str = str_unite("[%i] [%i]", d1, d2);
	return str;
}

int chamber_compare(const void *a, const void *b)
{
	return rand() % 7;
}

char *roulette_change_string(int opt)
{
	int d2, i;
	char *str = NULL;

	int chamber[6];

	debug(NULL, "roulette_change_string: opt=%i\n", opt);

	chamber[0] = 1;
	chamber[1] = 2;
	chamber[2] = 3;
	chamber[3] = 4;
	chamber[4] = 5;
	chamber[5] = 6;

	qsort(chamber, sizeof(chamber) / sizeof(int), sizeof(int),
	      chamber_compare);

	while (1) {
		d2 = rand() % 7;
		if (d2 != 0)
			break;
	}

	for (i = 0; i < opt; i++) {

		if (chamber[i] == d2) {
			str = str_unite("BANG!");
			break;
		}
		str = str_unite("CLICK.");
	}

	return str;
}
