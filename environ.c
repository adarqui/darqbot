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

bot_t *
environ_set (bot_t * bot, char *key, char *val)
{
  char *str;

  if (!bot || !key || !val)
    return NULL;

  str = str_unite ("%s=%s", key, val);
  if (!str)
    return NULL;

  dlist_Dinsert_after (&bot->dl_environ, str);

  return bot;
}




bot_t *
environ_set_withnum (bot_t * bot, int num, char *val)
{
  char num_buf[32];

  if (!bot || num < 0 || !val)
    return NULL;

  bz (num_buf);

  snprintf_buf (num_buf, "%i", num);

  return environ_set (bot, num_buf, val);
}




char *
environ_get (bot_t * bot, char *key)
{
  dlist_t *dptr;
  char *str, *tok_1;
  if (!bot || !key)
    return NULL;

  dlist_fornext (bot->dl_environ, dptr)
  {
    str = dlist_data (dptr);

    tok_1 = strchr (str, '=');
    if (!tok_1)
      continue;

    if (!strncasecmp (str, key, tok_1 - str))
      {
	return str;
      }

  }

  return NULL;
}


bot_t *
environ_unset (bot_t * bot, char *key)
{
  dlist_t *dptr;
  char *str, *tok_1;
  if (!bot || !key)
    return NULL;

  dlist_fornext (bot->dl_environ, dptr)
  {
    str = dlist_data (dptr);

    tok_1 = strchr (str, '=');
    if (!tok_1)
      continue;

    if (!strncasecmp (str, key, tok_1 - str))
      {
	dlist_remove (&bot->dl_environ, dptr);
	free (str);
	return bot;
      }

  }

  return NULL;
}



bot_t *
environ_destroy (bot_t * bot)
{

  if (!bot)
    return NULL;

  dlist_fini (&bot->dl_environ, free);

  return bot;
}



bot_t *
environ_destroy_args (bot_t * bot)
{
  dlist_t *dptr, *t;
  char *ptr;


  if (!bot)
    return NULL;

  debug (bot, "environ_destroy_args: Entered\n");

  dlist_fornext_retarded (bot->dl_environ, dptr, t)
  {
    if (!dptr)
      break;

    ptr = dlist_data (dptr);

    if (isdigit (ptr[0]) && ptr[1] == '=')
      {
	debug (bot, "environ_destroy_args: about to remove %s\n",
		   ptr);
	dlist_remove (&bot->dl_environ, dptr);
	free (ptr);
      }

  }


  environ_debugprint (bot);

  return NULL;
}



bot_t *
environ_add_args1 (bot_t * bot, char *string, char *trig)
{
  dlist_t *dl, *dptr;
  char *tok;
  int num;


  if (!bot || !string)
    return NULL;

  dl = tokenize (bot, string, TOKENIZE_NORMAL, " ");
  if (!dl)
    return NULL;

  num = 0;

  if (trig)
    {
      environ_set_withnum (bot, num, trig);
      num++;
    }

  dlist_fornext (dl, dptr)
  {
    if (num > ENVIRON_MAX_ARGV)
      break;
    tok = (char *) dlist_data (dptr);
    environ_set_withnum (bot, num, tok);
    num++;
  }

  tokenize_destroy (bot, &dl);

  return bot;
}






bot_t *
environ_debugprint (bot_t * bot)
{
  dlist_t *dptr;
  char *str;

  if (!bot)
    return NULL;

  debug (bot, "environ_debugprint: ENTERED\n");

  dlist_fornext (bot->dl_environ, dptr)
  {
    str = (char *) dlist_data (dptr);
    debug (bot, "environ_debugprint: env_var=%s\n", str);
  }

  return bot;
}







char *
environ_sub_envs (bot_t * bot, char *str)
{
  dlist_t *dptr;
  char *new_str = NULL, *ptr, *tok, *tok_2;
  char *argv[ENVIRON_MAX_ARGV + 2], buf[MAX_BUF_SZ + 1];
  int num, num_b;

/* ^a(add) testargv ^e $1 |^e $0 |^$2 */
  if (!bot || !str)
    return NULL;


  debug (bot, "environ_sub_envs: Entered\n");


  bz (argv);


  dlist_fornext (bot->dl_environ, dptr)
  {
    ptr = (char *) dlist_data (dptr);
    if (!ptr)
      continue;
    num = ptr[0];
    if (num <= '0' && num >= '9' && ptr[1] != '=')
      continue;

    num = ctoi (num);
    argv[num] = ptr + 2;
  }

  bz(buf);
  tok = str;
  while (1)
    {
      tok_2 = strstr (tok, "$$");
      if (!tok_2)
	{
	  strlcat_buf(buf, tok);
	  break;
	}

      num_b = tok_2[2];
      if (num_b >= 'A' && num_b <= 'I')
	{
/* all args */
	  int num_tmp;
	  num_b = num_b - 65;

	  *tok_2 = '\0';
	  strlcat_buf (buf, tok);

	  for (num_tmp = num_b; num_tmp < ENVIRON_MAX_ARGV; num_tmp++)
	    {
	      strlcat_buf (buf, argv[num_tmp]);
	      charcat_buf (buf, ' ');
	    }

	}
      else
	{
	  num = ctoi (num_b);
	  if (num < 0 || num > ENVIRON_MAX_ARGV)
	    continue;

	  *tok_2 = '\0';
	  strlcat_buf(buf, tok);
	  strlcat_buf(buf, argv[num]);
	}

      tok = tok_2 + 3;
    }

  if (sNULL(buf) != NULL)
    new_str = strdup (buf);
  return new_str;
}
