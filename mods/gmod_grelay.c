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
#include "gmod_grelay.h"

void __grelay_init__(void)
{

/*
  gmod_grelay_info.name = "gmod_grelay";
  gmod_grelay_info.trigger = strdup ("^relay");
*/
	strlcpy_buf(gmod_grelay_info.name, "gmod_grelay");
	strlcpy_buf(gmod_grelay_info.trigger, "^grelay");

	gmod_grelay_info.init = grelay_init;
	gmod_grelay_info.fini = grelay_fini;
	gmod_grelay_info.help = grelay_help;
	gmod_grelay_info.run = grelay_run;
	gmod_grelay_info.control = NULL;

	gmod_grelay_info.output = grelay_output;
	gmod_grelay_info.input = grelay_input;

	gmod_grelay_info.off = grelay_off;

	gmod_grelay_info.control_up = grelay_control_up;
	gmod_grelay_info.control_down = grelay_control_down;

	gmod_grelay_info.destroy_up = grelay_destroy_up;
	gmod_grelay_info.destroy_down = grelay_destroy_down;

	gmod_grelay_info.self = &gmod_grelay_info;

	debug(NULL, "__grelay_init__: Loaded gmod_grelay\n");

	return;
}

bot_t *grelay_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "grelay_init: Entered\n");
	return NULL;
}

bot_t *grelay_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "grelay_fini: Entered\n");
	return NULL;
}

bot_t *grelay_off(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "grelay_off: Entered\n");
	return NULL;
}

bot_t *grelay_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "grelay_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^grelay";

	return NULL;
}

/*
bot_t *
grelay_control (dlist_t * dlist_node, bot_t * bot, int what, va_list ap)
{

  debug (NULL, "grelay_control: Entered\n");

  if (!bot)
    return NULL;

  return NULL;
}
*/

bot_t *grelay_run(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
/*
  dlist_t *dl, *dptr;
  module_t *module;
  char *str_ptr, *dl_module_arg_after_options, *dl_options_ptr, *new_str;
  int opt, type;

  char *tok_1, *tok_2, *tok_3;

  debug (bot, "grelay_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "grelay_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

  if (bot_shouldreturn (bot))
    return NULL;

gmod = gmodule_find_gmod(bot, gmod_grelay_info.trigger, gmod_grelay_info.trigger_ext);
if(!gmod) {
puts("GMOD_GRELAY RUN GMOD=NULL");
return NULL;
}

  opt = 0;
  new_str = NULL;

  dl = dptr = NULL;

  tok_1 = tok_2 = tok_3 = NULL;

  printf ("dl_module_arg=%s\n", bot->dl_module_arg);

  MOD_OPTIONS_TOP_HALF;
  printf ("relay trigger: [%s]\n", dl_options_ptr);
  MOD_OPTIONS_BOTTOM_HALF;

  tok_1 = strtok (dl_options_ptr, ",");
  if (!tok_1)
    return NULL;

  tok_2 = strtok (NULL, "");
  if (!tok_1)
    return NULL;

  str_ptr = strchr (tok_2, '(');
  if (str_ptr)
    {
      tok_3 = strdup_len (tok_2, (str_ptr - tok_2));
    }
  else
    {
      tok_3 = strdup (tok_2);
    }

  if (!strcasecmp (tok_1, "p"))
    {
      dl = gi->dl_pmodule;
      type = XMODULE_TYPE_PMODULE;
    }
  else if (!strcasecmp (tok_1, "g"))
    {
      dl = gi->dl_gmodule;
      type = XMODULE_TYPE_GMODULE;
    }
  else if (!strcasecmp (tok_1, "m"))
    {
      dl = gi->dl_module;
      type = XMODULE_TYPE_MODULE;
    }
  else
    return NULL;

  module = xmodule_find_by_trig (type, tok_3);
  if (!module)
    goto cleanup;

  if (!module->run)
    goto cleanup;

  memset (bot->trig_called, 0, sizeof (bot->trig_called));
  strlcpy_buf (bot->trig_called, tok_2, sizeof (bot->trig_called) - 1);

  bot->dl_module_arg = bot->trig_called + fn_strlen (tok_3);

  bot->isprivmsg = 1;

  module->run (dlist_node, bot);

cleanup:
  if (tok_3)
    free (tok_3);

*/

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	grelay_input(dlist_node, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *grelay_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	return str;
}

bot_t *grelay_output(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "grelay_output: Entered\n");

	gmodule_down(dlist_node, bot);

	return NULL;
}

