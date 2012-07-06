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



int
xmodule_sort_compare_forward (const void *v1, const void *v2)
{
  module_t *mod_ptr_1 = NULL, *mod_ptr_2 = NULL;
  int x;

  if (!v1 || !v2)
    return -1;

  mod_ptr_1 = *(module_t * const *) v1;
  mod_ptr_2 = *(module_t * const *) v2;

  if (!mod_ptr_1 || !mod_ptr_2)
    return -1;

  if (mod_ptr_1->tmp_stat_count > mod_ptr_2->tmp_stat_count)
    x = -1;
  else if (mod_ptr_1->tmp_stat_count < mod_ptr_2->tmp_stat_count)
    x = 1;
  else
    x = 0;

  return x;
}


void
xmodule_sort (int type)
{

  dlist_t *dptr;
  dlist_t **dl_pointer = NULL;

  module_t **mod_array = NULL;
  int i;

  debug (NULL, "xmodule_update_stats: Entered\n");



  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;
  if (!(*dl_pointer))
    return;

  if ((*dl_pointer)->type == DLIST_TYPE_AVL)
    return;

  mod_array = (module_t **) dlist_convert_dlist_to_array (*dl_pointer);
  if (!mod_array)
    return;

  for (i = 0; mod_array[i] != NULL; i++);

  qsort (mod_array, i, sizeof (module_t *), xmodule_sort_compare_forward);

  i = 0;
  dlist_fornext (*(dl_pointer), dptr)
  {
    if (!mod_array[i])
      break;
    dptr->data = mod_array[i];
    i++;
  }

  free (mod_array);

  return;
}


void
xmodule_update_stats (int type)
{

  dlist_t **dl_pointer = NULL;

  debug (NULL, "xmodule_update_stats: Entered\n");

  if (gi->var_xmod_style != XMODULE_STYLE_LIST)
    return;

  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;
  if (!(*dl_pointer))
    return;

  dlist_traverse (dl_pointer, xmodule_list_traverse, NULL);

  return;
}




void *
xmodule_sort_traverse (void *pa, void *pb)
{
  dlist_t *dptr_mod = NULL;
  module_t *module = NULL;

  if (!pa || !pb)
    return NULL;

  dptr_mod = (dlist_t *) pb;
  module = (module_t *) dlist_data (dptr_mod);
  if (!module)
    return NULL;

  if (!sNULL (module->trigger))
    return NULL;

  module->tmp_stat_count = stat_retcnt (NULL, module->trigger);
  if (module->tmp_stat_count < 0)
    module->tmp_stat_count = 0;

  return NULL;
}



char *
xmodule_path (int type, char *name)
{
  char *str;
  char c;

  switch (type)
    {
    case XMODULE_TYPE_MODULE:
      c = '/';
      break;
    case XMODULE_TYPE_PMODULE:
      c = 'p';
      break;
    case XMODULE_TYPE_GMODULE:
      c = 'g';
      break;
    default:
      return NULL;
    }

  str = str_unite_static ("%s/%cmods/%s.so", gi->confdir, c, name);
  return str;
}



module_t *
xmodule_load_reload (int type, module_t * module)
{

  debug (NULL,
	 "xmodule_load_reload: Entered: name=%s, trigger=%s, filename=%s\n",
	 module->name, module->trigger, module->filename);

  if (!type || !module)
    return NULL;


  if (!xmodule_load_dlso (module))
    {
      goto cleanup;
    }


  module->active = 1;

  if (module->timer != NULL)
    {
      xmodule_add_timer (module);
    }

  if (module->input || module->output)
    {
      xmodule_add_iohook (module);
    }

  xmodule_load_subtrigs (module);

cleanup:

  return NULL;
}


module_t *
xmodule_load (int type, char *name)
{
  dlist_t **dl_pointer = NULL;
  module_t *module = NULL;

  debug (NULL, "xmodule_load: Entered: %s\n", name);

  if (!sNULL (name))
    return NULL;

  dl_pointer = xmodule_type_to_dl (type);

  if (dl_pointer == NULL)
    return NULL;

  module = (module_t *) xmodule_find_by_name (type, name);

  if (module)
    {

      if (module->locked)
	return module;

      switch (module->active)
	{
	case 1:
	  {
	    return NULL;
	  }
	case 0:
	  {
	    module = xmodule_load_reload (type, module);
	    break;
	  }
	default:
	  break;
	}
    }

  else
    {
      module = xmodule_load_new (type, name);
    }

  return module;
}


module_t *
xmodule_load_new (int type, char *name)
{
  dlist_t **dl_pointer = NULL;
  module_t *module = NULL;

  debug (NULL, "xmodule_load_new: Entered\n");

  if (!type || !name)
    return NULL;

  module = (module_t *) calloc (1, sizeof (module_t) + 1);
  if (!module)
    return NULL;

  module->active = 1;
  module->type = type;

  dl_pointer = xmodule_type_to_dl (type);

  strlcpy_buf (module->filename, xmodule_path (type, name));

  strlcpy_buf (module->name, name);

  if (!xmodule_load_dlso (module))
    {
      goto cleanup;
    }

  module->dl_pointer = dl_pointer;

  if (!(module->type & XMODULE_TYPE_MIRROR))
    {
      if (module->timer != NULL)
	{
	  xmodule_add_timer (module);
	}

      if (module->input || module->output)
	{
	  xmodule_add_iohook (module);
	}

      xmodule_load_subtrigs (module);
    }

  return module;


cleanup:

  if (module)
    {
      xmodule_free_destroy (module);
    }

  return NULL;
}






