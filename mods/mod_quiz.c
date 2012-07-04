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
#include "mod_quiz.h"

void
__quiz_init__ (void)
{

  strlcpy_buf (mod_quiz_info.name, "mod_quiz");
  strlcpy_buf (mod_quiz_info.trigger, "^quiz");

  module_add_subtrigs (&mod_quiz_info, "^answer");
  module_add_subtrigs (&mod_quiz_info, "^ans");

  mod_quiz_info.init = quiz_init;
  mod_quiz_info.fini = quiz_fini;
  mod_quiz_info.help = quiz_help;
  mod_quiz_info.run = quiz_run;

  mod_quiz_info.off = quiz_off;


  mod_quiz_info.output = NULL;
  mod_quiz_info.input = NULL;


  debug (NULL, "__quiz_init__: Loaded mod_quiz\n");

  return;
}


bot_t *
quiz_off (dlist_t * dlist_node, bot_t * bot)
{
  unique_t *bu = NULL;
  quiz_active_t **qa = NULL;
  dlist_t *dptr, *dptr_tmp;

  debug (NULL, "quiz_off: Entered\n");

  dlist_fornext_retarded (dl_quiz_active_unique, dptr, dptr_tmp)
  {
    if (!dptr)
      break;

    bu = (unique_t *) dlist_data (dptr);
    if (!bu)
      continue;

    qa = (quiz_active_t **) & bu->data;
    if (!qa)
      continue;
  }


  dlist_fini (&dl_quiz_pairs, xdb_pair_destroy);

  return NULL;
}


bot_t *
quiz_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "quiz_init: Entered: %p\n", bot);

  quiz_clear_db_from_files ();
  quiz_add_files_to_db ();

  swap_inmem_get_assign_and_remove ("dl_quiz_active", 0,
				    (void **) &dl_quiz_active_unique);

  return NULL;
}

bot_t *
quiz_fini (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "quiz_fini: Entered\n");

  dlist_fini (&dl_quiz_pairs, xdb_pair_destroy);
  dl_quiz_pairs = NULL;

  swap_inmem_create_and_add ("dl_quiz_active", 0, dl_quiz_active_unique);

  return NULL;
}

bot_t *
quiz_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "quiz_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^quiz || ^quiz(get) <key> || ^quiz(add) <key> || ^quiz(del) <key> || ^quiz(getmore) <key> || ^a <key>";

  return NULL;
}


bot_t *
quiz_timer (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "quiz_timer: Entered\n");

  return NULL;
}


bot_t *
quiz_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "quiz_run: Entered\n");


  quiz_clear_db_from_files ();

  if (!bot)
    return NULL;


  if (gi->mongo_isconnected == 0)
    {
      debug (bot, "quiz_run: Mongo is not connected\n");
      return NULL;
    }

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "quiz_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

  if (bot_shouldreturn (bot))
    return NULL;

  opt = QUIZ_OPT_ANSWER;

  if (!strcasecmp (bot->trig_called, "^answer")
      || !strcasecmp (bot->trig_called, "^ans"))
    {
      opt = QUIZ_OPT_ANSWER;
    }


  MOD_OPTIONS_TOP_HALF;

  if (xstrcasestr_len (dl_options_ptr, "SETUP", len_options_area))
    {
      opt = QUIZ_OPT_SETUP;
    }
  else if (xstrcasestr_len (dl_options_ptr, "answer", len_options_area))
    {
      opt = QUIZ_OPT_ANSWER;
    }

  else if (xstrcasestr_len (dl_options_ptr, "get", len_options_area))
    {
      opt = QUIZ_OPT_GET;

    }
  else if (xstrcasestr_len (dl_options_ptr, "add", len_options_area))
    {
      opt = QUIZ_OPT_ADD;
    }

  else if (xstrcasestr_len (dl_options_ptr, "info", len_options_area))
    {
      opt = QUIZ_OPT_INFO;
    }

  else if (xstrcasestr_len (dl_options_ptr, "getmore", len_options_area))
    {

      opt = QUIZ_OPT_GETMORE;
    }
  else if (xstrcasestr_len (dl_options_ptr, "list", len_options_area))
    {
      opt = QUIZ_OPT_LIST;
    }

  else if (xstrcasestr_len (dl_options_ptr, "timeout", len_options_area))
    {
      opt = QUIZ_OPT_TIMEOUT;
    }
  else
    if (xstrcasestr_len (dl_options_ptr, "clear_winners", len_options_area))
    {
      opt = QUIZ_OPT_CLEAR_WINNERS;
    }
  else if (xstrcasestr_len (dl_options_ptr, "clear", len_options_area))
    {
      opt = QUIZ_OPT_CLEAR;
    }
  else if (xstrcasestr_len (dl_options_ptr, "size", len_options_area))
    {
      opt = QUIZ_OPT_SIZE;
    }
  else if (xstrcasestr_len (dl_options_ptr, "winners", len_options_area))
    {
      opt = QUIZ_OPT_WINNERS;
    }
  else if (xstrcasestr_len (dl_options_ptr, "channel", len_options_area))
    {
      opt = QUIZ_OPT_CHANNEL;
    }
  else if (xstrcasestr_len (dl_options_ptr, "multi", len_options_area))
    {
      opt = QUIZ_OPT_MULTI;
    }
  else if (xstrcasestr_len (dl_options_ptr, "newline", len_options_area - 1))
    {
      opt = QUIZ_OPT_NEWLINES;
    }
  else if (xstrcasestr_len (dl_options_ptr, "hidekey", len_options_area - 1))
    {
      opt = QUIZ_OPT_HIDEKEY;
    }
  else if (xstrcasestr_len (dl_options_ptr, "fixkey", len_options_area - 1))
    {
      opt = QUIZ_OPT_FIXKEY;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = quiz_change_string (bot, l_str_ptr, opt);
  if (l_new_str)
    bot->var_multi = 1;
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;

}









