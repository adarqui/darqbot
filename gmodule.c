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
#include "bot.h"



void
gmodule_sort (void)
{
  xmodule_sort (XMODULE_TYPE_GMODULE);
  return;
}


void
gmodule_update_stats (void)
{
  xmodule_update_stats (XMODULE_TYPE_GMODULE);
  return;
}

void
gmodule_lock (char *name)
{
  xmodule_lock (XMODULE_TYPE_GMODULE, name);
  return;
}

void
gmodule_unlock (char *name)
{
  xmodule_unlock (XMODULE_TYPE_GMODULE, name);
  return;
}

dlist_t * gmodule_find_by_trig_dptr(char * name) {
return xmodule_find_by_trig_dptr(XMODULE_TYPE_GMODULE, name);
}

module_t *
gmodule_find_by_trig (char *name)
{
  return xmodule_find_by_trig (XMODULE_TYPE_GMODULE, name);
}

module_t *
gmodule_find_by_name (char *name)
{
  return xmodule_find_by_name (XMODULE_TYPE_GMODULE, name);
}

void
gmodule_update (module_t * mod_struct, char *name)
{
  xmodule_update (XMODULE_TYPE_GMODULE, mod_struct, name);
  return;
}


char *
gmodule_path (char *name)
{

  return xmodule_path (XMODULE_TYPE_GMODULE, name);
}


module_t *
gmodule_load (char * name)
{
  return xmodule_load (XMODULE_TYPE_GMODULE, name);
}


module_t *
gmodule_unload (module_t *mod)
{
  return xmodule_unload (XMODULE_TYPE_GMODULE, mod,NULL);
}


void
gmodule_iolist (void)
{
  xmodule_iolist (XMODULE_TYPE_GMODULE);
  return;
}


void
gmodule_timerlist (void)
{
  xmodule_timerlist (XMODULE_TYPE_GMODULE);
  return;
}



void
gmodule_list (void)
{
  xmodule_list (XMODULE_TYPE_GMODULE);
  return;
}


void
gmodule_load_all (void)
{

  xmodule_load_all (XMODULE_TYPE_GMODULE);

  return;
}



bot_t *
gmodule_unload_all (void)
{
  return xmodule_unload_all (XMODULE_TYPE_GMODULE);
}


/* console parsing stuff */
void
console_gmodload (char *arg)
{

  console_xmodload (XMODULE_TYPE_GMODULE, arg);

  return;
}


void
console_gmodunload (char *arg)
{

  console_xmodunload (XMODULE_TYPE_GMODULE, arg);
  return;
}

void
console_gmodreload (char *arg)
{

  console_xmodreload (XMODULE_TYPE_GMODULE, arg);

  return;
}










void
bot_gmodule_elm_free (void *data)
{
  bot_gmod_elm_t *gmod;

  if (!data)
    return;

  gmod = (bot_gmod_elm_t *) data;

/*
  if (gmod->syntax)
    free (gmod->syntax);

  if (gmod->trigger)
    free (gmod->trigger);
*/

  free (gmod);

  return;
}





/*
void
bot_gmodule_run_stack_new (bot_t * bot)
{
  if (!bot)
    return;

  debug (bot, "bot_gmodule_run_stack_new: Entered\n");

  bot->dl_gmodules_cur = NULL;

  if (!bot->on)
    {
      debug (bot, "bot_gmodule_run_stack_new: Returning, bot is off\n");
      return;
    }

  bot_gmodule_run_stack (bot);
  return;
}
*/



void
bot_gmodule_run_stack_once (bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;

  bot_gmod_elm_t *gmod = NULL;
  dlist_t *dptr_gmodules=NULL, *dptr_gmodule=NULL;
  module_t *module = NULL;
  char *trigger, *trig_p_ptr, *trig_clean_ptr=NULL;

  int trig_called_proper_len = 0, trig_len = 0;

  if (!bot)
    return;

  debug (bot, "bot_gmodule_run_stack: Entered\n");

  if (bot->dl_gmodules_cur)
    dptr_gmodules = bot->dl_gmodules_cur;
  else
    dptr_gmodules = bot->dl_gmodules;

  gmodules_cur_orig = bot->dl_gmodules_cur;

    gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmodules);
    trigger = gmod->syntax;

      trig_len = 0;
      trig_p_ptr = strchr (trigger, '(');
      if (trig_p_ptr)
        {
          trig_len = trig_p_ptr - trigger;
        }
      else
        trig_len = fn_strlen (trigger);