module_t *
xmodule_load_dlso (module_t * module)
{
  module_t *module_sym = NULL;
  char *str = NULL;
  void *h = NULL;

  if (!module)
    return NULL;

  h = dlopen (module->filename, RTLD_LAZY | RTLD_GLOBAL);

  debug (NULL, "module_load: h=%p, filename=%s, error=%s\n", h,
	 module->filename, dlerror ());

  if (!h)
    goto cleanup;

  str = str_unite_static ("%s_info", module->name);
  module_sym = dlsym (h, str);

  debug (NULL, "module_load: module_sym=%p, sym_name=[%s]\n", module_sym,
	 str);

  if (!module_sym)
    {
      goto cleanup;
    }

  strlcpy_buf (module->trigger, module_sym->trigger);

  if (!(module->type & XMODULE_TYPE_MIRROR))
    {
/*
  for (i = 0; i < MODULE_SUBTRIG_MAX; i++)
    {
      if (sNULL(module_sym->subtrigs[i]))
{
strlcpy_buf(module->subtrigs[i], module_sym->subtrigs[i]);
}
      else
        break;
    }
*/
      module->dl_subtrigs = module_sym->dl_subtrigs;
    }

  module->init = module_sym->init;
  module->fini = module_sym->fini;
  module->help = module_sym->help;
  module->run = module_sym->run;

  module->control = module_sym->control;
  module->stat = module_sym->stat;
  module->timer = module_sym->timer;
  module->output = module_sym->output;
  module->input = module_sym->input;
  module->off = module_sym->off;
  module->on = module_sym->on;

  module->destroy_up = module_sym->destroy_up;
  module->destroy_down = module_sym->destroy_down;

  module->control_up = module_sym->control_up;
  module->control_down = module_sym->control_down;

  module->self = module_sym;

  module->dl_handle = h;

  module->init (NULL, NULL);

/*
module->dl_pointer = dl_pointer;
*/

/*
if(module->timer != NULL) {
xmodule_add_timer(module);
}

if(module->input || module->output ) {
xmodule_add_iohook(module);
}
*/

  return module;

cleanup:
  if (h)
    dlclose (h);


  return NULL;
}



module_t *
xmodule_unload (int type, module_t * module, char *name)
{
  module_t *ret = NULL;
  dlist_t *dptr_mod = NULL, *dptr_subtrig = NULL, *dptr_subtrig_tmp = NULL;
  char *subtrig_ptr = NULL;

  debug (NULL, "xmodule_unload: Entered\n");

  if (module)
    {
    }
  else if (name)
    {
      dptr_mod = xmodule_find_by_trig_dptr (type, name);
      if (!dptr_mod)
	return NULL;
      module = (module_t *) dlist_data (dptr_mod);
      if (!module)
	return NULL;
    }

  if (!module)
    return NULL;

  if (module->locked)
    return module;

  debug (NULL, "module_unload: unloading %s\n", module->name);


  if (module->type & XMODULE_TYPE_MIRROR)
    {
      xmodule_zero (module);
      return module;
    }


  if (module->fini)
    {
      module->fini (NULL, NULL);
    }

/*
  for (i = 0; sNULL(module->subtrigs[i]) != NULL; i++)
    {
ret = xmodule_unload(type, NULL, module->subtrigs[i]);
if(ret) {
xmodule_zero(ret);
}
    }
*/
  dlist_fornext_retarded (module->dl_subtrigs, dptr_subtrig, dptr_subtrig_tmp)
  {
    if (!dptr_subtrig)
      break;
    subtrig_ptr = (char *) dlist_data (dptr_subtrig);
    if (!subtrig_ptr)
      continue;
    ret = xmodule_unload (type, NULL, subtrig_ptr);
    if (ret)
      {
	xmodule_zero (ret);
      }
  }


  if (module->dl_handle)
    {
      dlclose (module->dl_handle);
    }

  module->dl_handle = NULL;

/* get rid of timers/iohooks */
  xmodule_del_iohook (module, module->trigger);
  xmodule_del_timer (module, module->trigger);

  xmodule_del_subtrigs (module);

  xmodule_zero (module);

  dlist_fini (&module->dl_subtrigs, free);

  return module;
}





void
xmodule_iolist (int type)
{
  dlist_t *dl_text = NULL;
  void *v_dl[2];
  int i = 0;

  dlist_t **dl_pointer;


  debug (NULL, "xmodule_iolist: Entered\n");

  dl_pointer = xmodule_typeio_to_dl (type);

  if (!dl_pointer)
    return;

  if (!(*dl_pointer))
    {
      return;
    }

  i = 0;

  v_dl[0] = &dl_text;
  v_dl[1] = &i;

  dlist_traverse (dl_pointer, xmodule_iolist_traverse, &v_dl);

  if (dl_text)
    {
      dl_str_print (&dl_text);
    }

  dl_str_destroy (&dl_text);

  return;
}




