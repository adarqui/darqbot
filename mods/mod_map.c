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
#include "mod_map.h"

void
__map_init__ (void)
{

strlcpy_buf(mod_map_info.name, "mod_map");
strlcpy_buf(mod_map_info.trigger, "^map");

  mod_map_info.init = map_init;
  mod_map_info.fini = map_fini;
  mod_map_info.help = map_help;
  mod_map_info.run = map_run;


  mod_map_info.output = NULL;
  mod_map_info.input = NULL;


  debug (NULL, "__map_init__: Loaded mod_map\n");

  return;
}



bot_t *
map_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "map_init: Entered\n");
  return NULL;
}

bot_t *
map_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "map_fini: Entered\n");
  return NULL;
}

bot_t *
map_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "map_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^map || ^map(normal,stuff) ...";

  return NULL;
}

bot_t *
map_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  char *tok_1, *tok_2, *tok_3, *tok_4, *opt_str = NULL;

  debug (bot, "map_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "map_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = MAP_OPT_NORMAL;

  MOD_OPTIONS_TOP_HALF;

  tok_1 = strtok (dl_options_ptr, ",");
  if (!tok_1)
    return NULL;
  tok_2 = strtok (NULL, ",");
  if (!tok_2)
    return NULL;
  tok_3 = strtok (NULL, ",");
  if (!tok_3)
    return NULL;
  tok_4 = strtok (NULL, "");
  if (!tok_4)
    return NULL;

  if (!strcasecmp (tok_2, "PIPE"))
    tok_2 = "|";

  if (!strcasecmp (tok_3, "PIPE"))
    tok_3 = "|";

  opt_str = eat_whitespace (tok_2);

  MOD_OPTIONS_BOTTOM_HALF;

  if (!opt_str)
    return NULL;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str =
    map_change_string (dlist_node, bot, l_str_ptr, opt, tok_2, tok_3,
		       eat_whitespace (tok_4));
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
map_change_string (dlist_t * dlist_node, bot_t * bot, char *string, int opt,
		   char *opt_sep, char *opt_append, char *opt_str)
{
  dlist_t *dl, *dptr;
  char *str = NULL, *tok_ptr;
  char buf[MAX_BUF_SZ];
  char *sep_ptr;

  if (!string || !opt_str || !opt_sep || !opt_append)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;


  dl =
    tokenize (bot, string,
		  TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES |
		  TOKENIZE_LEAVESEP, opt_sep);
  if (!dl)
    return NULL;

  memset (buf, 0, sizeof (buf));

  dlist_fornext (dl, dptr)
  {
    tok_ptr = (char *) dlist_data (dptr);
    if (!tok_ptr)
      continue;

    strlcatfmt_buf (buf, "%s %s%s", opt_str, tok_ptr,
		    opt_append);
  }

  tokenize_destroy (bot, &dl);

  if (buf[0] != '\0')
    str = strdup (buf);

  return str;
}