trig_clean_ptr = strdup_len(trigger, trig_len);
if(!trig_clean_ptr) goto cleanup;


dptr_gmodule = gmodule_find_by_trig_dptr(trig_clean_ptr);

free(trig_clean_ptr);

if(!dptr_gmodule) goto cleanup;

module = (module_t *) dlist_data( dptr_gmodule);
if(!module) goto cleanup;

if(!module->trigger) goto cleanup;

   bot->dl_gmodules_cur = dptr_gmodules;

          trig_called_proper_len = fn_strlen (module->trigger);

          if (module->run)
            {

              bz(bot->trig_called);
              strlcpy_buf (bot->trig_called, trigger);

              bot->dl_module_arg = bot->trig_called + trig_called_proper_len;

              module->run (dptr_gmodules, bot);
            }

bz(module->self->trigger_ext);

cleanup:

  bot->dl_gmodules_cur = gmodules_cur_orig;

  return;
}










void
bot_gmodule_run_stack_new_once (bot_t * bot)
{
  if (!bot)
    return;

  debug (bot, "bot_gmodule_run_stack_new_once: Entered\n");

  bot->dl_gmodules_cur = NULL;

  if (!bot->on)
    {
      debug (bot,
		 "bot_gmodule_run_stack_new_once: Returning, bot is off\n");
      return;
    }

  bot_gmodule_run_stack_once (bot);
  return;
}





void
bot_gmodule_run_control (bot_t * bot, int what, ...)
{
  va_list ap;
  dlist_t *dptr;
  bot_gmod_elm_t *gmod;
  module_t *module;

  debug (bot, "bot_gmodule_run_control: Entered\n");

  if (!bot)
    return;

  if (!bot->dl_gmodules)
    return;

  dptr = bot->dl_gmodules;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    return;

  va_start (ap, what);
  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);

  if (module)
    {
      if (module->control)
	{
	  module->control (dptr, bot, what, ap);
	}
    }

  va_end (ap);

  return;
}







int
gmodule_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;
  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod;
  module_t *module;
  debug (bot, "gmodule_destroy_up: Entered\n");
  if (!bot)
    return -1;
  if (!dlist_node)
    {
      debug (NULL, "gmodule_destroy_up: DLIST_NODE EMPTY\n");
      dptr = bot->dl_gmodules;
      if (!dptr)
	return -1;
      dlist_node = dptr;
    }
  else
    {
      dptr = dlist_next (dlist_node);
      if (!dptr)
	return -1;
    }

  debug (NULL, "gmodule_destroy_up: NEXT EXISTS\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    return -1;

  if (gmod->opt_keepalive)
    {
/* dont pass up */
      debug (NULL,
		 "gmodule_destroy_up: DESTROY_UP: KEEPALIVE SET, RETURNING\n");
      return -1;
    }


  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);

  if (!module)
    return -1;

  if (!module->destroy_up)
    return -1;

  bz(bot->trig_called);
  strlcpy_buf (bot->trig_called, gmod->syntax);


  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);

/*
  module->self->trigger_ext = gmod->trigger_ext;
*/
strlcpy_buf(module->self->trigger_ext, gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  module->destroy_up (dptr, bot);

/*
  module->self->trigger_ext = NULL;
*/
bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;

  return 0;
}