void *
xmodule_iolist_traverse (void *pa, void *pb)
{
  dlist_t **dl_text = NULL, *dptr_mod = NULL;
  module_t *module = NULL;
  void **v_dl = NULL;
  int *i = 0;

  if (!pa || !pb)
    return NULL;

  v_dl = (void **) pa;

  dptr_mod = (dlist_t *) pb;
  if (!dptr_mod)
    return NULL;

  module = (module_t *) dlist_data (dptr_mod);

  if (!module)
    return NULL;
  if (!sNULL (module->trigger))
    return NULL;

  dl_text = (dlist_t **) v_dl[0];
  i = (int *) v_dl[1];

  *i = *i + 1;


  dl_str_unite (dl_text,
		"xmodule_iolist: %i. name=%s, trigger=%s, active=%i, locked=%i, input=%p, output=%p\n",
		*i, module->name, module->trigger, module->active,
		module->locked, module->input, module->output);


  return NULL;
}






void
xmodule_timerlist (int type)
{
  dlist_t *dl_text = NULL;
  void *v_dl[2];
  int i = 0;

  dlist_t **dl_pointer;


  debug (NULL, "xmodule_timerlist: Entered\n");

  dl_pointer = xmodule_typetimer_to_dl (type);

  if (!dl_pointer)
    return;

  if (!(*dl_pointer))
    {
      return;
    }

  i = 0;

  v_dl[0] = &dl_text;
  v_dl[1] = &i;

  dlist_traverse (dl_pointer, xmodule_timerlist_traverse, &v_dl);

  if (dl_text)
    {
      dl_str_print (&dl_text);
    }

  dl_str_destroy (&dl_text);

  return;
}








void *
xmodule_timerlist_traverse (void *pa, void *pb)
{
  dlist_t **dl_text = NULL, *dptr_mod = NULL;
  module_t *module = NULL;
  void **v_dl = NULL;
  int *i = 0;

  if (!pa || !pb)
    return NULL;

  v_dl = (void **) pa;

  dptr_mod = (dlist_t *) pb;
  if (!dptr_mod)
    return NULL;

  module = (module_t *) dlist_data (dptr_mod);

  if (!module)
    return NULL;
  if (!sNULL (module->trigger))
    return NULL;

  dl_text = (dlist_t **) v_dl[0];
  i = (int *) v_dl[1];

  *i = *i + 1;


  dl_str_unite (dl_text,
		"xmodule_timerlist: %i. name=%s, trigger=%s, active=%i, locked=%i, timer=%p\n",
		*i, module->name, module->trigger, module->active,
		module->locked, module->timer);

  return NULL;
}






void
xmodule_list (int type)
{
  dlist_t *dl_text_run = NULL, *dl_text_io_input = NULL, *dl_text_io_output =
    NULL, *dl_text_timer = NULL;
  void *v_dl[5];
  int i = 0;

  dlist_t **dl_pointer;


  debug (NULL, "module_list: Entered\n");

  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;

  if (!(*dl_pointer))
    {
      return;
    }

  i = 0;

  v_dl[0] = &dl_text_run;
  v_dl[1] = &dl_text_io_input;
  v_dl[2] = &dl_text_io_output;
  v_dl[3] = &dl_text_timer;
  v_dl[4] = &i;

  dlist_traverse (dl_pointer, xmodule_list_traverse, &v_dl);

  if (dl_text_run)
    {
      dl_str_print (&dl_text_run);
    }

  if (dl_text_io_input)
    {
      dl_str_print (&dl_text_io_input);
    }

  if (dl_text_io_output)
    {
      dl_str_print (&dl_text_io_output);
    }

  if (dl_text_timer)
    {
      dl_str_print (&dl_text_timer);
    }

  dl_str_destroy (&dl_text_run);
  dl_str_destroy (&dl_text_io_input);
  dl_str_destroy (&dl_text_io_output);
  dl_str_destroy (&dl_text_timer);

  return;
}




void *
xmodule_list_traverse (void *pa, void *pb)
{
  dlist_t **dl_text_run = NULL, **dl_text_io_input =
    NULL, **dl_text_io_output = NULL, **dl_text_timer = NULL, *dptr_mod =
    NULL;
  module_t *module = NULL;
  void *v = NULL, **v_dl = NULL;
  int *i = 0;

  if (!pa || !pb)
    return NULL;

  v_dl = (void **) pa;

  dptr_mod = (dlist_t *) pb;
  if (!dptr_mod)
    return NULL;

  module = (module_t *) dlist_data (dptr_mod);

  if (!module)
    return NULL;
  if (!sNULL (module->trigger))
    return NULL;

  dl_text_run = (dlist_t **) v_dl[0];
  dl_text_io_input = (dlist_t **) v_dl[1];
  dl_text_io_output = (dlist_t **) v_dl[2];
  dl_text_timer = (dlist_t **) v_dl[3];
  i = (int *) v_dl[4];

  *i = *i + 1;


  dl_str_unite (dl_text_run,
		"module_list: %i. name=%s, trigger=%s, active=%i, locked=%i, count=%i, init=%p, fini=%p, run=%p\n",
		*i, module->name, module->trigger, module->active,
		module->locked, module->tmp_stat_count, module->init,
		module->fini, module->run);

  if (module->type & XMODULE_TYPE_MIRROR)
    return v;

  if (module->output)
    {
      dl_str_unite (dl_text_io_input,
		    "module_list: OUTPUT: %i. name=%s, trigger=%s, active=%i, init=%p, fini=%p, help=%p, run=%p\n",
		    *i, module->name, module->trigger, module->active,
		    module->init, module->fini, module->help, module->run);
    }

  if (module->input)
    {
      dl_str_unite (dl_text_io_input,
		    "module_list: INPUT: %i. name=%s, trigger=%s, active=%i, init=%p, fini=%p, help=%p, run=%p\n",
		    *i, module->name, module->trigger, module->active,
		    module->init, module->fini, module->help, module->run);
    }


  if (module->timer)
    {
      dl_str_unite (dl_text_timer,
		    "module_list: TIMER: %i. name=%s, trigger=%s, init=%p, fini=%p, help=%p, run=%p\n",
		    *i, module->name, module->trigger, module->init,
		    module->fini, module->help, module->run);
    }

  return v;
}





