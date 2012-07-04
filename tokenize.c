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

dlist_t *
tokenize (bot_t * bot, char *string, int opt, char *sep)
{
  dlist_t *dl, *dptr;
  char *str_a, *str_b;
  int str_len, need_break = 0;
  char *sep_orig;
  char buf[1024], *buf_dup;

  int sep_len, sep_prefix_len;

  if (!sNULL(string) || !opt || !sNULL(sep))
    return NULL;

  sep_len = strlen (sep);

  debug (bot, "tokenize: Entered, [\n[%s]\n]\n", string);

  dl = dptr = NULL;

  str_len = fn_strlen (string);
  sep_orig = sep;

  if (opt & TOKENIZE_EATWHITESPACE)
    str_a = eat_whitespace (string);
  else
    str_a = string;

  str_b = str_a;
  while (1)
    {
      sep_prefix_len = 0;
      sep = sep_orig;
      bz(buf);

      if (opt & TOKENIZE_EATWHITESPACE)
	str_a = eat_whitespace (str_a);

      if (!(opt & TOKENIZE_LEAVEQUOTES))
	{
	  if (*str_a == '\"')
	    {
	      sep = "\"";
	      sep_prefix_len = 1;
	    }
	}

      if (opt & TOKENIZE_IGNORECASE)
	{
	  str_b =
	    strcasestr (sep != sep_orig ? str_a + sep_prefix_len : str_a,
			sep);
	}
      else if (opt & TOKENIZE_MATCHANY)
	{
	  str_b =
	    strchr_str (sep != sep_orig ? str_a + sep_prefix_len : str_a, sep,
			&sep_len);
	}
      else
	{
	  str_b =
	    strstr (sep != sep_orig ? str_a + sep_prefix_len : str_a, sep);
	}

      if (!str_b)
	{
	  str_b = string + fn_strlen (string);
	  need_break = 1;
	}

      if (opt & TOKENIZE_NORMAL)
	{


	  if (sep != sep_orig)
	    str_b++;

	  memcopy (buf, str_a, sizeof (buf) - 1,
		   (opt & TOKENIZE_LEAVESEP) ? ((str_b + sep_len) -
						    str_a) : (str_b - str_a));


	  if (fn_strlen (buf) > 0)
	    {
	      buf_dup = strdup (buf);
	      if (buf_dup)
		{
		  dlist_Dinsert_after (&dl, buf_dup);
		}
	    }

	  str_b += sep_len;

	}

      str_a = str_b;

      if (need_break)
	break;
    }

  dlist_fornext (dl, dptr)
  {
    str_a = (char *) dlist_data (dptr);

    if (!(opt & TOKENIZE_LEAVEQUOTES))
      {
	str_shrink_quotes (str_a);
      }

  }

  return dl;
}




void
tokenize_destroy (bot_t * bot, dlist_t ** dl)
{


  if (!dl)
    return;

  dlist_fini (dl, free);

  return;
}





char **
tokenize_array (bot_t * bot, char *string, int opt, char *sep,
		    int *expecting)
{
  dlist_t *dl, *dptr;
  char **array, *str_ptr;
  int array_size, i;


  array = NULL;
  array_size = 0;

  if (!string || !sep || !expecting)
    return NULL;

  dl = tokenize (bot, string, opt, sep);
  if (!dl)
    return NULL;

  array_size = dlist_size (dl);
  if (*expecting > 0)
    {
      array_size = *expecting;
    }
  else
    *expecting = array_size;

  array = (char **) calloc (array_size + 1, sizeof (char *));
  if (!array)
    goto cleanup;


  i = 0;
  dlist_fornext (dl, dptr)
  {
    str_ptr = (char *) dlist_data (dptr);
    if (!str_ptr)
      continue;
    if (i > array_size)
      break;
    array[i] = strdup (str_ptr);
    i++;
  }

cleanup:
  if (dl)

    tokenize_destroy (bot, &dl);

  return array;
}




void
tokenize_destroy_array (bot_t * bot, char **array)
{
  int i;

  if (!array)
    return;

  for (i = 0; array[i] != NULL; i++)
    {
      if (array[i])
	free (array[i]);
    }

  free (array);

  return;
}






char *
tokenize_find_closing_bracket (char *str, int c)
{
  int p_cnt, p_cnt_tog;
  int c_open, c_close;

  if (!str)
    return NULL;


  if (c == '(' || c == ')')
    {
      c_open = '(';
      c_close = ')';
    }
  else if (c == '{' || c == '}')
    {
      c_open = '{';
      c_close = '}';
    }
  else if (c == '[' || c == ']')
    {
      c_open = '[';
      c_close = ']';
    }
  else
    return NULL;

  p_cnt = p_cnt_tog = 0;
  while (*str)
    {
      if (*str == c_open)
	{
	  p_cnt++;
	  p_cnt_tog++;
	}
      else if (*str == c_close)
	p_cnt--;

      if (p_cnt < 0)
	return NULL;

      if (p_cnt == 0 && p_cnt_tog)
	return str;

      str++;
    }

  return NULL;
}






char *
tokenize_find_inside_of_brackets (char *str, int c)
{
  int p_cnt, p_cnt_tog;
  int c_open, c_close;

  char *str_begin = NULL;

  if (!str)
    return NULL;


  if (c == '(' || c == ')')
    {
      c_open = '(';
      c_close = ')';
    }
  else if (c == '{' || c == '}')
    {
      c_open = '{';
      c_close = '}';
    }
  else if (c == '[' || c == ']')
    {
      c_open = '[';
      c_close = ']';
    }
  else if (c == '\"')
    {
      c_open = '\"';
      c_close = '\"';
    }

  else
    return NULL;

  p_cnt = p_cnt_tog = 0;
  while (*str)
    {
      if (*str == c_open)
	{
	  p_cnt++;
	  p_cnt_tog++;
	  str_begin = str;
	}
      else if (*str == c_close)
	p_cnt--;

      if (p_cnt < 0)
	return NULL;

      if (p_cnt == 0 && p_cnt_tog)
	{
	  return strdup_len (str_begin, str - str_begin);
	}


      str++;
    }

  return NULL;
}









char ** tokenize_str2argv(char * string, int *argc, int opt) {
dlist_t * dl=NULL;
char **argv=NULL;

debug(NULL, "tokenize_str2argv: Entered\n");

if(!string || !argc) return NULL;

/*
*argc = 0;

argv = (char **) tokenize_array(NULL, string, TOKENIZE_NORMAL, " ", argc);
*/
dl = tokenize(NULL, string, TOKENIZE_NORMAL, " ");
if(!dl) goto cleanup;

if(opt & TOKENIZE_STR2ARGV_ARGV0) {
dlist_Dinsert_before(&dl, strdup("argv0"));
}

argv = (char **) dlist_convert_dlist_to_array_new(dl);
if(!argv) goto cleanup;

*argc = dlist_size(dl);

cleanup:
if(dl) tokenize_destroy(NULL, &dl);

return argv;
}