int
gmodule_destroy_down (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;

  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod;
  module_t *module;
  debug (bot, "gmodule_destroy_down: Entered\n");
  if (!bot)
    return -1;


  if (!dlist_node)
    {
      debug (NULL, "gmodule_destroy_down: DLIST_NODE EMPTY\n");
      dptr = dlist_tail (bot->dl_gmodules);
      if (!dptr)
	dptr = bot->dl_gmodules;

      if (!dptr)
	return -1;

      dlist_node = dptr;
    }
  else
    {
      dptr = dlist_prev (dlist_node);
      if (!dptr)
	return -1;
    }

  debug (NULL, "gmodule_destroy_down: PREV EXISTS\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);

  if (!module)
    return -1;

  if (!module->destroy_down)
    return -1;

  bz(bot->trig_called);
  strlcpy_buf (bot->trig_called, gmod->syntax);


  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);

/*
  module->self->trigger_ext = gmod->trigger_ext;
*/
strlcpy_buf(module->self->trigger_ext,gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  module->destroy_down (dptr, bot);

/*
  module->self->trigger_ext = NULL;
*/
bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;
  return 0;
}







int
gmodule_control_up (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;

  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod;
  module_t *module;
  debug (bot, "gmodule_control_up: Entered\n");
  if (!bot)
    return -1;
  if (!dlist_node)
    {
      debug (NULL, "gmodule_control_up: DLIST_NODE EMPTY\n");
      dptr = bot->dl_gmodules;
      if (!dptr)
	return -1;
      dlist_node = dptr;
    }
  else
    {
      dptr = dlist_next (dlist_node);
      if (!dptr)
	return -1;
    }

  debug (NULL, "gmodule_control_up: NEXT EXISTS\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    return -1;

  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);
  if (!module)
    {
      return -1;
    }

  if (!module->control_up)
    {
      return -1;
    }

bz(bot->trig_called);
strlcpy_buf(bot->trig_called,gmod->syntax);

  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);

strlcpy_buf(module->self->trigger_ext,gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  module->control_up (dptr, bot);

bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;

  return 0;
}





int
gmodule_control_down (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;
  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod;
  module_t *module;
  debug (bot, "gmodule_control_down: Entered\n");
  if (!bot)
    return -1;
  if (!dlist_node)
    {
      debug (NULL, "gmodule_control_down: DLIST_NODE EMPTY\n");
      dptr = dlist_tail (bot->dl_gmodules);
      if (!dptr)
	dptr = bot->dl_gmodules;
      if (!dptr)
	return -1;
      dlist_node = dptr;
    }
  else
    {
      dptr = dlist_prev (dlist_node);
      if (!dptr)
	return -1;
    }

  debug (NULL, "gmodule_control_down: PREV EXISTS\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    return -1;

  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);

  if (!module)
    return -1;

  if (!module->control_down)
    return -1;
/*
  memset (bot->trig_called, 0, sizeof (bot->trig_called));
*/
/*
  strlcpy_buf (bot->trig_called, gmod->syntax, sizeof (bot->trig_called) - 1);
*/
strlcpy_buf(bot->trig_called, gmod->syntax);


  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);

/*
  module->self->trigger_ext = gmod->trigger_ext;
*/
strlcpy_buf(module->self->trigger_ext, gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  module->control_down (dptr, bot);

/*
  module->self->trigger_ext = NULL;
*/
bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;

  return 0;
}



int
gmodule_up (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;
  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod, *gmod_self;
  module_t *module;

  debug (bot, "gmodule_up: Entered\n");

  if (!bot)
    return -1;


  if (!dlist_node)
    {
      debug (NULL, "gmodule_up: DLIST_NODE EMPTY\n");
      dptr = bot->dl_gmodules;
      if (!dptr)
	return -1;
      dlist_node = dptr;
    }

  dptr = dlist_next (dlist_node);
  if (!dptr)
    return -1;

  debug (NULL, "gmodule_up: NEXT EXISTS\n");

  gmod_self = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (gmod_self->can_pass_up == 0 && bot->dl_gmodules_force == 0)
    {
      debug (NULL, "gmodule_up: CANT PASS UP\n");
      return -1;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    return -1;

  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);

  if (!module)
    return -1;

strlcpy_buf(bot->trig_called, gmod->syntax);

  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);

  gmodule_fix_data_up (bot);

strlcpy_buf(module->self->trigger_ext, gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  if (!gmod->has_run)
    {
/* the first time this module is being run inside this bot/sub */
/* this 'dptr' is wrong for run, should be a module dlist dptr */
      if (module->run)
	module->run (dptr, bot);
    }
  else
    {
      if (module->input)
	{
	  module->input (dptr, bot);
	}
    }

bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;

  return 0;
}








int
gmodule_down (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *gmodules_cur_orig = NULL;
  dlist_t *dptr = NULL;
  bot_gmod_elm_t *gmod, *gmod_self;
  module_t *module;

  int n = 0;

  debug (bot, "gmodule_down: Entered\n");

  if (!bot)
    return -1;

  if (!dlist_node)
    {
      debug (NULL, "gmodule_down: DLIST_NODE EMPTY\n");
      dptr = bot->dl_gmodules;
      if (!dptr)
	{
	  n = -1;
	  goto cleanup;
	}
      dlist_node = dlist_tail (dptr);
      if (!dlist_node)
	{
	  n = -1;
	  goto cleanup;
	}
    }

  dptr = dlist_prev (dlist_node);
  if (!dptr)
    {
      n = -1;
      goto cleanup;
    }

  debug (NULL, "gmodule_down: PREV EXISTS\n");

  gmod_self = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (gmod_self->can_pass_down == 0 && bot->dl_gmodules_force == 0)
    {
      debug (NULL, "gmodule_down: CANT PASS DOWN\n");
      n = -1;
      goto cleanup;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dptr);

  if (!gmod)
    {
      n = -1;
      goto cleanup;
    }

  module = xmodule_find_by_trig (XMODULE_TYPE_GMODULE, gmod->trigger);
  if (!module)
    {
      n = -1;
      goto cleanup;
    }

  bz(bot->trig_called);
  strlcpy_buf (bot->trig_called, gmod->syntax);


  bot->dl_module_arg = bot->trig_called + fn_strlen (gmod->trigger);


  gmodule_fix_data_down (bot);

/*
  module->self->trigger_ext = gmod->trigger_ext;
*/
strlcpy_buf(module->self->trigger_ext, gmod->trigger_ext);

  gmodules_cur_orig = bot->dl_gmodules_cur;
  bot->dl_gmodules_cur = dptr;

  if (module->output)
    {
      module->output (dptr, bot);
    }


/*
  module->self->trigger_ext = NULL;
*/
bz(module->self->trigger_ext);

  bot->dl_gmodules_cur = gmodules_cur_orig;

/* ADDED .. ?? */
cleanup:

  bot_line_clear_bufs (bot);

  return n;
}





int
gmodule_up_then_down (dlist_t * dlist_node, bot_t * bot)
{

  gmodule_up (dlist_node, bot);
  gmodule_down (dlist_node, bot);

  return 0;
}




void
gmodule_fix_data_up (bot_t * bot)
{
  if (!bot)
    return;

  if (bot->txt_data_out_sz > 0)
    {
      memcopy (bot->txt_data_in, bot->txt_data_out,
	       sizeof (bot->txt_data_in) - 1, bot->txt_data_out_sz);
      bot->txt_data_in_sz = bot->txt_data_out_sz;
      bz(bot->txt_data_out);
      bot->txt_data_out_sz = 0;
    }

  return;
}



void
gmodule_fix_data_down (bot_t * bot)
{
  if (!bot)
    return;

  if (bot->txt_data_in_sz > 0 && bot->txt_data_out_sz == 0)
    {
      memcopy (bot->txt_data_out, bot->txt_data_in,
	       sizeof (bot->txt_data_out) - 1, bot->txt_data_in_sz);
      bot->txt_data_out_sz = bot->txt_data_in_sz;
      bz (bot->txt_data_in);
      bot->txt_data_in_sz = 0;
    }

  return;
}


void
gmodule_fix_data_in_to_out (bot_t * bot)
{
  if (!bot)
    return;

  if (bot->txt_data_in_sz > 0)
    {
      memcopy (bot->txt_data_out, bot->txt_data_in,
	       sizeof (bot->txt_data_out) - 1, bot->txt_data_in_sz);
      bot->txt_data_out_sz = bot->txt_data_in_sz;
      bz (bot->txt_data_in);
      bot->txt_data_in_sz = 0;
    }

  return;
}


void
gmodule_fix_data_out_to_in (bot_t * bot)
{
  if (!bot)
    return;


  if (bot->txt_data_out_sz > 0)
    {
      memcopy (bot->txt_data_in, bot->txt_data_out,
	       sizeof (bot->txt_data_in) - 1, bot->txt_data_out_sz);
      bot->txt_data_in_sz = bot->txt_data_out_sz;
      bz(bot->txt_data_out);
      bot->txt_data_out_sz = 0;
    }

  return;
}



bot_gmod_elm_t *
gmodule_get_gmod (dlist_t * dlist_node)
{
  bot_gmod_elm_t *gmod = NULL;

  if (!dlist_node)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);

  return gmod;
}