void
xmodule_load_all (int type)
{
  dlist_t *dl_tok = NULL, *dptr_tok = NULL, *dptr_mod = NULL;
  module_t *module;

  char *tok = NULL;

  char **dl_pointer_str = NULL;
  dlist_t **dl_pointer = NULL, **dl_pointer_orig = NULL;

  debug (NULL, "xmodule_load_all: Entered\n");


  switch (type)
    {
    case XMODULE_TYPE_MODULE:
      dl_pointer = &gi->dl_module;
      dl_pointer_str = &gi->dl_module_str;
      break;
    case XMODULE_TYPE_PMODULE:
      dl_pointer = &gi->dl_pmodule;
      dl_pointer_str = &gi->dl_pmodule_str;
      break;
    case XMODULE_TYPE_GMODULE:
      dl_pointer = &gi->dl_gmodule;
      dl_pointer_str = &gi->dl_gmodule_str;
      break;
    default:
      return;
    }


  if (!dl_pointer)
    return;

  if (!dl_pointer_str)
    return;
  if (!(*dl_pointer_str))
    return;


  dl_pointer_orig = dl_pointer;


  dl_tok =
    tokenize (NULL, *dl_pointer_str, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE,
	      ",");
  if (!dl_tok)
    return;

  dlist_fornext (dl_tok, dptr_tok)
  {

    tok = (char *) dlist_data (dptr_tok);

    module = xmodule_load (type, tok);

    if (module)
      {
	debug (NULL,
	       "module_load_all: Successfully loaded module: %s, %s\n",
	       module->name, module->filename);

	dptr_mod = xmodule_insert (dl_pointer, module);

      }
    else
      {
	debug (NULL, "module_load_all: Failed to load module: %s\n", tok);
      }

  }

  tokenize_destroy (NULL, &dl_tok);

  return;
}




void
xmodule_free_destroy_dptr (void *arg)
{
  dlist_t *dptr_mod = NULL;
  module_t *mod = NULL;

  dptr_mod = (dlist_t *) arg;
  if (!dptr_mod)
    return;

  mod = (module_t *) dlist_data (dptr_mod);
  if (!mod)
    return;

  xmodule_unload (mod->type, mod, NULL);

  free (mod);

  return;
}




void
xmodule_free_destroy (void *arg)
{
  module_t *mod = NULL;

  mod = (module_t *) arg;
  if (!mod)
    return;

  xmodule_unload (mod->type, mod, NULL);

  free (mod);

  return;
}


/*
void
module_deactivate_dptr (void *arg)
{
  dlist_t *dptr_mod = NULL;
  module_t *mod = NULL;

  mod = (module_t *) arg;
  if (!mod)
    return;

  dptr_mod = (dlist_t *) mod->dptr_self;
  if (!dptr_mod)
    return;

  xmodule_unload (mod->type, mod, NULL);

  return;
}
*/



/*
void
module_deactivate (void *arg)
{
  module_t *module;

  if (!arg)
    return;

  module = (module_t *) arg;
  if (!module)
    return;

  xmodule_unload (module->type, module, NULL);

  return;
}
*/





void
xmodule_unload_everything (void)
{
  xmodule_unload_all (XMODULE_TYPE_MODULE);
  xmodule_unload_all (XMODULE_TYPE_PMODULE);
  xmodule_unload_all (XMODULE_TYPE_GMODULE);
  return;
}


void
xmodule_destroy_everything (void)
{
  void (*f) (void *) = NULL;

  if (gi->var_xmod_style == XMODULE_STYLE_TREE)
    {
      f = xmodule_free_destroy_dptr;
      if (gi->dl_module)
	gi->dl_module->avl_free = xmodule_avl_free_destroy;
      if (gi->dl_pmodule)
	gi->dl_pmodule->avl_free = xmodule_avl_free_destroy;
      if (gi->dl_gmodule)
	gi->dl_gmodule->avl_free = xmodule_avl_free_destroy;
    }
  else
    {
      f = xmodule_free_destroy;
    }

  dlist_fini (&gi->dl_module, f);
  dlist_fini (&gi->dl_pmodule, f);
  dlist_fini (&gi->dl_gmodule, f);
  dlist_fini (&gi->dl_mod_timers, xmodule_free_destroy);
  dlist_fini (&gi->dl_gmod_gtimers, xmodule_free_destroy);
  dlist_fini (&gi->dl_mod_iohooks, xmodule_free_destroy);
  dlist_fini (&gi->dl_gmod_giohooks, xmodule_free_destroy);

  return;
}


bot_t *
xmodule_unload_all (int type)
{

  dlist_t **dl_pointer = NULL;

  debug (NULL, "module_unload_all: Entered\n");

  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return NULL;
  if (!(*dl_pointer))
    return NULL;


  dlist_traverse (dl_pointer, xmodule_unload_all_traverse, dl_pointer);


  return NULL;
}






