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
#include "mod_moby.h"

void
__moby_init__ (void)
{

  strlcpy_buf (mod_moby_info.name, "mod_moby");
  strlcpy_buf (mod_moby_info.trigger, "^moby");

  mod_moby_info.init = moby_init;
  mod_moby_info.fini = moby_fini;
  mod_moby_info.help = moby_help;
  mod_moby_info.run = moby_run;


  mod_moby_info.output = NULL;
  mod_moby_info.input = NULL;

  debug (NULL, "__moby_init__: Loaded mod_moby\n");

  memset (&moby_info, 0, sizeof (moby_info));

  return;
}



void
moby_create_db (bot_t * bot)
{
  DB *db = NULL;
  FILE *fp = NULL;
  struct stat st;
  int count = 0;
  char *str, *str_db, *tok_1 = NULL, *tok_2 = NULL;
  char buf[10024];



/*
  str = str_unite_static ("%s/mods/mod_moby_files/syns.txt", gi->confdir);

  fp = fopen (str, "r");
  if (!fp)
    goto cleanup;
*/


  strlcpy_buf (moby_info.db_name,
	       str_unite_static ("%s/mods/mod_moby_files/syns.moby_db",
				 gi->confdir));
  str_db = moby_info.db_name;
  if (!stat (str_db, &st))
    {
/* db already exists */
      moby_info.initialized = 1;

      return;
    }

  str = str_unite_static ("%s/mods/mod_moby_files/syns.txt", gi->confdir);

  fp = fopen (str, "r");
  if (!fp)
    goto cleanup;


  db = xdb_open (str_db);
  if (!db)
    goto cleanup;

  while (1)
    {
      memset (buf, 0, sizeof (buf));

      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;

      str_apply (buf, tolower);

      strstrip_nl (buf);

      if (buf[0] == '#')
	continue;

      if (strlen (buf) == 0)
	continue;

      tok_1 = strtok (buf, ",");
      if (!tok_1)
	continue;

      tok_2 = strtok (NULL, "");
      if (!tok_2)
	continue;

      xdb_write (db, tok_1, tok_2);
      count++;
    }

  moby_info.initialized = 1;

  moby_info.count = count;

cleanup:

  if (fp)
    fclose (fp);

  if (db)
    xdb_fini (db);

  return;
}



bot_t *
moby_init (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "moby_init: Entered\n");

  moby_create_db (bot);

  return NULL;
}

bot_t *
moby_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "moby_fini: Entered\n");

  moby_info.count = 0;

  return NULL;
}

bot_t *
moby_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "moby_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^moby || ^moby(list) <keyword>";

  return NULL;
}

bot_t *
moby_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;

  int opt;


  debug (bot, "moby_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "moby_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;


  opt = MOBY_SINGLE;

  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "list"))
    opt = MOBY_LIST;

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF;
  l_new_str = moby_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}


char *
moby_get_syn (DB * db, char *tok, int opt)
{
  dlist_t *dl = NULL, *dptr = NULL;
  xdb_pair_t *pair = NULL;
  char *str = NULL;

  debug (NULL, "moby_get_syn: Entered\n");

  if (!tok || !moby_info.initialized)
    return NULL;

  str_apply (tok, tolower);

  pair = xdb_get (db, tok);
  if (!pair)
    return NULL;

  if (opt == MOBY_SINGLE)
    {
      dl =
	tokenize (NULL, pair->value,
		  TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, ",");
      if (!dl)
	goto cleanup;

      dptr = dlist_node_rand (dl);
      if (!dptr)
	goto cleanup;

      str = str_unite_static ("%s", (char *) dlist_data (dptr));
    }
  else if (opt == MOBY_LIST)
    {
      str = str_unite_static ("%s,%s", pair->key, pair->value);
    }


cleanup:

  if (dl)
    tokenize_destroy (NULL, &dl);

  xdb_pair_destroy (pair);

  return str;
}


char *
moby_change_string (char *string, int opt)
{
  DB *db = NULL;

  int i, j, tok_nonalpha_len, retchar;
  char *str, *syn, *tok_ptr, *tok_nonalpha;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  str = NULL;


  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));
  tok_ptr = string;

  db = xdb_open (moby_info.db_name);
  if (!db)
    {
      return NULL;
    }

  j = 0;
  while (1)
    {

      if (j > 0 && opt == MOBY_LIST)
	break;

      retchar = 0;
      tok_ptr = strtokbionic_nonalpha (tok_ptr, &retchar);
      if (!tok_ptr)
	break;

      if (strlen (tok_ptr))
	{

	  tok_nonalpha = tok_ptr + strlen (tok_ptr) + 1;
	  for (i = 0; i < strlen (tok_nonalpha); i++)
	    {
	      if (isalpha (tok_nonalpha[i]))
		break;
	    }

	  tok_nonalpha_len = i;


	  syn = moby_get_syn (db, tok_ptr, opt);
	  if (syn)
	    {
	      moby_transform_word (syn, tok_ptr);

	      strlcat_bot (buf, syn);
	    }
	  else
	    {
	      strlcat_bot (buf, tok_ptr);
	    }

	  if (retchar != 0)
	    {
	      charcat_bot (buf, retchar);
	      for (i = 0; i < tok_nonalpha_len; i++)
		{
		  charcat_bot (buf, tok_nonalpha[i]);
		}
	    }

	}


      tok_ptr = NULL;
      j++;
    }

  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}




int
moby_transform_word (char *word1, char *word2)
{
/* word1 = moby word, word2 = original */
  int i, caps;

  if (!word1 || !word2)
    return -1;

  caps = 0;
  for (i = 0; i < strlen (word2); i++)
    {
      if (isupper (word2[i]))
	caps++;
    }

  if (caps == 0)
    {
      while (*word1)
	{
	  *word1 = tolower (*word1);
	  word1++;
	}
      return 0;
    }

  if (isupper (word2[0]))
    word1[0] = toupper (word1[0]);


  if (strlen (word2) == caps)
    {
      while (*word1)
	{
	  *word1 = toupper (*word1);
	  word1++;
	}
    }

  return 0;
}