bot_gmod_elm_t *
gmodule_get_gmod_next (dlist_t * dlist_node)
{
  bot_gmod_elm_t *gmod = NULL;

  if (!dlist_node)
    return NULL;

  if (!dlist_next (dlist_node))
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_next (dlist_node));

  return gmod;
}


bot_gmod_elm_t *
gmodule_find_gmod (bot_t * bot, char *trigger, char *trigger_ext)
{
  dlist_t *dptr;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_find_gmod: Entered. [%s] [%s]\n", trigger,
	     trigger_ext);

  if (!bot)
    return NULL;

  dlist_fornext (bot->dl_gmodules, dptr)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr);
    if (!gmod)
      continue;
    if (!sNULL(gmod->trigger))
      continue;

    if (sNULL(trigger_ext))
      {
/* match the extension first */
	if (!sNULL(gmod->trigger_ext))
	  continue;
	if (strcasecmp (gmod->trigger_ext, trigger_ext))
	  continue;
/* allow us to find a gmod based entirely on trigger_ext */
	if (!sNULL(trigger))
	  return gmod;
      }

    if (sNULL(trigger))
      {
	if (!strcasecmp (gmod->trigger, trigger))
	  return gmod;
      }
  }


  return NULL;
}



dlist_t *
gmodule_find_gmod_dptr (bot_t * bot, char *trigger, char *trigger_ext)
{
  dlist_t *dptr;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_find_gmod_dptr: Entered. [%s] [%s]\n", trigger,
	     trigger_ext);
  if (!bot)
    return NULL;

  dlist_fornext (bot->dl_gmodules, dptr)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr);
    if (!gmod)
      continue;
    if (!sNULL(gmod->trigger))
      continue;

    if (sNULL(trigger_ext))
      {
/* match the extension first */
	if (!sNULL(gmod->trigger_ext))
	  continue;
	if (strcasecmp (gmod->trigger_ext, trigger_ext))
	  continue;
	if (!trigger)
	  return dptr;
      }

    if (sNULL(trigger))
      {
	if (!strcasecmp (gmod->trigger, trigger))
	  return dptr;
      }
  }

  return NULL;
}