/* console parsing stuff */
void
console_xmodload (int type, char *arg)
{
  dlist_t *dptr_mod;
  module_t *module;
  char *mod_name = arg;

  dlist_t **dl_pointer = NULL;

  if (!arg)
    return;

  strstrip_nl (arg);

  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;


  if (!strncasecmp_len (mod_name, "ALL"))
    {
/* load all of the modules */
      xmodule_load_all (type);
    }
  else
    {
/* load a single mod */
      module = xmodule_load (type, mod_name);
      if (!module)
	return;
      dptr_mod = xmodule_insert (dl_pointer, module);
    }

  return;
}





void
console_xmodunload (int type, char *arg)
{
  char *mod_name = (char *) arg;

  dlist_t **dl_pointer = NULL;

  if (!arg)
    return;

  strstrip_nl (arg);


  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;

  if (!(*dl_pointer))
    return;



  if (!strncasecmp_len (mod_name, "ALL"))
    {
/* unload all of the modules */


      dlist_fini (dl_pointer, xmodule_free);
    }
  else
    {
/* unload a single mod */

      xmodule_unload (type, NULL, mod_name);
    }

  return;
}




void
console_xmodreload (int type, char *arg)
{
  module_t *module;
  char *mod_name = arg;

  char *mod_name_tmp = NULL;

  dlist_t **dl_pointer = NULL;

  if (!arg)
    return;

  strstrip_nl (arg);


  dl_pointer = xmodule_type_to_dl (type);

  if (!dl_pointer)
    return;

  module = xmodule_find_by_trig (type, mod_name);
  if (module)
    {
      mod_name_tmp = strdup (module->name);
    }
  else
    mod_name_tmp = strdup (mod_name);

  console_xmodunload (type, mod_name);
  console_xmodload (type, mod_name_tmp);

  if (mod_name_tmp)
    free (mod_name_tmp);


  return;
}






void
module_add_subtrigs (module_t * m, char *subtrig)
{
/*
dlist_t * dptr_subtrig=NULL;
module_t * mod_subtrig=NULL;
*/

/* SUPERFIX */


  if (!m || !subtrig)
    return;

/*
  for (i = 0; i < MODULE_SUBTRIG_MAX; i++)
    {
      if (!sNULL(m->subtrigs[i]))
	{
	strlcpy_buf(  m->subtrigs[i], subtrig);
	  break;
	}
    }
*/

  dlist_Dinsert_after (&m->dl_subtrigs, strdup (subtrig));

  return;
}





module_t *
xmodule_find_by_trig (int type, char *name)
{
  dlist_t *dptr = NULL;

  debug (NULL, "xmodule_find_by_trig: Entered: name=%s\n", name);

  dptr = xmodule_find_by_trig_dptr (type, name);

  if (dptr)
    {
      return (module_t *) dlist_data (dptr);
    }

  return NULL;
}


dlist_t *
xmodule_find_by_trig_dptr (int type, char *name)
{
  dlist_t *dl = NULL, **dl_pointer = NULL, *dptr, dptr_mod, *dptr_tmp;
  module_t *module = NULL, mod;

  debug (NULL, "xmodule_find_by_trig_dptr: Entered: %s\n", name);

  if (!sNULL (name))
    return NULL;

  dl_pointer = xmodule_type_to_dl (type);
  if (!dl_pointer)
    {
      return NULL;

    }
  dl = *dl_pointer;
  if (!dl)
    {
      return NULL;
    }


  if (gi->var_xmod_style == XMODULE_STYLE_LIST)
    {
      dlist_fornext_retarded (dl, dptr, dptr_tmp)
      {
	if (!dptr)
	  break;

	module = (module_t *) dlist_data (dptr);

	if (!module)
	  continue;

	if (!sNULL (module->trigger))
	  continue;

	if (!strcasecmp (module->trigger, name))
	  return dptr;
      }
      dptr = NULL;
    }
  else if (gi->var_xmod_style == XMODULE_STYLE_TREE)
    {
      bz2 (dptr_mod);
      bz2 (mod);
      strlcpy_buf (mod.trigger, name);
      dptr_mod.data = &mod;
      dptr = dlist_tree_find_dptr (&dl, &dptr_mod);
      if (dptr)
	{
	  module = (module_t *) dlist_data (dptr);

	  return dptr;
	}

    }

  return NULL;
}




module_t *
xmodule_find_by_name (int type, char *name)
{
  dlist_t **dl_pointer = NULL;
  module_t *module = NULL;

  debug (NULL, "xmodule_find_by_name: Entered: name=%s\n", name);

  if (!sNULL (name))
    return NULL;

  dl_pointer = xmodule_type_to_dl (type);
  if (!dl_pointer)
    return NULL;

  module =
    (module_t *) dlist_traverse (dl_pointer, xmodule_tree_match_by_name,
				 name);

  return module;
}



void *
xmodule_tree_match_by_name (void *pa, void *pb)
{
/* pa = char *, pb = dlist_t * */
  module_t *mod = NULL;
  dlist_t *b = NULL;
  char *a = NULL;

  if (!pa || !pb)
    return NULL;
  a = (char *) pa;
  b = (dlist_t *) pb;

  mod = (module_t *) dlist_data (b);
  if (!mod)
    return NULL;

  if (!sNULL (mod->name))
    return NULL;

  if (!strcasecmp (mod->name, a))
    {
      return mod;
    }
  return NULL;
}


