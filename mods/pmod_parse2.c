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
#include "pmod_parse2.h"

int parse2_handle_text (dlist_t *, bot_t *, void *);

void __parse2_init__ (void) __attribute__ ((constructor));

void
__parse2_init__ (void)
{

  strlcpy_buf (pmod_parse2_info.name, "pmod_parse2");
  strlcpy_buf (pmod_parse2_info.trigger, "^parse2");
  pmod_parse2_info.init = parse2_init;
  pmod_parse2_info.fini = parse2_fini;
  pmod_parse2_info.help = parse2_help;
  pmod_parse2_info.run = parse2_run;


  pmod_parse2_info.output = NULL;
  pmod_parse2_info.input = NULL;


  debug (NULL, "__parse2_init__: Loaded mod_parse2\n");

  return;
}



bot_t *
parse2_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse2_init: Entered\n");

  swap_inmem_get_assign_and_remove ("dl_pmod_parse2", 0,
				    (void **) &dl_pmod_parse2_unique);

  return NULL;
}

bot_t *
parse2_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse2_fini: Entered\n");

  swap_inmem_create_and_add ("dl_pmod_parse2", 0, dl_pmod_parse2_unique);

/*
  parse2_allow_clear ();
  parse2_deny_clear ();
*/

  return NULL;
}

bot_t *
parse2_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse2_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^parse2";

  return NULL;
}

bot_t *
parse2_run (dlist_t * dlist_node, bot_t * bot)
{
  unique_t *bu = NULL;
  parse2_data_t *p2data = NULL;
  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "parse2_run: Entered: %p %p\n", dlist_node, bot);

  pmodule_cur_set (parse2_run, dlist_node, bot);

  if (!dlist_node || !bot)
    return NULL;

  debug (bot,
	 "parse2_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

  stat_inc (bot, bot->trig_called);

  bu =
    unique_create (bot, &dl_pmod_parse2_unique, UNIQUE_ID_ID | UNIQUE_ID_TAG);
  p2data = (parse2_data_t *) bu->data;

  if (!p2data)
    {

      p2data = parse2_data_init ();
      bu->data = p2data;

      MOD_OPTIONS_TOP_HALF;

      parse2_process_options (dlist_node, bot, dl_options_ptr, p2data);

      p2data->initialized = 1;

      MOD_OPTIONS_BOTTOM_HALF;
    }

  parse2_handle_text (dlist_node, bot, p2data);

/*
  parse2_allow_clear ();
  parse2_deny_clear ();
*/


  return bot;
}




int
parse2_handle_text (dlist_t * dlist_node, bot_t * bot, void *arg_info)
{
  dlist_t *dl_text_list = NULL;
  char *str_ptr;

  debug (bot, "parse2_handle_text: Entered\n");

  if (!bot)
    return -1;

  str_ptr = bot->txt_data_in;

  dlist_Dpush (&bot->stack_cmds, strdup (str_ptr));

  parse2_build_text_list (&dl_text_list, str_ptr, strlen (str_ptr), 0);
  parse2_nesting = 0;
  parse2_build_text_list_print (dl_text_list);
  parse2_nesting = 0;
  parse2_build_text_list_run (dl_text_list, dlist_node, bot, arg_info);
  parse2_text_list_destroy (&dl_text_list);

  dlist_pop_and_free (&bot->stack_cmds, free);

  return 0;
}


