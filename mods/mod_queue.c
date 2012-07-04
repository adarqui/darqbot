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
#include "mod_queue.h"

void
__queue_init__ (void)
{

strlcpy_buf(mod_queue_info.name, "mod_queue");
strlcpy_buf(mod_queue_info.trigger, "^queue");

  module_add_subtrigs (&mod_queue_info, "^enqueue");
  module_add_subtrigs (&mod_queue_info, "^dequeue");

  mod_queue_info.init = queue_init;
  mod_queue_info.fini = queue_fini;
  mod_queue_info.help = queue_help;
  mod_queue_info.run = queue_run;


  mod_queue_info.output = NULL;
  mod_queue_info.input = NULL;


  debug (NULL, "__queue_init__: Loaded mod_queue\n");

  return;
}



bot_t *
queue_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "queue_init: Entered\n");

  swap_inmem_get_assign_and_remove ("dl_mod_queue", 0,
					(void **) &dl_mod_queue_unique);

  return NULL;
}

bot_t *
queue_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "queue_fini: Entered\n");

  swap_inmem_create_and_add ("dl_mod_queue", 0, dl_mod_queue_unique);

  return NULL;
}

bot_t *
queue_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "queue_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^queue || ^queue(enqueue:dequeue:clear:size:list) ...";

  return NULL;
}

bot_t *
queue_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "queue_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "queue_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  if (!strcasecmp (bot->trig_called, "^enqueue"))
    {
      opt = MOD_QUEUE_ENQUEUE;
    }
  else if (!strcasecmp (bot->trig_called, "^dequeue"))
    {
      opt = MOD_QUEUE_DEQUEUE;
    }

  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "clear"))
    {
      opt = MOD_QUEUE_CLEAR;
    }
  else if (!strncasecmp_len (dl_options_ptr, "size"))
    {
      opt = MOD_QUEUE_SIZE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "enqueue"))
    {
      opt = MOD_QUEUE_ENQUEUE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "dequeue"))
    {
      opt = MOD_QUEUE_DEQUEUE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "list"))
    {
      opt = MOD_QUEUE_LIST;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = queue_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
queue_change_string (bot_t * bot, char *string, int opt)
{
  unique_t *bu = NULL;
  dlist_t **dl_mod_queue = NULL;
  dlist_t *dptr;
  char *str = NULL, *data_ptr;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

bz(buf);

  bu =
    unique_create (bot, &dl_mod_queue_unique,
		       UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
  if (!bu)
    return NULL;

  dl_mod_queue = (dlist_t **) & bu->data;

  if (opt == MOD_QUEUE_CLEAR)
    {
      dlist_fini (dl_mod_queue, free);
      return NULL;
    }
  else if (opt == MOD_QUEUE_SIZE)
    {
      strlcatfmt_bot (buf, "%i", dlist_size (*dl_mod_queue));
    }
  else if (opt == MOD_QUEUE_ENQUEUE)
    {
      dlist_Denqueue (dl_mod_queue, strdup (string));
    }
  else if (opt == MOD_QUEUE_DEQUEUE)
    {
      dptr = dlist_dequeue (dl_mod_queue);
      if (!dptr)
	return NULL;
      if (!dlist_data (dptr))
	return NULL;
      return dlist_data (dptr);
    }
  else if (opt == MOD_QUEUE_LIST)
    {
      if (!dl_mod_queue)
	return NULL;
      if (dlist_size (*dl_mod_queue) == 0)
	return NULL;

      strlcat_bot (buf,"[back]\n");
      dlist_fornext (*dl_mod_queue, dptr)
      {
	data_ptr = (char *) dlist_data (dptr);
	if (!data_ptr)
	  continue;
	strlcatfmt_bot (buf, "%s\n", data_ptr);
      }
      strlcat_bot (buf, "[front]\n");
    }

  if (sNULL(buf)!=NULL)
    str = strdup (buf);

  return str;
}