void *
xmodule_unload_all_traverse (void *pa, void *pb)
{
  module_t *mod = NULL;
  dlist_t *b = NULL, **c = NULL;

  debug (NULL, "xmodule_unload_all_traverse: Entered\n");

  if (!pb)
    {
      return NULL;
    }

  c = (dlist_t **) pa;
  b = (dlist_t *) pb;
  mod = (module_t *) dlist_data (b);
  if (!mod)
    return NULL;

  if (mod->locked)
    return NULL;

/* WORKS
xmodule_unload(mod->type, mod);
dlist_remove(c, b);
*/

  dlist_remove_and_free (c, &b, xmodule_free_destroy);
//xmodule_free_destroy((void *)mod);

  return NULL;
}



void
xmodule_update (int type, module_t * mod_struct, char *trig)
{
  module_t *mod = NULL;

  debug (NULL, "xmodule_update: Entered\n");

  if (!sNULL (trig) || !mod_struct)
    return;

  mod = xmodule_find_by_trig (type, trig);
  if (!mod)
    return;

  memcpy (mod, mod_struct, sizeof (module_t));

  return;
}





void
xmodule_lock (int type, char *trig)
{
  module_t *mod = NULL;

  debug (NULL, "xmodule_lock: type=%i, trigger=%s\n", type, trig);

  mod = xmodule_find_by_trig (type, trig);
  if (!mod)
    return;

  mod->locked = 1;

  return;
}


void
xmodule_unlock (int type, char *trig)
{
  module_t *mod = NULL;
  debug (NULL, "xmodule_unlock: trigger=%s\n", trig);

  mod = xmodule_find_by_trig (type, trig);
  if (!mod)
    return;

  mod->locked = 0;

  return;
}




void
xmodule_parse_style (char *type)
{

  debug (NULL, "xmodule_parse_style: Entered\n");

  if (!type)
    return;

  if (!strcasecmp (type, "list"))
    {
      gi->var_xmod_style = XMODULE_STYLE_LIST;

/* FIXFIX: if it's already a tree, turn it into a list */
    }
  else if (!strcasecmp (type, "tree"))
    {
      gi->var_xmod_style = XMODULE_STYLE_TREE;

/* FIXFIX: if it's already a list, turn it into an avl tree */

      if (!gi->dl_module)
	gi->dl_module =
	  dlist_tree_init (NULL, xmodule_avl_compare, xmodule_avl_free);

      if (!gi->dl_pmodule)
	gi->dl_pmodule =
	  dlist_tree_init (NULL, xmodule_avl_compare, xmodule_avl_free);

      if (!gi->dl_gmodule)
	gi->dl_gmodule =
	  dlist_tree_init (NULL, xmodule_avl_compare, xmodule_avl_free);
    }


  return;
}




int
xmodule_avl_compare (const void *pa, const void *pb, void *param)
{
  dlist_t *dptr_a = (dlist_t *) pa, *dptr_b = (dlist_t *) pb;
  module_t *a = NULL, *b = NULL;
  int n = 0;

  if (!pa || !pb)
    return -1;

  a = (module_t *) dlist_data (dptr_a);
  b = (module_t *) dlist_data (dptr_b);

  if (!a || !b)
    return -1;

  if (!sNULL (a->trigger) || !sNULL (b->trigger))
    return -1;

  n = strcasecmp (a->trigger, b->trigger);

  return n;
}



void
xmodule_avl_free (void *data, void *param)
{
  dlist_t *dptr = (dlist_t *) data;
  module_t *mod = NULL;
  if (!data)
    return;

  mod = (module_t *) dlist_data (dptr);
  if (!mod)
    return;

  xmodule_unload (mod->type, mod, NULL);

  return;
}



void
xmodule_avl_free_destroy (void *data, void *param)
{
  dlist_t *dptr = (dlist_t *) data;
  module_t *mod = NULL;
  if (!data)
    return;

  mod = (module_t *) dlist_data (dptr);
  if (!mod)
    return;

  xmodule_unload (mod->type, mod, NULL);

  free (mod);
  dptr->data = NULL;
  free (dptr);

  return;
}









void *
xmodule_tree_print_node (void *pa)
{
  dlist_t *a = NULL;
  module_t *a2 = NULL;
  char *s1 = NULL;
  static char buf[1024];

  a = (dlist_t *) pa;

  if (a)
    {

      a2 = (module_t *) dlist_data (a);

      if (a2)
	{

	  s1 = a2->trigger;
	}

    }

  bz (buf);
  snprintf_buf (buf, "%s", s1);

  return (char *) buf;
}


void
xmodule_del_timer (module_t * mod_parent, char *trig)
{
  dlist_t *dptr = NULL, **dl_pointer = NULL;
  module_t *mod = NULL;

  debug (NULL, "xmodule_del_timer: Entered\n");


  if (!mod_parent || !sNULL (trig))
    return;

  dl_pointer = xmodule_typetimer_to_dl (mod_parent->type);
  if (!dl_pointer)
    return;

  if (!(*dl_pointer))
    return;

  dptr = xmodule_find_timer (mod_parent, trig);
  if (!dptr)
    return;

  mod = (module_t *) dlist_data (dptr);
  if (mod)
    {
      xmodule_unload (mod->type, mod, NULL);
    }

  dlist_remove_and_free_wrapper (dl_pointer, &dptr, xmodule_free_destroy);

  return;
}