int
parse2_handle_text_pipes (dlist_t * dlist_node, bot_t * bot, void *arg_info)
{
  parse2_data_t *p2data = NULL;

  dlist_t *text_list;
  dlist_t *dptr, *dptr_mod;
  module_t *module;
  char *str_ptr, *trig_ptr, *trigfound_ptr = NULL;
  int trigfound, subtrig_found;

  int i, j;

  int trig_called_proper_len = 0, trig_ptr_dup = 0;

  debug (bot, "parse2_handle_text_pipes: Entered\n");


  if (!bot)
    {
      return -1;
    }

/* 
 * ^trigger text1
 * ^trigger(options) text1
 * ^trigger text1 || ^trigger ...
 * ^trigger text1 || ^trigger(options) ...
 * text1 || ^trigger
 * text1 || ^trigger(options)
 * text1 || ^trigger text2 
 * text1 || ^trigger(options) text2
 *
 * stack/queue
 * {module, input_text, text} <- module operates on input_text, then text
 * ^trigger text1 = {^trigger, NULL, text1}
 * text1 || ^trigger = {NULL, NULL, text1} {^trigger, INPUT, NULL}
 * text1 || ^trigger text2 = {NULL, NULL, text1} {^trigger, INPUT, text2}
 * ^trigger1 text1 || ^trigger2 text2 = {^trigger1, NULL, text1} {^trigger2, INPUT, text2}
 */

  debug (bot, "parse2_handle_text_pipes: Entered\n");

  debug (bot,
	 "parse2_handle_text_pipes: nesting level is %i (current max nesting is %i)\n",
	 bot->var_nesting_cur, bot->var_nesting);

  if (bot->var_nesting_cur >= bot->var_nesting)
    {
      debug (bot, "parse2_handle_text_pipes: nesting limit reached!\n");
      return 0;
    }
  bot->var_nesting_cur++;


  p2data = (parse2_data_t *) arg_info;

  text_list = dptr = NULL;

  str_ptr = bot->txt_data_in;

  debug (bot, "parse2_handle_text_pipes: to=%s, out=%s, in=%s\n",
	 bot->txt_to, bot->txt_data_out, bot->txt_data_in);



/* hooks: output module processing */
/* XXX XXX XXX
  dlist_fornext (gi->dl_module, dptr)
  {
    module = (module_t *) dlist_data (dptr);
    if (module->output)
      module->output (dptr, bot);
  }
*/

  if (bot->isprivmsg)
    {
      i = str_transform_pipes (str_ptr);


      while (1)
	{
	  str_ptr = strtok (str_ptr, "|");
	  if (!str_ptr)
	    break;

	  j = strlen (str_ptr);
	  i = str_untransform_pipes (str_ptr);

	  dptr = dlist_Dinsert_after (&text_list, strdup (str_ptr));
	  str_ptr = NULL;
	}

      memset (bot->txt_data_in, 0, sizeof (bot->txt_data_in));

      if (text_list)
	{
	  dlist_fornext (text_list, dptr)
	  {


	    str_ptr = (char *) dlist_data (dptr);
	    debug (bot, "parse2_handle_text_pipes: text_list: node=%s\n",
		   str_ptr);

// FIX - change this to searching through trigger mods only
	    trigfound = 0;

	    if (trig_ptr_dup)
	      {
		free (trig_ptr);
		trig_ptr_dup = 0;
	      }

	    trig_ptr = NULL;

	    trig_ptr = eat_whitespace (str_ptr);

	    debug (NULL,
		   "parse2_handle_text_pipes: trig_prefix=%s, trig_ptr=%s\n",
		   bot->trig_prefix, trig_ptr);

	    if (sNULL (bot->trig_prefix))
	      {
		if (!strncasecmp_len (trig_ptr, bot->trig_prefix))
		  {
		    trig_ptr =
		      str_unite ("^%s", trig_ptr + strlen (bot->trig_prefix));
		    trig_ptr_dup = 1;
		  }
//else continue;
		else
		  {
		    goto trig_not_found;
		  }
	      }

	    bz (bot->trig_called);
	    strlcpy_buf (bot->trig_called, trig_ptr);

	    if (!strncmp (bot->trig_called, "^:", 2)
		|| !strncmp (bot->trig_called, "^!", 2)
		|| !strncmp (bot->trig_called, "^#", 2))
	      {
		bot->trig_called[2] = '\0';
	      }
	    else
	      {
		for (i = 1; i < strlen (bot->trig_called); i++)
		  {
		    if (!istrig (bot->trig_called[i]))
		      {
			bot->trig_called[i] = '\0';
			break;
		      }
		  }
	      }

	    dptr_mod = module_find_by_trig_dptr (bot->trig_called);
	    if (!dptr_mod)
	      goto trig_not_found;

	    module = (module_t *) dlist_data (dptr_mod);
	    if (!module)
	      goto trig_not_found;

	    trigfound_ptr = module->trigger;

	    trig_called_proper_len = fn_strlen (bot->trig_called);

	    subtrig_found = 1;

	    if (parse2_deny_find (p2data, trigfound_ptr))
	      continue;

	    if (!parse2_allow_find (p2data, trigfound_ptr))
	      continue;

	    debug (bot,
		   "parse2_handle_text_pipes: trigger caught, %s\n",
		   bot->trig_called);
	    bot->dl_module_arg = trig_ptr + trig_called_proper_len;

	    if (module->run)
	      {
		module->run (dptr, bot);
	      }

	    debug (bot,
		   "parse2_handle_text_pipes: bot->isforked=%i, bot->isforked_stop=%i\n",
		   bot->isforked, bot->isforked_stop);

	    if (bot->isforked_stop)
	      {
		bot->isforked_stop = 0;
		trigfound = 0;
// FIX?
		bot->shouldsend = 0;
		goto cleanup;
	      }

	    trigfound = 1;


	  trig_not_found:
	    if (!trigfound && !bot->iscomment)
	      {
		strlcat_bot (bot->txt_data_out, str_ptr);
	      }


	  }


	}



      else
	debug (bot, "parse2_handle_text_pipes: text_list=NULL\n");
    }

  if (!sNULL (bot->txt_data_out))
    {
    }

cleanup:
  dlist_fini (&text_list, free);

  str_clean_sep_shrink (bot->txt_data_out, strlen (bot->txt_data_out));

/* cleaning up bot data */
  memset (&bot->trig_called, 0, sizeof (bot->trig_called));

  return 0;
}