bot_gmod_elm_t *
gmodule_find_latest_gmod (bot_t * bot, char *trigger, char *trigger_ext)
{
  dlist_t *dptr;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_find_latest_gmod: Entered. [%s] [%s]\n", trigger,
	     trigger_ext);

  if (!bot)
    return NULL;

  dlist_forprev (bot->dl_gmodules_cur, dptr)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr);
    if (!gmod)
      continue;
    if (!sNULL(gmod->trigger))
      continue;

    if (trigger_ext)
      {
/* match the extension first */
	if (!sNULL(gmod->trigger_ext))
	  continue;
	if (strcasecmp (gmod->trigger_ext, trigger_ext))
	  continue;
/* allow us to find a gmod based entirely on trigger_ext */
	if (!trigger)
	  return gmod;
      }

    if (sNULL(trigger))
      {
	if (!strcasecmp (gmod->trigger, trigger))
	  return gmod;
      }
  }


  return NULL;
}


dlist_t *
gmodule_find_latest_gmod_dptr (bot_t * bot, char *trigger, char *trigger_ext)
{
  dlist_t *dptr;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_find_latest_gmod_dptr: Entered. [%s] [%s]\n",
	     trigger, trigger_ext);
  if (!bot)
    return NULL;

  dlist_forprev (bot->dl_gmodules_cur, dptr)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr);
    if (!gmod)
      continue;
    if (!sNULL(gmod->trigger))
      continue;

    if (sNULL(trigger_ext))
      {
/* match the extension first */
	if (!sNULL(gmod->trigger_ext))
	  continue;
	if (strcasecmp (gmod->trigger_ext, trigger_ext))
	  continue;
	if (!sNULL(trigger))
	  return dptr;
      }

    if (sNULL(trigger))
      {
	if (!strcasecmp (gmod->trigger, trigger))
	  return dptr;
      }
  }

  return NULL;
}