dlist_t *
xmodule_find_timer (module_t * mod, char *trig)
{
  dlist_t *dptr = NULL, **dl_pointer = NULL;
  module_t *mod_ptr = NULL;

  debug (NULL, "xmodule_find_timer: Entered\n");

  if (!mod || !sNULL (trig))
    return NULL;

  dl_pointer = xmodule_typetimer_to_dl (mod->type);
  if (!dl_pointer)
    return NULL;

  if (!(*dl_pointer))
    return NULL;

  dlist_fornext (*dl_pointer, dptr)
  {
    mod_ptr = (module_t *) dlist_data (dptr);
    if (!mod_ptr)
      continue;

    if (!strcasecmp (mod_ptr->trigger, trig))
      {
	return dptr;
      }

  }

  return NULL;

}



void
xmodule_add_timer (module_t * mod)
{
  dlist_t *dptr = NULL, *dptr_mod = NULL, **dl_pointer = NULL;
  module_t *mod_ptr = NULL;

  debug (NULL, "xmodule_add_timer: Entered\n");

  if (!mod)
    return;

  dl_pointer = xmodule_typetimer_to_dl (mod->type);
  if (!dl_pointer)
    return;

  dptr = xmodule_find_timer (mod, mod->trigger);
  if (dptr)
    return;

  mod_ptr =
    xmodule_dup_mirror (mod, mod->name, mod->trigger, mod->trigger_ext);
  if (!mod_ptr)
    return;

  dptr_mod = xmodule_insert (dl_pointer, mod_ptr);

  return;
}



void
xmodule_del_iohook (module_t * mod_parent, char *trig)
{
  dlist_t *dptr = NULL, **dl_pointer = NULL;
  module_t *mod = NULL;

  debug (NULL, "xmodule_del_iohook: Entered\n");

  if (!mod_parent || !sNULL (trig))
    {
      return;
    }

  dl_pointer = xmodule_typeio_to_dl (mod_parent->type);
  if (!dl_pointer)
    {
      return;
    }

  if (!(*dl_pointer))
    {
      return;
    }

  dptr = xmodule_find_iohook (mod_parent, trig);
  if (!dptr)
    {
      return;
    }

  mod = (module_t *) dlist_data (dptr);

  if (mod)
    {
      xmodule_unload (mod->type, mod, NULL);
    }

  dlist_remove_and_free_wrapper (dl_pointer, &dptr, xmodule_free_destroy);

  return;
}



dlist_t *
xmodule_find_iohook (module_t * mod, char *trig)
{
  dlist_t *dptr = NULL, **dl_pointer = NULL;
  module_t *mod_ptr = NULL;

  debug (NULL, "xmodule_find_iohook: Entered\n");

  if (!mod || !sNULL (trig))
    return NULL;

  dl_pointer = xmodule_typeio_to_dl (mod->type);
  if (!dl_pointer)
    return NULL;

  if (!(*dl_pointer))
    return NULL;

  dlist_fornext (gi->dl_mod_iohooks, dptr)
  {
    mod_ptr = (module_t *) dlist_data (dptr);
    if (!mod_ptr)
      continue;

    if (!strcasecmp (mod_ptr->trigger, trig))
      {
	return dptr;
      }

  }

  return NULL;
}


void
xmodule_add_iohook (module_t * mod)
{
  dlist_t *dptr = NULL, *dptr_mod = NULL, **dl_pointer;
  module_t *mod_ptr = NULL;

  debug (NULL, "xmodule_add_iohook: Entered\n");

  if (!mod)
    return;

  dl_pointer = xmodule_typeio_to_dl (mod->type);
  if (!dl_pointer)
    {
      return;
    }

  dptr = xmodule_find_iohook (mod, mod->trigger);
  if (dptr)
    {
      return;
    }

  mod_ptr =
    xmodule_dup_mirror (mod, mod->name, mod->trigger, mod->trigger_ext);
  if (!mod_ptr)
    return;

  dptr_mod = xmodule_insert (dl_pointer, mod_ptr);

  return;
}





module_t *
xmodule_dup_mirror (module_t * mod, char *name, char *trig, char *trig_ext)
{
  module_t *mod_mir = NULL;

  debug (NULL, "xmodule_dup_mirror: Entered: name=%s , trig=%s\n", name,
	 trig);

  if (!mod || !sNULL (name) || !sNULL (trig))
    return NULL;


  mod_mir = (module_t *) calloc (1, sizeof (module_t));
  if (!mod_mir)
    return NULL;

  memcpy (mod_mir, mod, sizeof (module_t));

  mod_mir->type = mod->type;
  mod_mir->type |= XMODULE_TYPE_MIRROR;

  strlcpy_buf (mod_mir->name, name);
  strlcpy_buf (mod_mir->trigger, trig);
  strlcpy_buf (mod_mir->trigger_ext, trig_ext);
  strlcpy_buf (mod_mir->trigger_root, trig);
  mod_mir->dl_pointer = mod->dl_pointer;
  mod_mir->self = mod;

  bz (mod_mir->filename);
/*
bz(mod_mir->subtrigs);
*/
  mod_mir->dl_subtrigs = NULL;

  mod_mir->dl_handle = NULL;

  return mod_mir;
}





void
xmodule_del_subtrigs (module_t * mod)
{

  if (!mod)
    return;

  dlist_fini (&mod->dl_subtrigs, free);

  return;
}



