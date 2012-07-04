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
#include "gmod_gpty.h"


void __gpty_init__ (void) __attribute__ ((constructor));

void
__gpty_init__ (void)
{

strlcpy_buf(gmod_gpty_info.name, "gmod_gpty");
strlcpy_buf(gmod_gpty_info.trigger, "^gpty");

  gmod_gpty_info.init = gpty_init;
  gmod_gpty_info.fini = gpty_fini;
  gmod_gpty_info.help = gpty_help;
  gmod_gpty_info.run = gpty_run;

  gmod_gpty_info.off = gpty_off;
  gmod_gpty_info.destroy_up = gpty_destroy_up;
  gmod_gpty_info.destroy_down = gpty_destroy_down;

  gmod_gpty_info.control_up = gpty_control_up;
  gmod_gpty_info.control_down = gpty_control_down;

  gmod_gpty_info.output = gpty_output;
  gmod_gpty_info.input = gpty_input;

  gmod_gpty_info.self = &gmod_gpty_info;

  debug (NULL, "__gpty_init__: Loaded gmod_gpty\n");

  return;
}



bot_t *
gpty_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gpty_init: Entered\n");
  return NULL;
}

bot_t *
gpty_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gpty_fini: Entered\n");
  return NULL;
}


bot_t *
gpty_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  debug (bot, "gpty_off: Entered\n");


  if (!dlist_node || !bot)
    {
      return NULL;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    {
      return NULL;
    }


  gpty = (gpty_t *) gmod->data;
  if (!gpty)
    {
      return NULL;
    }

  free (gpty);

  return NULL;
}





bot_t *
gpty_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gpty_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^gpty";

  return NULL;
}


bot_t *
gpty_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "gpty_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "gpty_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_gpty_info.trigger,
					gmod_gpty_info.trigger_ext);
  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      debug (NULL,"gpty_run: GPTY GMOD=NULL\n");
      return NULL;
    }

  if (!gmod->data)
    {
      debug (NULL, "gpty_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
      gpty_gmod_init (bot, gmod, dptr_gmod);
      gpty = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  gpty_process_options (gpty, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  gmodule_set_can_pass_up (gmod);
  gmodule_set_can_pass_down (gmod);

  gpty_input (dptr_gmod, bot);

  gmodule_set_has_run (gmod);

  return bot;
}



char *
gpty_process_options (gpty_t * gpty, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "gpty_process_options: Entered\n");


  if (!gpty || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  dl =
    tokenize (NULL, string,
		  TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "...");
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    gpty_process_options_parse (gpty, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
gpty_process_options_parse (gpty_t * gpty, char *string)
{

  if (!gpty || !string)
    return;

  debug (NULL, "gpty_process_options_parse: Entered\n");

  if (!strcasecmp (string, "null"))
    {
      gpty_process_options_parse_null (gpty, &string[strlen ("null=")]);
    }

  gmodule_parse_common_options (gpty->bot, gpty->gmod, string);

  return;
}




void
gpty_process_options_parse_null (gpty_t * gpty, char *string)
{

  if (!gpty || !string)
    return;

  return;
}



char *
gpty_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char *sep_ptr;

  if (!bot || !string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  return str;
}






bot_t *
gpty_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  debug (bot, "gpty_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gpty = (gpty_t *) gmod->data;
  if (!gpty)
    {
      debug (NULL, "gpty_output: gpty=NULL\n");
      return NULL;
    }

  debug (NULL, "gpty_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	     bot->txt_data_out);

  gmodule_down (dlist_node, bot);

  return bot;
}


bot_t *
gpty_input (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  debug (bot, "gpty_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gpty = (gpty_t *) gmod->data;
  if (!gpty)
    {
      debug (NULL, "GPTY_INPUT: gpty=NULL\n");
      return NULL;
    }

  gmodule_fix_data_down (bot);
  gmodule_up (dlist_node, bot);

  debug (NULL, "gpty_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	     bot->txt_data_out);

  return bot;
}




bot_t *
gpty_destroy_up_gpty (gpty_t * gpty)
{
  bot_t *ret = NULL;

  debug (NULL, "gpty_destroy_up_gpty: Entered\n");

  if (gpty)
    {
      ret = gpty_destroy_up (gpty->dptr_gmod, gpty->bot);
    }

  return ret;
}


bot_t *
gpty_destroy_down_gpty (gpty_t * gpty)
{
  bot_t *ret = NULL;

  debug (NULL, "gpty_destroy_down_gpty: Entered\n");

  if (gpty)
    {
      ret = gpty_destroy_down (gpty->dptr_gmod, gpty->bot);
    }

  return ret;
}




bot_t *
gpty_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  debug (NULL, "gpty_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gpty = gmod->data;
	  free (gpty);
	}
    }

  return bot;
}

bot_t *
gpty_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  gpty_t *gpty = NULL;

  debug (NULL, "gpty_destroy_up: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gpty = gmod->data;
	  free (gpty);
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
gpty_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  gpty_t *gpty = NULL;
  debug (NULL, "gpty_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  gpty = (gpty_t *) calloc (1, sizeof (gpty_t));

  if (!gpty)
    return;

  gmod->data = gpty;

  gpty->bot = bot;
  gpty->dptr_gmod = dptr_gmod;
  gpty->gmod = gmod;

  return;
}








bot_t *
gpty_control_up (dlist_t * dlist_node, bot_t * bot)
{
  gpty_t *gpty = NULL;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gpty_control_up: Entered\n");

  if (!dlist_node || !bot)
    {
      return NULL;
    }


  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  gpty = (gpty_t *) gmod->data;

  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
gpty_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "gpty_control_down: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  gmodule_control_down (dlist_node, bot);

  return bot;
}



void
gpty_free (void *arg)
{
  gpty_t *gpty = (gpty_t *) arg;

  if (!gpty)
    return;

/* ... */

  if (!gpty->gmod)
    return;

  gpty->gmod->data = NULL;
  free (gpty);

  return;
}