void
parse2_build_text_list (dlist_t ** dl, char *input, int len, int type)
{
  dlist_t *dl_pointer = NULL;
  text_list_t *tl;
  char *closure_end = NULL, *trig = NULL;

  char *substring = NULL;
  int substring_len = 0;

  char *left_string = NULL;
  int left_string_len = 0;

  if (!dl || !input || len <= 0)
    return;

  parse2_nesting++;

  while (1)
    {

      trig = strstr (input, "^(");
      if (!trig)
	{
	  if (type == 0 && strlen (input))
	    {
	      tl = (text_list_t *) calloc (1, sizeof (text_list_t));
	      tl->string = strdup (input);
	      dlist_Dinsert_after (dl, tl);
	    }
	  break;
	}

      if (trig > input + len)
	break;

      closure_end = (char *) tokenize_find_closing_bracket (trig, '(');
      if (!closure_end)		/* violation */
	break;


      substring = trig;
      substring_len = (closure_end - trig) + 1;

      left_string = input;
      left_string_len = (trig - left_string);

      substring++;
      substring_len++;

      *(closure_end + 1) = '\0';


      if (type == 0 && left_string_len > 0)
	{
	  tl = (text_list_t *) calloc (1, sizeof (text_list_t));
	  tl->string = strdup_len (left_string, left_string_len);
	  dlist_Dinsert_after (dl, tl);
	}

      if (type == 0)
	{
	  tl = (text_list_t *) calloc (1, sizeof (text_list_t));
	  tl->string = strdup ("root");
	  tl->type = TEXT_LIST_ROOT;
	  dlist_Dinsert_after (dl, tl);

	  dl_pointer = (dlist_t *) & tl->children;
	}
      else
	{
	  dl_pointer = (dlist_t *) dl;
	}

      tl = (text_list_t *) calloc (1, sizeof (text_list_t));
      tl->string = str_unite ("^%.*s", substring_len, substring);


      dlist_Dinsert_after ((dlist_t **) dl_pointer, tl);
      parse2_build_text_list ((dlist_t **) dl_pointer, substring,
			      substring_len, 1);

      input = closure_end + 2;
    }

  parse2_nesting--;

  return;
}




void
parse2_build_text_list_print (dlist_t * dl)
{
  dlist_t *dptr;
  text_list_t *tl;

  if (!dl)
    return;

  parse2_nesting++;

  printf ("parse2_build_text_list_print: Entered: %p\n", dl);

  if (!dl)
    return;

  dlist_fornext (dl, dptr)
  {
    tl = (text_list_t *) dlist_data (dptr);

    if (tl->children)
      {
	parse2_build_text_list_print (tl->children);
      }
  }

  parse2_nesting--;

  return;
}