bot_t *grelay_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	dlist_t *dl, *dptr;
	module_t *module;
	char *str_ptr, *dl_module_arg_after_options, *new_str;
	int opt, type;

	char *tok_1, *tok_2, *tok_3;

	debug(bot, "grelay_input: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "grelay_input: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gmodule_set_has_run(gmod);

	opt = 0;
	new_str = NULL;

	dl = dptr = NULL;

	tok_1 = tok_2 = tok_3 = NULL;

	printf("dl_module_arg=%s\n", bot->dl_module_arg);

	if (bot->txt_data_in_sz > 0) {

		dl_module_arg_after_options = bot->dl_module_arg;
		if (bot->dl_module_arg[0] == '(') {
			int len_options_area = 0;

			dl_module_arg_after_options =
			    tokenize_find_closing_bracket(bot->dl_module_arg,
							  '(');
			if (!dl_module_arg_after_options)
				return NULL;

			*dl_module_arg_after_options = '\0';
			dl_module_arg_after_options++;

			len_options_area =
			    dl_module_arg_after_options - bot->dl_module_arg;

			printf("relay trigger: [%s]\n", &bot->dl_module_arg[1]);
		}

		tok_1 = strtok(&bot->dl_module_arg[1], ",");
		if (!tok_1)
			return NULL;

		tok_2 = strtok(NULL, "");
		if (!tok_2)
			return NULL;

		tok_2 = strdup(tok_2);

		str_ptr = strchr(tok_2, '(');

		if (str_ptr) {
			tok_3 = strdup_len(tok_2, (str_ptr - tok_2));
		} else {
			tok_3 = strdup(tok_2);
		}

		if (!strcasecmp(tok_1, "p")) {
			dl = gi->dl_pmodule;
			type = XMODULE_TYPE_PMODULE;
		} else if (!strcasecmp(tok_1, "g")) {
			dl = gi->dl_gmodule;
			type = XMODULE_TYPE_GMODULE;
		} else if (!strcasecmp(tok_1, "m")) {
			dl = gi->dl_module;
			type = XMODULE_TYPE_MODULE;
		} else
			return NULL;

		puts("WTF1");

		module = xmodule_find_by_trig(type, tok_3);
		if (!module)
			goto cleanup;

		puts("WTF2");

		if (!module->run)
			goto cleanup;

/*
      memset (&bot->trig_called, 0, sizeof (bot->trig_called));
*/
		bz(bot->trig_called);
		strlcpy_buf(bot->trig_called, tok_2);
		bot->dl_module_arg = bot->trig_called + fn_strlen(tok_3);
		printf("trig_called=%s, tok_2=%s, tok_3=%s\n", bot->trig_called,
		       tok_2, tok_3);

		bot->isprivmsg = 1;

		if (type == XMODULE_TYPE_MODULE) {
			gmodule_fix_data_in_to_out(bot);
		}

		module->run(dlist_node, bot);

		bot->txt_data_out_sz = strlen(bot->txt_data_out);

	}

	gmodule_up(dlist_node, bot);

	if (bot->txt_data_out_sz && bot->shouldsend) {
		gmodule_down(dlist_node, bot);
/* ...
bot_line_clear(bot);
*/
	}

 cleanup:
	puts("WHYYYYYYYYYYYYYYYYYYYYYYYYY");

	if (tok_3)
		free(tok_3);

	if (tok_2)
		free(tok_2);

	return bot;
}

bot_t *grelay_destroy_gmod(dlist_t * dlist_node, bot_t * bot)
{

	debug(NULL, "grelay_destroy_gmod: Entered\n");

	return NULL;
}

bot_t *grelay_destroy_down(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "grelay_destroy_down: Entered\n");

	ret = grelay_destroy_gmod(dlist_node, bot);

	gmodule_destroy_down(dlist_node, bot);

	return ret;
}

bot_t *grelay_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "grelay_destroy_up: Entere\n");

	gmodule_destroy_up(dlist_node, bot);

	ret = grelay_destroy_gmod(dlist_node, bot);

	return ret;
}

bot_t *grelay_control_down(dlist_t * dlist_node, bot_t * bot)
{

	debug(NULL, "grelay_control_down: Entered\n");

	return NULL;
}

bot_t *grelay_control_up(dlist_t * dlist_node, bot_t * bot)
{

	debug(NULL, "grelay_control_up: Entered\n");

	return NULL;
}