char *
quiz_change_string (bot_t * bot, char *string, int opt)
{
  unique_t *bu = NULL;
  quiz_active_t **qa = NULL;
  dlist_t *dl = NULL;
  char **arg_array = NULL;


  char *str = NULL;
  char buf[MAX_BUF_SZ];

  char *sep_ptr;

  debug (bot, "quiz_change_string: Entered\n");

  if (!bot || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  bu =
    unique_create (bot, &dl_quiz_active_unique,
		   UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
  if (!bu)
    return NULL;

  qa = (quiz_active_t **) & bu->data;
  if (!qa)
    {
    }

  memset (buf, 0, sizeof (buf));

  if (strlen (string))
    {
      dl =
	tokenize (bot, string, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
      if (!dl)
	return NULL;

      arg_array = (char **) dlist_convert_dlist_to_array (dl);
      if (!arg_array)
	goto cleanup;

      capsup (arg_array[0]);
    }

  switch (opt)
    {
    case QUIZ_OPT_LIST:
      {
	str = quiz_op_list (bot);
	break;
      }
    case QUIZ_OPT_INFO:
      {
	str = quiz_op_info (bot, *qa);
	break;
      }
    case QUIZ_OPT_CLEAR:
      {
	str = quiz_op_clear (bot, *qa);
	break;
      }
    case QUIZ_OPT_CLEAR_WINNERS:
      {
	str = quiz_op_clear_winners (bot, *qa);
	break;
      }
    case QUIZ_OPT_WINNERS:
      {
	str = quiz_op_winners (bot, *qa);
	break;
      }

    case QUIZ_OPT_SETUP:
      {
	str = quiz_op_setup (bot, qa, string);
	if (str)
	  {
	    module_t *m_ptr = NULL;
	    mod_quiz_info.timer = quiz_timer;
	    m_ptr = module_find_by_name ("mod_quiz");
	    if (m_ptr)
	      {
		m_ptr->timer = quiz_timer;
		module_update (m_ptr, "mod_quiz");
	      }
	  }
	if (*qa)
	  bu->data = *qa;
	break;
      }
    case QUIZ_OPT_ANSWER:
      {
	str = quiz_op_answer (bot, *qa, string);
	break;
      }
    case QUIZ_OPT_GET:
      {
	if (!arg_array[1])
	  return NULL;
	str = quiz_op_get (bot, arg_array[0], atoi (arg_array[1]));
	break;
      }
    case QUIZ_OPT_DEL:
      {
	break;
      }
    case QUIZ_OPT_TIMEOUT:
      {
	str = quiz_op_timeout (bot, *qa, atoi (arg_array[0]));
	break;
      }
    case QUIZ_OPT_SIZE:
      {
	str = quiz_op_size (bot, arg_array[0]);
	break;
      }
    case QUIZ_OPT_MULTI:
      {
	str = quiz_op_multi (bot, *qa, arg_array[0]);
	break;
      }
    case QUIZ_OPT_NEWLINES:
      {
	str = quiz_op_newlines (bot, *qa, arg_array[0]);
	break;
      }
    case QUIZ_OPT_HIDEKEY:
      {
	str = quiz_op_hidekey (bot, *qa, arg_array[1]);
	break;
      }

    default:
      break;
    }

cleanup:

  if (dl)
    tokenize_destroy (bot, &dl);

  if (arg_array)
    free (arg_array);

  if (str)
    bot->shouldsend = 1;

  return str;
}










char *
quiz_op_get (bot_t * bot, char *name, int recnum)
{
  xdb_pair_t *pair = NULL, *pair_2 = NULL;
  dlist_t *dptr;
  DB *db = NULL;
  char *str = NULL;
  int count;

  debug (bot, "quiz_op_get: Entered: %s, %i\n", name, recnum);

  if (!bot || !name || recnum < 0)
    {
      return NULL;
    }

  if (!dl_quiz_pairs)
    return NULL;

  if (!strcmp (name, "*"))
    {
      dptr = dlist_node_rand (dl_quiz_pairs);
      if (!dptr)
	goto cleanup;

      pair = (xdb_pair_t *) dlist_data (dptr);
      if (!pair)
	goto cleanup;
    }
  else
    {
      dlist_fornext (dl_quiz_pairs, dptr)
      {
	pair = (xdb_pair_t *) dlist_data (dptr);
	if (!pair)
	  continue;
	if (!pair->key)
	  continue;
	if (!strcasecmp (pair->key, name))
	  {
	    break;
	  }
	pair = NULL;
      }
    }

  if (!pair)
    goto cleanup;

  db = xdb_open (pair->value);
  if (!db)
    return NULL;

  db->stat_print (db, 0);

  count = xdb_count (db);

  pair_2 = xdb_get_recnum (db, recnum);

  str = str_unite ("%s:::%s", pair_2->key, pair_2->value);

cleanup:
  if (db)
    xdb_fini (db);

  if (pair_2)
    xdb_pair_destroy (pair_2);

  return str;
}



char *
quiz_op_del (bot_t * bot, char *key)
{

  debug (bot, "quiz_op_del: Entered\n");

  return NULL;
}



char *
quiz_op_add (bot_t * bot, char *string)
{

  debug (bot, "quiz_op_add: Entered\n");

  return NULL;
}









void
quiz_add_files_to_db (void)
{
  xdb_pair_t *pair;
  dlist_t *dl, *dptr, *dptr_tmp;
  char *dir;
  char *suffix, *str, *str_tmp;

  debug (NULL, "quiz_add_files_to_db: Entered\n");


  dir = str_unite ("%s/mods/mod_quiz_files/", gi->confdir);

  dl =
    files_get_listing (NULL, dir, FILES_NORMAL | FILES_RECURSE | FILES_NODOT);
  if (!dl)
    goto cleanup;

  dlist_fornext_retarded (dl, dptr, dptr_tmp)
  {
    if (!dptr)
      break;

    str = (char *) dlist_data (dptr);
    if (!str)
      continue;

    suffix = strrchr (str, '.');

    if (suffix)
      {
	if (!strcasecmp (suffix, ".qdb"))
	  {
	    dlist_remove_and_free (&dl, &dptr, free);
	    continue;
	  }
      }

    suffix = strrchr (str, '/');
    if (!suffix)
      continue;
    suffix++;

    pair = xdb_pair_create (suffix, strlen (suffix), str, strlen (str));
    if (!pair)
      continue;

    capsup (pair->key);
    str_tmp = pair->value;
    pair->value = str_unite ("%s.qdb", pair->value);
    free (str_tmp);
    dlist_Dinsert_after (&dl_quiz_pairs, pair);
  }


  dlist_fornext (dl, dptr)
  {
    debug (NULL, "quiz_add_files_to_db: file=%s\n",
	   (char *) dlist_data (dptr));
    quiz_add_files_to_db_file ((char *) dlist_data (dptr));
  }

cleanup:
  if (dl)
    files_destroy (NULL, &dl);

  if (dir)
    free (dir);

  return;
}



void
quiz_add_files_to_db_file (char *name)
{
  DB *db = NULL;
  FILE *fp;
  char *name_db;
  char buf[1024], buf_inside[MAX_BUF_SZ];

  char *tok_1 = NULL, *tok_2 = NULL;
  int tok_1_len;

  int count = 0;
  int inside = 0;



  if (!name)
    return;

  name_db = strrchr (name, '/');
  if (!name_db)
    return;

  fp = fopen (name, "r");
  if (!fp)
    return;

  name_db = str_unite_static ("%s.qdb", name);

  unlink (name_db);

  db = xdb_open (name_db);

  debug (NULL, "db=%p\n", db);

  if (!db)
    goto cleanup;

  while (1)
    {
      memset (buf, 0, sizeof (buf));

      if (!inside)
	memset (buf_inside, 0, sizeof (buf_inside));

      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;
      strstrip_nl (buf);

      if (buf[0] == '#')
	continue;

      if (strlen (buf) == 0)
	continue;

      if (!strstr (buf, "+++") && !inside)
	{
	  tok_1 = eat_whitespace (buf);

	  tok_2 = strstr (tok_1, ":::");
	  if (!tok_2)
	    continue;

	  tok_1_len = (tok_2 - tok_1);
	  tok_2 = tok_2 + 3;
	  tok_2 = eat_whitespace (tok_2);
	  tok_1 = strdup_len (tok_1, tok_1_len);
	  xdb_write (db, tok_1, tok_2);

	  free (tok_1);

	  count++;
	  continue;
	}

      if (strstr (buf, "+++"))
	{
/* we're beginning or ending a multi line entry */

	  if (inside)
	    {
/* end.. */
	      inside = 0;

	      xdb_write (db, tok_1, buf_inside);
	      free (tok_1);
	      tok_1 = NULL;
	      count++;
	      memset (buf_inside, 0, sizeof (buf_inside));

	    }
	  else
	    {
/* beginning */

	      tok_1 = eat_whitespace (buf);

	      tok_2 = strstr (tok_1, ":::");
	      if (!tok_2)
		{
		  tok_1 = NULL;
		  continue;
		}

	      tok_1_len = (tok_2 - tok_1);
	      tok_1 = strdup_len (tok_1, tok_1_len);

	      inside = 1;

	    }
	}
      else
	{
/* append to buf_inside */
	  strlcatfmt_buf (buf_inside, "%s\n", buf);
	}


    }

cleanup:
  if (fp)
    fclose (fp);

  if (db)
    xdb_fini (db);

  return;
}


void
quiz_clear_db_from_files (void)
{

  debug (NULL, "quiz_clear_db_from_files: Entered : CLEARING\n");

  return;
}




char *
quiz_op_list (bot_t * bot)
{
  xdb_pair_t *pair = NULL;
  dlist_t *dptr;
  char buf[MAX_BUF_SZ + 1], *str = NULL;

  debug (NULL, "quiz_op_list: Entered\n");

  if (!bot)
    return NULL;

  memset (buf, 0, sizeof (buf));

  dlist_fornext (dl_quiz_pairs, dptr)
  {
    pair = (xdb_pair_t *) dlist_data (dptr);

    if (!pair)
      continue;

    strlcatfmt_buf (buf, "%s ", pair->key);
  }

  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}




char *
quiz_op_answer (bot_t * bot, quiz_active_t * qa, char *answer)
{
  char *str = NULL;


  debug (bot, "quiz_op_answer: Entered, %s\n", answer);

  if (!bot || !qa || !answer)
    return NULL;

  str = quiz_active_answer (bot, qa, answer);

  return str;
}






char *
quiz_op_info (bot_t * bot, quiz_active_t * qa)
{

  dlist_t *dptr;
  quiz_winner_t *qw = NULL;
  char *topic_str = NULL, *str = NULL;
  char buf[1024];

  debug (bot, "quiz_op_info: Entered\n");

  if (!bot || !qa)
    return NULL;

  memset (buf, 0, sizeof (buf));
  strlcatfmt_buf (buf,
		  "quiz id: %i\n channel: %s , timeout: %i , size: %i\n",
		  qa->ID, bot->txt_to, qa->timeout, qa->size);
  strlcatfmt_buf (buf,
		  " multi: %i , newlines: %i , hidekey: %i , fixkey=%i\n",
		  qa->multi, qa->newlines, qa->hidekey, qa->fixkey);
  strlcat_bot (buf, " topics: ");
  dlist_fornext (qa->dl_topics, dptr)
  {
    topic_str = (char *) dlist_data (dptr);

    if (!topic_str)
      continue;

    strlcatfmt_buf (buf, "%s ", topic_str);
  }
  strlcatfmt_buf (buf,
		  "\n correct answers: %i , incorrect answers: %i , questions asked: %i\n",
		  qa->correct, qa->incorrect, qa->count);

  strlcat_bot (buf, " winners: ");

  dlist_fornext (qa->dl_winners, dptr)
  {
    qw = (quiz_winner_t *) dlist_data (dptr);
    if (!qw)
      continue;
    if (!qw->nick)
      continue;

    strlcatfmt_buf (buf, "%s(%i) ", qw->nick, qw->count);
    charcat_bot (buf, '\n');
  }


  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}




char *
quiz_op_setup (bot_t * bot, quiz_active_t ** qa_arg, char *string)
{
  quiz_active_t *qa = NULL;
  char *str = NULL, *str_previous_answer = NULL, *tstr = NULL;

  debug (NULL, "quiz_op_setup: Entered\n");

  if (!bot || !qa_arg)
    return NULL;

  if (*qa_arg)
    {
      str_previous_answer = quiz_active_answer_return (bot, *qa_arg);
    }

  qa = quiz_active_init (bot, string);
  if (!qa)
    {
      return NULL;
    }

  quiz_active_free (*qa_arg);

  *qa_arg = qa;

/* ask the first question */
  str = quiz_active_ask (bot, qa);

  if (str_previous_answer)
    {
      tstr = str;
      str = str_unite ("%s %s", str_previous_answer, str);
      free (tstr);
      free (str_previous_answer);
    }

  return str;
}



char *
quiz_op_clear (bot_t * bot, quiz_active_t * qa)
{
  char *str = NULL;


  debug (bot, "quiz_op_clear: Entered\n");

  if (!bot || !qa)
    return NULL;

  if (qa->dl_winners)
    {
      dlist_fini (&qa->dl_winners, quiz_winner_free);
    }

  qa->incorrect = 0;
  qa->correct = 0;
  qa->count = 0;

  return str;
}


char *
quiz_op_clear_winners (bot_t * bot, quiz_active_t * qa)
{
  return quiz_op_clear (bot, qa);
}



char *
quiz_op_timeout (bot_t * bot, quiz_active_t * qa, int timeout)
{
  char *str = NULL;

  if (timeout <= 0)
    timeout = 60;

  debug (bot, "quiz_op_timeout: Entered, timeout=%i\n", timeout);

  if (!bot || !qa)
    return NULL;

  qa->timeout = timeout;

  return str;
}


char *
quiz_op_size (bot_t * bot, char *string)
{
  int size;
  char *str = NULL;


  debug (bot, "quiz_op_size: Entered\n");

  if (!bot || !string)
    return NULL;

  size = quiz_pair_size (string);
  str = str_unite ("%i", size);

  return str;
}




char *
quiz_op_multi (bot_t * bot, quiz_active_t * qa, char *string)
{
  char *str = NULL;
  int on = 0;

  debug (bot, "quiz_op_multi: Entered\n");

  if (!bot || !qa || !string)
    return NULL;

  if (!strcasecmp (string, "on"))
    {
      on = 1;
    }
  else if (!strcasecmp (string, "off"))
    {
      on = 0;
    }
  else
    return NULL;

  qa->multi = on;

  return str;
}


char *
quiz_op_newlines (bot_t * bot, quiz_active_t * qa, char *string)
{
  char *str = NULL;
  int on = 0;

  debug (bot, "quiz_op_newlines: Entered\n");

  if (!bot || !string || !qa)
    return NULL;

  if (!strcasecmp (string, "on"))
    {
      on = 1;
    }
  else if (!strcasecmp (string, "off"))
    {
      on = 0;
    }
  else
    return NULL;

  qa->newlines = on;

  return str;
}


char *
quiz_op_hidekey (bot_t * bot, quiz_active_t * qa, char *string)
{
  char *str = NULL;
  int on = 0;

  debug (bot, "quiz_op_hidekey: Entered\n");

  if (!bot || !string || !qa)
    return NULL;

  if (!strcasecmp (string, "on"))
    {
      on = 1;
    }
  else if (!strcasecmp (string, "off"))
    {
      on = 0;
    }
  else
    return NULL;

  qa->hidekey = on;

  return str;
}



char *
quiz_op_fixkey (bot_t * bot, quiz_active_t * qa, char *string)
{
  char *str = NULL;
  int on = 0;

  debug (bot, "quiz_op_fixkey: Entered\n");

  if (!bot || !string || !qa)
    return NULL;

  if (!strcasecmp (string, "on"))
    {
      on = 1;
    }
  else if (!strcasecmp (string, "off"))
    {
      on = 0;
    }
  else
    return NULL;

  qa->fixkey = on;

  return str;
}




char *
quiz_op_winners (bot_t * bot, quiz_active_t * qa)
{

  dlist_t *dptr;
  quiz_winner_t *qw = NULL;
  char *str = NULL;
  int found = 0;
  char buf[1024];

  debug (bot, "quiz_op_info: Entered\n");

  if (!bot || !qa)
    return NULL;

  found = 0;
  memset (buf, 0, sizeof (buf));

  strlcatfmt_buf (buf,
		  "correct answers: %i , incorrect answers: %i , questions asked: %i\n",
		  qa->correct, qa->incorrect, qa->count);

  strlcat_bot (buf, "winners: ");

  dlist_fornext (qa->dl_winners, dptr)
  {
    qw = (quiz_winner_t *) dlist_data (dptr);
    if (!qw)
      continue;
    if (!qw->nick)
      continue;

    strlcatfmt_buf (buf, "%s(%i) ", qw->nick, qw->count);
    charcat_buf (buf, '\n');
  }

  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}




quiz_active_t *
quiz_active_init (bot_t * bot, char *string)
{
  dlist_t *dl = NULL, *dptr, *dptr_tmp, *dptr_pairs;
  quiz_active_t *qa = NULL;
  xdb_pair_t *pair = NULL;
  char *topic_str = NULL;
  int found = 0, add_all = 0;
  if (!bot || !string)
    return NULL;

  debug (bot, "quiz_active_init: Entered\n");

  found = 0;
  dl = tokenize (bot, string, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
  if (!dl)
    return NULL;

/* CHECK DL_ACTIVE FIRST, SEE IF THIS ID EXISTS, THEN ADD IF NOT, UPDATE OTHERWISE */


  dlist_fornext_retarded (dl, dptr, dptr_tmp)
  {

    if (!dptr)
      break;

    topic_str = dlist_data (dptr);
    if (!topic_str)
      continue;

    dlist_fornext (dl_quiz_pairs, dptr_pairs)
    {
      pair = (xdb_pair_t *) dlist_data (dptr_pairs);

      if (!strcasecmp (topic_str, "*"))
	{
	  add_all = 1;
	  found = 1;
	  continue;
	}

      if (!strcasecmp (pair->key, topic_str))
	{
	  found = 1;
	  break;
	}
    }

    if (add_all)
      break;

    if (!found)
      {
	dlist_remove (&dl, dptr);
	free (topic_str);
	free (dptr);
      }
    else
      {
/* add it */
      }

    found = 0;
  }

  if (add_all)
    {
      tokenize_destroy (bot, &dl);
      dl = NULL;
      dlist_fornext (dl_quiz_pairs, dptr)
      {
	pair = (xdb_pair_t *) dlist_data (dptr);
	if (!pair)
	  continue;
	if (!pair->key)
	  continue;
	dlist_Dinsert_after (&dl, strdup (pair->key));
      }
    }


  if (!dl)
    goto cleanup;

  if (dlist_size (dl) == 0)
    goto cleanup;

  if (qa == NULL)
    {
      qa = (quiz_active_t *) calloc (1, sizeof (quiz_active_t));
      if (!qa)
	goto cleanup;

      qa->dl_topics = dl;
      qa->ID = bot->ID;

      quiz_active_defaults (qa);
    }
  else
    {
/* already exists */
      tokenize_destroy (bot, &qa->dl_topics);
      qa->dl_topics = dl;
      quiz_active_defaults (qa);
    }

  return qa;




cleanup:

  if (dl)
    tokenize_destroy (bot, &dl);

  return qa;
}










char *
quiz_active_ask (bot_t * bot, quiz_active_t * qa)
{
  dlist_t *dl;
  DB *db;
  dlist_t *dptr;
  xdb_pair_t *pair = NULL, *pair_2 = NULL;
  char *str = NULL, *topic_str = NULL, *delim_str = " ", *tmp_str = NULL;
  int count, recnum;

  debug (bot, "quiz_active_ask: Entered\n");

  if (!bot || !qa)
    return NULL;

  dptr = dlist_node_rand (qa->dl_topics);
  topic_str = (char *) dlist_data (dptr);

  pair = xdb_pair_find_by_key (dl_quiz_pairs, topic_str);
  if (!pair)
    return NULL;

  db = xdb_open (pair->value);
  if (!db)
    return NULL;

  db->stat_print (db, 0);

  count = xdb_count (db);

  recnum = rand () % (count + 1);

  pair_2 = xdb_get_recnum (db, recnum);

  quiz_active_set_question (qa, pair_2);

  if (qa->newlines == 0)
    {
      str_shrink_chars (pair_2->value, "\n");
      str_shrink_spaces_to_one (pair_2->value);
    }

  if (str_has_newline (pair_2->value))
    {
      delim_str = "\n";
    }

  if (qa->fixkey)
    {
      int fixkey_sz = 0;
      dl =
	tokenize (bot, pair_2->key,
		  TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, "^^^");

      if (dl)
	{
	  int l_sum = 0;
	  fixkey_sz = dlist_size (dl);
	  dlist_t *dptr_tmp, *dptr_tmp_2;
	  dptr_tmp = dlist_tail (dl);
	  dlist_fornext_retarded (dl, dptr, dptr_tmp_2)
	  {
	    if (dptr == NULL)
	      break;
	    tmp_str = (char *) dlist_data (dptr);
	    tmp_str = str_unite ("%s^^^", tmp_str);
	    free ((char *) dlist_data (dptr));
	    /*  dlist_data (dptr) = strdup (tmp_str); */
	    dptr->data = strdup (tmp_str);
	    l_sum = str_sum (tmp_str);
	    str_shrink_chars_and_replace (tmp_str, "-", ' ');

	    if (l_sum != str_sum (tmp_str))
	      {
/* this means the string was modified, replaced with our chars */
		dlist_Dinsert_after (&dl, tmp_str);
	      }
	    else
	      {
		free (tmp_str);
	      }

	    if (dptr_tmp == dptr)
	      break;
	  }
	  if (dlist_size (dl) != fixkey_sz)
	    {
/* means we added nodes, make this the new key */

	      tmp_str = dlist_to_str (dl);
	      if (tmp_str)
		{
		  free (pair_2->key);
		  pair_2->key = tmp_str;
		}

	    }
	  dlist_fini (&dl, free);

	}

    }


  if (qa->hidekey)
    {
      dl =
	tokenize (bot, pair_2->key,
		  TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, "^^^");
      if (dl)
	{

	  dlist_fornext (dl, dptr)
	  {
	    tmp_str = (char *) dlist_data (dptr);
	    str_shrink_chars_and_replace (pair_2->value, tmp_str, '_');
	  }
	  dlist_fini (&dl, free);
	}

    }

  str =
    str_unite ("QUIZ (%s) question =>%s%s", topic_str, delim_str,
	       pair_2->value);

  qa->count++;

  xdb_fini (db);

  return str;
}



void
quiz_active_set_question (quiz_active_t * qa, xdb_pair_t * pair)
{

  if (!qa || !pair)
    return;

  if (qa->question)
    xdb_pair_destroy (qa->question);

  qa->question = pair;

  time (&qa->start);

  return;
}






void
quiz_winner_free (void *arg)
{
  quiz_winner_t *qw;

  qw = (quiz_winner_t *) arg;

  if (!qw)
    return;

  if (qw->nick)
    free (qw->nick);

  free (qw);

  return;
}

void
quiz_active_free (void *arg)
{
  quiz_active_t *qa;

  qa = (quiz_active_t *) arg;

  if (!qa)
    return;


  if (qa->dl_topics)
    {
      dlist_fini (&qa->dl_topics, free);
    }

  if (qa->dl_winners)
    {
      dlist_fini (&qa->dl_winners, quiz_winner_free);
    }

  if (qa->question)
    xdb_pair_destroy (qa->question);

  memset (qa, 0, sizeof (quiz_active_t));

  free (qa);

  return;

}





char *
quiz_active_answer_return (bot_t * bot, quiz_active_t * qa)
{
  char *str = NULL;
  char saved_key[512];

  debug (bot, "quiz_active_answer_return\n");


  if (!bot || !qa)
    return NULL;

  strlcpy_buf (saved_key, qa->question->key);

  str = str_unite ("previous answer was [%s]!", saved_key);

  return str;
}






char *
quiz_active_answer (bot_t * bot, quiz_active_t * qa, char *answer)
{
  dlist_t *dl, *dptr;
  char *tok, *str = NULL, *new_str = NULL;
  time_t t;
  char saved_key[512];

  debug (bot, "quiz_active_answer: %s\n", answer);


  if (!bot || !qa || !answer)
    return NULL;

  dl =
    tokenize (NULL, qa->question->key,
	      TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, "^^^");
  if (!dl)
    return NULL;


  time (&t);

  dlist_fornext (dl, dptr)
  {
    tok = (char *) dlist_data (dptr);
    if (!strcasecmp (tok, answer))
      {
	quiz_active_winner (bot, qa, bot->txt_nick);
	str = quiz_active_ask (bot, qa);
	if (str)
	  {
	    new_str = str_unite ("winner is %s! NEXT %s", bot->txt_nick, str);
	    free (str);
	  }
	tokenize_destroy (bot, &dl);
	return new_str;
      }
  }

/* INCORRECT ! */
  quiz_active_incorrect (bot, qa, bot->txt_nick);

/* timeout processing */
  if ((t - (qa->start)) > qa->timeout)
    {
      strlcpy_buf (saved_key, qa->question->key);
      str = quiz_active_ask (bot, qa);
      if (str)
	{
	  new_str =
	    str_unite ("no winners. previous answer was [%s]! NEXT %s",
		       saved_key, str);
	  free (str);
	}
      tokenize_destroy (bot, &dl);
      return new_str;
    }


  tokenize_destroy (bot, &dl);
  return str;
}





void
quiz_active_winner (bot_t * bot, quiz_active_t * qa, char *nick)
{
  dlist_t *dptr;
  quiz_winner_t *qw = NULL;

  if (!bot || !qa || !nick)
    return;

  dlist_fornext (qa->dl_winners, dptr)
  {
    qw = (quiz_winner_t *) dlist_data (dptr);
    if (!qw)
      continue;
    if (!qw->nick)
      continue;

    if (!strcasecmp (qw->nick, nick))
      {
	qw->count++;
	qa->correct++;
	return;
      }

  }

  qw = (quiz_winner_t *) calloc (1, sizeof (quiz_winner_t));
  if (!qw)
    return;

  qw->nick = strdup (nick);
  qw->count++;

  dlist_Dinsert_after (&qa->dl_winners, qw);

  qa->correct++;

  return;
}



void
quiz_active_incorrect (bot_t * bot, quiz_active_t * qa, char *nick)
{

  if (!bot || !qa || !nick)
    return;

  qa->incorrect++;

  return;
}




int
quiz_active_size (quiz_active_t * qa, char *key)
{
  dlist_t *dptr;
  DB *db;
  xdb_pair_t *pair;
  size_t tot = 0;

  char *topic_str;

  int count = 0;

  if (!qa)
    return 0;
  dlist_fornext (qa->dl_topics, dptr)
  {
    topic_str = (char *) dlist_data (dptr);
    pair = xdb_pair_find_by_key (dl_quiz_pairs, topic_str);
    if (!pair)
      continue;

    if (key)
      {
	if (strcasecmp (pair->key, key))
	  continue;
      }
    db = xdb_open (pair->value);
    if (!db)
      continue;

    count = xdb_count (db);

    if (count > 0)
      tot += count;

    xdb_fini (db);
  }
  return tot;
}





int
quiz_pair_size (char *key)
{
  dlist_t *dptr;
  DB *db;
  xdb_pair_t *pair;
  size_t tot = 0;

  int count = 0;

  dlist_fornext (dl_quiz_pairs, dptr)
  {
    pair = (xdb_pair_t *) dlist_data (dptr);
    if (!pair)
      continue;
    if (!pair->key || !pair->value)
      continue;

    if (key)
      {
	if (strcasecmp (pair->key, key))
	  continue;
      }
    db = xdb_open (pair->value);
    if (!db)
      continue;

    count = xdb_count (db);

    if (count > 0)
      tot += count;

    xdb_fini (db);
  }
  return tot;
}






void
quiz_active_defaults (quiz_active_t * qa)
{
  if (!qa)
    return;

  qa->timeout = 60;
  qa->size = quiz_active_size (qa, NULL);
  qa->multi = 1;
  qa->newlines = 1;
  qa->hidekey = 1;
  qa->fixkey = 1;

  return;
}