module_t *
xmodule_load_subtrigs (module_t * mod)
{
  dlist_t *dptr_subtrig = NULL, *dptr_subtrig_tmp = NULL, *dptr_mod = NULL;
  module_t *mod_mir = NULL;
  char *subtrig_ptr = NULL;

  debug (NULL, "xmodule_load_subtrigs: Entered: mod=%p\n", mod);

  if (!mod)
    return NULL;

  if (mod->type & XMODULE_TYPE_MIRROR)
    return NULL;

/*
for(i=0;sNULL(mod->subtrigs[i])!=NULL;i++) {

dptr_mod = xmodule_find_by_trig_dptr(mod->type, mod->subtrigs[i]);
*/

  dlist_fornext_retarded (mod->dl_subtrigs, dptr_subtrig, dptr_subtrig_tmp)
  {
    if (!dptr_subtrig)
      break;

    subtrig_ptr = (char *) dlist_data (dptr_subtrig);
    if (!subtrig_ptr)
      continue;

    dptr_mod = xmodule_find_by_trig_dptr (mod->type, subtrig_ptr);

    if (dptr_mod)
      {
	mod_mir = (module_t *) dlist_data (dptr_subtrig);
	if (mod_mir)
	  {

	    xmodule_transfer (mod_mir, mod);

	    if (mod_mir->active)
	      return NULL;
	    mod_mir->active = 1;
	  }
	continue;
      }

    mod_mir =
      xmodule_dup_mirror (mod, mod->name, subtrig_ptr, mod->trigger_ext);
    if (!mod_mir)
      continue;

    debug (NULL,
	   "xmodule_add_subtrigs: dl_pointer=%p, mod_mir=%p ::: mod_mir->name=%s, mod_mir->trigger=%s, mod_mir->trigger_ext=%s\n",
	   mod_mir->dl_pointer, mod_mir, mod_mir->name, mod_mir->trigger,
	   mod_mir->trigger_ext);

    dptr_mod = xmodule_insert (mod_mir->dl_pointer, mod_mir);
  }

  return NULL;
}





dlist_t **
xmodule_type_to_dl (int type)
{
  dlist_t **dl = NULL;

  if (type & XMODULE_TYPE_MODULE)
    dl = &gi->dl_module;
  else if (type & XMODULE_TYPE_PMODULE)
    dl = &gi->dl_pmodule;
  else if (type & XMODULE_TYPE_GMODULE)
    dl = &gi->dl_gmodule;

  return dl;
}



dlist_t **
xmodule_typeio_to_dl (int type)
{
  dlist_t **dl = NULL;

  if (type & XMODULE_TYPE_MODULE)
    dl = &gi->dl_mod_iohooks;
  else if (type & XMODULE_TYPE_GMODULE)
    dl = &gi->dl_gmod_giohooks;

  return dl;
}

dlist_t **
xmodule_typetimer_to_dl (int type)
{
  dlist_t **dl = NULL;

  if (type & XMODULE_TYPE_MODULE)
    dl = &gi->dl_mod_timers;
  else if (type & XMODULE_TYPE_GMODULE)
    dl = &gi->dl_gmod_gtimers;

  return dl;
}





dlist_t *
xmodule_insert (dlist_t ** dl_pointer, module_t * mod)
{

  dlist_t *dptr_mod = NULL;

  module_t *mod2;

  if (!dl_pointer || !mod)
    return NULL;

  dptr_mod = dlist_Dinsert_after (dl_pointer, mod);

  if (!dptr_mod)
    return NULL;

  mod->dptr_self = (dlist_t *) dptr_mod;

  mod2 = (module_t *) dlist_data (mod->dptr_self);

  return dptr_mod;
}




void
xmodule_transfer (module_t * src, module_t * dst)
{


  debug (NULL, "xmodule_transfer: Entered\n");

  if (!src || !dst)
    return;

  src->init = dst->init;
  src->fini = dst->fini;
  src->help = dst->help;
  src->run = dst->run;
  src->stat = dst->stat;
  src->timer = dst->timer;
  src->on = dst->on;
  src->off = dst->off;

  src->destroy_up = dst->destroy_up;
  src->destroy_down = dst->destroy_down;
  src->control_up = dst->control_up;
  src->control_down = dst->control_down;
  src->output = dst->output;
  src->input = dst->input;

  return;
}




void
xmodule_zero (module_t * mod)
{

  if (!mod)
    return;

  mod->active = mod->locked = 0;

/* zero out mod->name, for reloading */
/*
bz(mod->filename);
bz(mod->trigger_ext);
*/

  if (mod->type & XMODULE_TYPE_MIRROR)
    {
      bz (mod->name);
/*
bz(mod->subtrigs);
*/
      mod->dl_subtrigs = NULL;
    }

/*
bz(mod->trigger_root);

*/

  mod->init = mod->fini = mod->help = mod->run = mod->stat = mod->timer =
    mod->on = mod->off = NULL;

  mod->destroy_up = mod->destroy_down = mod->control_up = mod->control_down =
    mod->output = mod->input = NULL;

/*
mod->self = NULL;
mod->dptr_self = NULL;
*/
  mod->dl_handle = NULL;
  mod->tmp_stat_count = 0;
  mod->max_nesting = 0;
//mod->dl_pointer = NULL;

  return;
}



void
xmodule_free (void *arg)
{
  module_t *mod = (module_t *) arg;

  debug (NULL, "xmodule_free\n");

  if (!mod)
    return;

  free (mod);

  return;
}