void
parse2_build_text_list_run (dlist_t * dl_text, dlist_t * dl_node, bot_t * bot,
			    void *arg_info)
{
  dlist_t *dptr_1, *dptr_2, *tail_or_head;
  text_list_t *tl_1, *tl_2;
  char buf[MAX_BUF_SZ + 1];

  if (!dl_text || !bot)
    return;

  memset (buf, 0, sizeof (buf));

  dlist_fornext (dl_text, dptr_1)
  {
/* left to right among the root nodes */
    tl_1 = (text_list_t *) dlist_data (dptr_1);
    if (!tl_1)
      continue;

    if (tl_1->type != TEXT_LIST_ROOT)
      printf ("%s\n", tl_1->string);

/* right to left among the children */
    if (tl_1->children)
      {
	tail_or_head =
	  dlist_tail (tl_1->children) ? dlist_tail (tl_1->children) :
	  tl_1->children;
	dlist_forprev (tail_or_head, dptr_2)
	{
	  tl_2 = (text_list_t *) dlist_data (dptr_2);
	  if (!tl_2)
	    continue;

	  if (tl_2->type != TEXT_LIST_ROOT)
	    {
	      debug (NULL, "parse2_build_text_list_run: -> %s\n",
		     tl_2->string);
	      memset (bot->txt_data_in, 0, sizeof (bot->txt_data_in));
	      memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
	      if (tl_2->string_new)
		snprintf_buf (bot->txt_data_in, "%.*s",
			      strlen (tl_2->string_new) - 3,
			      tl_2->string_new + 2);
	      else
		snprintf_buf (bot->txt_data_in, "%.*s",
			      strlen (tl_2->string) - 3, tl_2->string + 2);

	      debug (NULL, "parse2_build_text_list_run: TESTING %s\n",
		     bot->txt_data_in);
	      parse2_handle_text_pipes (NULL, bot, arg_info);
	      tl_2->result = strdup (bot->txt_data_out);
	      printf ("parse2_build_text_list_run: RESULT %s\n",
		      tl_2->result);
	      tl_2->type = TEXT_LIST_DEACTIVATED;
	      parse2_build_text_list_replace_last (tl_1->children, tl_2);
	    }
	}
      }

    if (tl_1->type != TEXT_LIST_ROOT)
      strlcat_bot (buf, tl_1->string);

    if (tl_1->children)
      {
	tl_2 = (text_list_t *) dlist_data (tl_1->children);
	if (tl_2)
	  {
	    if (tl_2->result)
	      {
		strlcat_bot (buf, tl_2->result);
	      }
	  }

      }

  }


  memset (bot->txt_data_in, 0, sizeof (bot->txt_data_in));
  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcat_bot (bot->txt_data_in, buf);
  parse2_handle_text_pipes (NULL, bot, arg_info);

  bot->txt_data_out_sz = strlen (bot->txt_data_out);

  return;
}






int
parse2_build_text_list_replace_last (dlist_t * dl_text_list, text_list_t * tl)
{
/* find the most recent matching substring, replace it with result */
  dlist_t *dptr = NULL, *tail_or_head = NULL;
  text_list_t *tl_ptr = NULL;
  int len_substring = 0, len_pre = 0, len_post = 0;
  char buf[MAX_BUF_SZ + 1], *substr = NULL, *poststr = NULL;

  if (!dl_text_list || !tl)
    return -1;

  memset (buf, 0, sizeof (buf));

  tail_or_head =
    dlist_tail (dl_text_list) ? dlist_tail (dl_text_list) : dl_text_list;
  dlist_forprev (tail_or_head, dptr)
  {
    tl_ptr = (text_list_t *) dlist_data (dptr);

    if (tl_ptr->type == TEXT_LIST_DEACTIVATED)
      continue;

    if ((substr = strstr (tl_ptr->string, tl->string)))
      {
	len_substring = strlen (tl->string);
	len_pre = substr - tl_ptr->string;
	len_post = strlen (tl_ptr->string) - len_pre - len_substring;
	poststr = substr + strlen (tl->string);

	snprintf_buf (buf, "%.*s%s%s", len_pre, tl_ptr->string,
		      tl->result, poststr);
	tl_ptr->string_new = strdup (buf);
	break;
      }

  }

  return 1;
}




void
parse2_text_list_destroy (dlist_t ** dl_text_list)
{
  dlist_t *dptr_1, *dptr_2;
  text_list_t *tl_ptr;

  if (!dl_text_list)
    return;

  dptr_1 = dptr_2 = NULL;
  dlist_fornext_retarded (*dl_text_list, dptr_1, dptr_2)
  {
    if (!dptr_1)
      break;
    tl_ptr = (text_list_t *) dlist_data (dptr_1);

    if (!tl_ptr)
      continue;

    if (tl_ptr->children)
      {
	parse2_text_list_destroy (&tl_ptr->children);
      }

    dlist_remove (dl_text_list, dptr_1);
    parse2_text_list_free (tl_ptr);
    free (dptr_1);
  }

  return;
}

void
parse2_text_list_free (void *tl_arg)
{
  text_list_t *tl;

  tl = (text_list_t *) tl_arg;
  if (!tl)
    return;

  if (tl->string)
    free (tl->string);

  if (tl->string_new)
    free (tl->string_new);

  if (tl->result)
    free (tl->result);

  free (tl);

  return;
}











void
parse2_process_options (dlist_t * dlist_node, bot_t * bot, char *string,
			parse2_data_t * p2data)
{

  char buf[MAX_BUF_SZ];

  dlist_t *dl = NULL, *dptr;

  debug (bot, "parse2_process_options: Entered\n");

  if (!bot || !sNULL (string) || !p2data)
    return;

  memset (buf, 0, sizeof (buf));

  dl = tokenize (bot, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "...");
  if (!dl)
    return;

  dlist_fornext (dl, dptr)
  {
    parse2_process_options_parse (dlist_node, bot,
				  (char *) dlist_data (dptr), p2data);
  }

  tokenize_destroy (bot, &dl);


  return;
}