void
gmodule_gmod_print (bot_gmod_elm_t * gmod)
{
  if (!gmod)
    return;

  debug (NULL,
	     "gmodule_gmod_print: Entered\n\tgmod->index=%i\n\tgmod->bot=%p\n\tgmod->trigger=%s\n\tgmod->syntax=%s\n\tgmod->data=%p\n",
	     gmod->index, gmod->bot, gmod->trigger, gmod->syntax, gmod->data);

  return;
}








dlist_t *
bot_gmodule_dup_bot_gmodules (bot_t * bot)
{
/* duplicate the bot->dl_gmodules list */
  dlist_t *dl = NULL, *dptr;
  bot_gmod_elm_t *gmod, *gmod_new;

  if (!bot)
    return NULL;

  dlist_fornext (bot->dl_gmodules, dptr)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr);

    gmod_new = (bot_gmod_elm_t *) calloc (1, sizeof (bot_gmod_elm_t));
    gmod_new->index = gmod->index;
    gmod_new->bot = NULL;

if(gmod->syntax)
    gmod_new->syntax = strdup (gmod->syntax);

/*
if(gmod->trigger)
    gmod_new->trigger = strdup (gmod->trigger);
*/
if(sNULL(gmod->trigger))
strlcpy_buf(gmod_new->trigger, gmod->trigger);

/*
if(gmod->trigger_ext)
    gmod_new->trigger_ext = strdup (gmod->trigger_ext);
*/

if(sNULL(gmod->trigger_ext))
strlcpy_buf(gmod_new->trigger_ext, gmod->trigger_ext);

    gmod_new->data = NULL;
    gmod_new->data_type = 0;

    gmod_new->has_run = 0;
    gmod_new->can_pass_up = 0;
    gmod_new->can_pass_down = 0;
    memset (&gmod_new->link, 0, sizeof (fd_link_t));

    gmod_new->dl_cache = NULL;

    dlist_Dinsert_after (&dl, gmod_new);
  }

  return dl;
}









void
gmodule_off (bot_t * bot)
{

  dlist_t *dptr = NULL, *dptr_tmp = NULL;
  bot_t *bot_tmp;

  debug (NULL, "gmodule_off: Entered\n");

  if (!bot)
    return;

  if (bot->isforked)
bot_fork_clean_exit(bot);

  dlist_fornext_retarded (bot->dl_subs, dptr, dptr_tmp)
  {
    if (!dptr)
      break;
    bot_tmp = (bot_t *) dlist_data (dptr);

    if (!bot_tmp)
      continue;

    gmodule_off (bot_tmp);
  }

  gmodule_off_bot (bot);

  return;
}



void
gmodule_off_bot (bot_t * bot)
{

  module_t *gmodule = NULL;
  dlist_t *dptr = NULL, *dptr_tail;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_off_bot: Entered\n");

  if (!bot)
    {
      return;
    }

  if (bot->isforked)
bot_fork_clean_exit(bot);

  if (bot->dl_gmodules)
    {

      dptr_tail = dlist_tail (bot->dl_gmodules);
      if (!dptr_tail)
	dptr_tail = bot->dl_gmodules;

      dlist_forprev (dptr_tail, dptr)
      {
	gmod = (bot_gmod_elm_t *) dlist_data (dptr);
	if (!gmod)
	  continue;
	if (!gmod->trigger)
	  continue;
	gmodule = gmodule_find_by_trig (gmod->trigger);

	if (gmodule)
	  {
	    if (gmodule->off)
	      {
		gmodule->off (dptr, bot);
	      }

	  }

      }

dlist_fini(&bot->dl_gmodules, gxmodule_free);

    }

  return;

}


void gxmodule_free(void *arg) {
bot_gmod_elm_t * gmod=(bot_gmod_elm_t *)arg;
if(!arg) return;

if(gmod->syntax)
free(gmod->syntax);

gmod->syntax=NULL;

/*
if(gmod->trigger)
free(gmod->trigger);

gmod->trigger=NULL;

if(gmod->trigger_ext)
free(gmod->trigger_ext);

gmod->trigger_ext = NULL;
*/

free(gmod);

return;
}


void
gmodule_set_has_run (bot_gmod_elm_t * gmod)
{

  debug (NULL, "gmodule_set_has_run: Entered\n");

  if (!gmod)
    return;

  gmod->has_run = 1;

  return;
}