void
parse2_process_options_parse (void *future, bot_t * bot, char *string,
			      parse2_data_t * p2data)
{

  if (!bot || !sNULL (string) || !p2data)
    return;

  debug (bot, "parse2_process_options_parse: Entered\n");

  if (!strncasecmp_len (string, "allow="))
    {
      parse2_process_options_parse_allow (future, bot,
					  &string[strlen ("allow=")], p2data);
    }
  else if (!strncasecmp_len (string, "deny="))
    {
      parse2_process_options_parse_deny (future, bot,
					 &string[strlen ("deny=")], p2data);
    }


  return;
}



void
parse2_process_options_parse_allow (void *future, bot_t * bot, char *string,
				    parse2_data_t * p2data)
{
  dlist_t *dl, *dptr;
  char *trigger;

  if (!bot || !string)
    return;

  if (!p2data)
    return;


  dl =
    tokenize (bot, string,
	      TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE |
	      TOKENIZE_LEAVEQUOTES, ",");
  if (!dl)
    return;

  dlist_fornext (dl, dptr)
  {
    trigger = (char *) dlist_data (dptr);
    if (!trigger)
      continue;
    parse2_allow_add (p2data, trigger);
  }

  tokenize_destroy (bot, &dl);

  return;
}





void
parse2_process_options_parse_deny (void *future, bot_t * bot, char *string,
				   parse2_data_t * p2data)
{
  dlist_t *dl, *dptr;
  char *trigger;

  if (!bot || !sNULL (string) || !p2data)
    return;


  dl =
    tokenize (bot, string,
	      TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE |
	      TOKENIZE_LEAVEQUOTES, ",");
  if (!dl)
    return;

  dlist_fornext (dl, dptr)
  {
    trigger = (char *) dlist_data (dptr);
    if (!trigger)
      continue;
    parse2_deny_add (p2data, trigger);
  }

  tokenize_destroy (bot, &dl);

  return;
}









void
parse2_allow_add (parse2_data_t * p2data, char *trigger)
{
  if (!p2data || !sNULL (trigger))
    return;

  if (parse2_allow_find (p2data, trigger))
    return;

  dlist_Dinsert_after (&p2data->allow, strdup (trigger));

  return;
}

void
parse2_allow_clear (parse2_data_t * p2data)
{

  if (!p2data)
    return;
  if (!p2data->allow)
    return;

  dlist_fini (&p2data->allow, free);

  return;
}


int
parse2_allow_find (parse2_data_t * p2data, char *trigger)
{
  dlist_t *dptr;
  char *allow_ptr;
  int ret;

  if (!p2data || !sNULL (trigger))
    return 0;

  if (!p2data->allow)
    return 1;

  ret = 0;

  dlist_fornext (p2data->allow, dptr)
  {
    allow_ptr = (char *) dlist_data (dptr);
    if (!allow_ptr)
      continue;
    if (!strcasecmp (allow_ptr, trigger))
      {
	return 1;
      }
  }

  return ret;
}








void
parse2_deny_add (parse2_data_t * p2data, char *trigger)
{
  if (!p2data || !sNULL (trigger))
    return;

  if (parse2_deny_find (p2data, trigger))
    return;

  dlist_Dinsert_after (&p2data->deny, strdup (trigger));

  return;
}

void
parse2_deny_clear (parse2_data_t * p2data)
{

  if (!p2data)
    return;
  if (!p2data->deny)
    return;

  dlist_fini (&p2data->deny, free);

  return;
}


int
parse2_deny_find (parse2_data_t * p2data, char *trigger)
{
  dlist_t *dptr;
  char *deny_ptr;
  int ret;

  if (!p2data || !sNULL (trigger))
    return 0;

  if (!p2data->deny)
    return 0;

  ret = 0;

  dlist_fornext (p2data->deny, dptr)
  {
    deny_ptr = (char *) dlist_data (dptr);
    if (!deny_ptr)
      continue;
    if (!strcasecmp (deny_ptr, trigger))
      {
	return 1;
      }
  }

  return ret;
}




parse2_data_t *
parse2_data_init (void)
{
  parse2_data_t *p2data;

  p2data = (parse2_data_t *) calloc (1, sizeof (parse2_data_t));
  if (!p2data)
    return NULL;

  return p2data;
}

void
parse2_data_fini (void *arg)
{
  parse2_data_t *p2data = NULL;

  p2data = (parse2_data_t *) arg;
  if (!p2data)
    return;

  return;
}