void
gmodule_set_can_pass_up (bot_gmod_elm_t * gmod)
{

  if (!gmod)
    return;

  debug (NULL, "gmodule_set_can_pass_up, %p\n", gmod->data);
  gmod->can_pass_up = 1;

  return;
}


void
gmodule_set_can_pass_down (bot_gmod_elm_t * gmod)
{

  if (!gmod)
    return;

  debug (NULL, "gmodule_set_can_pass_up, %p\n", gmod->data);
  gmod->can_pass_down = 1;

  return;
}





dlist_t *
gmod_cache_enqueue (dlist_t ** dl, int type, char *buf, int len)
{
  dlist_t *dptr;
  bot_gmod_cache_elm_t *gmod_c = NULL;
  if (!dl || !buf || len <= 0)
    return NULL;

  gmod_c = (bot_gmod_cache_elm_t *) calloc (1, sizeof (bot_gmod_cache_elm_t));
  if (!gmod_c)
    return NULL;

  gmod_c->data_type = type;
  gmod_c->data_len = len;
  gmod_c->data = (char *) calloc (1, len + 1);

  if (!gmod_c->data)
    {
      free (gmod_c);
      return NULL;
    }

  memcpy (gmod_c->data, buf, len);

  dptr = dlist_Denqueue (dl, gmod_c);

  return dptr;
}


bot_gmod_cache_elm_t *
gmod_cache_dequeue (dlist_t ** dl)
{
  dlist_t *dptr = NULL;
  bot_gmod_cache_elm_t *gmod_c = NULL;
  if (!dl)
    return NULL;

  dptr = dlist_dequeue (dl);
  if (!dptr)
    return NULL;

  gmod_c = (bot_gmod_cache_elm_t *) dlist_data (dptr);

  free (dptr);

  return gmod_c;
}



void
gmod_cache_free (void *arg)
{
  bot_gmod_cache_elm_t *gmod_c = (bot_gmod_cache_elm_t *) arg;
  if (!gmod_c)
    return;

  free (gmod_c->data);
  free (gmod_c);

  return;
}







void
gmodule_keepalive_on (bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gmodule_keepalive_on: Entered\n");
  if (!bot)
    return;

  dptr_gmod = bot->dl_gmodules_cur;
  if (!dptr_gmod)
    return;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    return;

  gmod->opt_keepalive = 1;
  bot->dl_gmodules_keepalive = 1;

  return;
}



void
gmodule_keepalive_off (bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  int keepalive_set = 0;

  debug (NULL, "gmodule_keepalive_off: Entered\n");
  if (!bot)
    return;

  dptr_gmod = bot->dl_gmodules_cur;
  if (!dptr_gmod)
    return;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    return;

  gmod->opt_keepalive = 0;

  dlist_fornext (bot->dl_gmodules, dptr_gmod)
  {
    gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
    if (gmod->opt_keepalive)
      keepalive_set = 1;
  }

  if (!keepalive_set)
    bot->dl_gmodules_keepalive = 0;

  return;
}




void
gmodule_parse_common_options (bot_t * bot, bot_gmod_elm_t * gmod,
			      char *string)
{


  debug (NULL, "gmodule_parse_common_options: Entered\n");

  if (!bot || !gmod || !string)
    return;

  if (!strncasecmp_len (string, "opt_keepalive"))
    {
      gmod->opt_keepalive = GMOD_KEEPALIVE_ON;
      gmodule_keepalive_on (bot);
    }
  else if (!strncasecmp_len (string, "opt_pass_normal"))
    {
      gmod->opt_opposite = 0;
    }
  else if (!strncasecmp_len (string, "opt_pass_opposite"))
    {
      gmod->opt_opposite = 1;
    }

  return;
}







int
gmodule_up_force (dlist_t * dlist_node, bot_t * bot)
{

  if (!dlist_node || !bot)
    return -1;

  bot->dl_gmodules_force = 1;

  return gmodule_up (dlist_node, bot);
}


int
gmodule_down_force (dlist_t * dlist_node, bot_t * bot)
{

  if (!dlist_node || !bot)
    return -1;

  bot->dl_gmodules_force = 1;

  return gmodule_down (dlist_node, bot);
}
