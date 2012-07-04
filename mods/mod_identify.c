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
#include "mod_identify.h"

void
__identify_init__ (void)
{

strlcpy_buf(mod_identify_info.name, "mod_identify");
strlcpy_buf(mod_identify_info.trigger, "^identify");

  mod_identify_info.init = identify_init;
  mod_identify_info.fini = identify_fini;
  mod_identify_info.help = identify_help;
  mod_identify_info.run = identify_run;


  mod_identify_info.output = NULL;
  mod_identify_info.input = NULL;


  debug (NULL, "__identify_init__: Loaded mod_identify\n");

  return;
}



bot_t *
identify_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "identify_init: Entered\n");

  return NULL;
}

bot_t *
identify_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "identify_fini: Entered\n");

  return NULL;
}

bot_t *
identify_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "identify_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^identify || ^identify(get) <key> || ^identify(add) <key> || ^identify(del) <key> || ^identify(getmore) <key> || ^a <key>";

  return NULL;
}

bot_t *
identify_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "identify_run: Entered\n");

  if (!bot)
    return NULL;

  if (gi->mongo_isconnected == 0)
    {
      debug (bot, "identify_run: Mongo is not connected\n");
      return NULL;
    }

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "identify_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;

  opt = IDENTIFY_OPT_RUN;

  MOD_OPTIONS_TOP_HALF;

  if (xstrcasestr_len (dl_options_ptr, "run", len_options_area))
    {
      opt = IDENTIFY_OPT_RUN;
    }
  else if (xstrcasestr_len (dl_options_ptr, "get", len_options_area))
    {
      opt = IDENTIFY_OPT_GET;

    }
  else if (xstrcasestr_len (dl_options_ptr, "add", len_options_area))
    {
      opt = IDENTIFY_OPT_ADD;
    }

  else if (xstrcasestr_len (dl_options_ptr, "del", len_options_area))
    {
      opt = IDENTIFY_OPT_DEL;
    }

  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = identify_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;

}









char *
identify_change_string (bot_t * bot, char *string, int opt)
{
  dlist_t *dl = NULL;
  char **arg_array = NULL;
  char *str = NULL;
  char buf[MAX_BUF_SZ];
  char *sep_ptr;

  debug (bot, "identify_change_string: Entered\n");

  if (!bot || !string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

bz(buf);

  if (opt == IDENTIFY_OPT_RUN)
    {
      return identify_op_run (bot);
    }

  dl =
    tokenize (bot, string,
		  TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
  if (!dl)
    return NULL;

  arg_array = (char **) dlist_convert_dlist_to_array (dl);
  if (!arg_array && opt != IDENTIFY_OPT_RUN)
    goto cleanup;

  if (!str_isclean (arg_array[0], iskeychars))
    {
      debug (bot,
		 "identify_run: Key contains illegal chars, returning\n");
      goto cleanup;
    }

  capsup (arg_array[0]);

  switch (opt)
    {
    case IDENTIFY_OPT_GET:
      {
	str = identify_op_get (bot, arg_array[0]);
	break;
      }
    case IDENTIFY_OPT_DEL:
      {
	str = identify_op_del (bot, arg_array[0]);
	break;
      }
    case IDENTIFY_OPT_ADD:
      {
	if (!arg_array[0])
	  break;
	if (!arg_array[1])
	  break;
	str = identify_op_add (bot, arg_array[0], arg_array[1]);
	break;
      }
    default:
      break;
    }

cleanup:

  if (dl)
    tokenize_destroy (bot, &dl);

  if (arg_array)
    {
      free (arg_array);
    }

  return str;
}









char *
identify_change_string_fn_null (bot_t * bot, char *string, int opt)
{
  return NULL;
}





char *
identify_return_user (dlist_t * dlist_node, bot_t * bot)
{

  bot_t *bot_ptr, bot_tmp;

  if (!bot)
    return NULL;

  memcpy (&bot_tmp, bot, sizeof (bot_t));
  memset (bot_tmp.txt_data_in, 0, sizeof (bot_tmp.txt_data_in));
  memset (bot_tmp.txt_data_out, 0, sizeof (bot_tmp.txt_data_out));
  bot_tmp.dl_module_arg = bot_tmp.txt_data_in;

  bot_ptr = identify_run (dlist_node, &bot_tmp);

  if (bot_tmp.txt_data_out[0])
    return strdup (bot_tmp.txt_data_out);

  return NULL;
}









char *
identify_op_get (bot_t * bot, char *key)
{
  char *str = NULL;
  bson *b;

  debug (bot, "identify_op_get: Entered\n");

  b =
    mongodb_bson_get (bot, "test.darqbotidentify", "key",
			MONGODB_ARG_STRING, (char *) key);

  if (!b)
    return NULL;

  str = mongodb_bson_print_init (b);

  bson_destroy (b);

  return str;
}



char *
identify_op_del (bot_t * bot, char *key)
{
  if (!bot || !key)
    return NULL;

/* del a key & value pair */
  mongodb_remove_key (bot, "test.darqbotidentify", key);
  return NULL;
}



char *
identify_op_run (bot_t * bot)
{
  dlist_t *dl, *dl_tok, *dptr, *dptr2;
  module_t *mod;
  keyvalue_t *kv;
  bot_t tmp_bot;
  char *(*pcre_change_string_fn) (bot_t *, char *, int);
  char *match_str, *pcre_str, *str = NULL;
  int found = 0;


  debug (bot, "identify_op_run: Entered\n");

  mod = module_find_by_name ("mod_pcre");
  if (!mod)
    return NULL;
  if (!mod->dl_handle)
    return NULL;

  dl = mongodb_dump_keyvalue (bot, "test.darqbotidentify");
  if (!dl)
    {
      return NULL;
    }

  pcre_change_string_fn = dlsym (mod->dl_handle, "pcre_change_string");
  if (!pcre_change_string_fn)
    pcre_change_string_fn = identify_change_string_fn_null;

  memset (&tmp_bot, 0, sizeof (tmp_bot));
  snprintf_buf (tmp_bot.txt_data_out,  "%s!%s@%s",
	    bot->txt_nick, bot->txt_ident, bot->txt_host);

  dlist_fornext (dl, dptr)
  {
    kv = (keyvalue_t *) dlist_data (dptr);
    if (!kv)
      continue;

    printf ("DL: kv->key=%s, kv->value=%s\n", kv->key, kv->value);

    dl_tok =
      tokenize (bot, kv->value,
		    TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, ",");
    if (!dl_tok)
      continue;

    printf ("YO: key=%s, value=%s\n", kv->key, kv->value);

    dlist_fornext (dl_tok, dptr2)
    {
      match_str = dlist_data (dptr2);
      printf ("identify checkin for mathc string=%s, checking=%s\n",
	      match_str, tmp_bot.txt_data_out);
      pcre_str = pcre_change_string_fn (&tmp_bot, match_str, 0);
      if (pcre_str)
	{
	  str = strdup (kv->key);
	  found = 1;
	  free (pcre_str);
	  break;
	}
      if (found)
	break;
    }
    if (found)
      break;
  }
  if (!found)
    {
      str = strdup ("unknown");
    }

  mongodb_destroy_keyvalue (bot, &dl);

  tokenize_destroy (bot, &dl_tok);

  return str;
}




char *
identify_op_add (bot_t * bot, char *key, char *value)
{
/* add a key/value pair */
  char *str;
  bson *b;
  debug (bot, "identify_op_add: Entered\n");

  str =
    str_unite_static ("%s!%s@%s!%s", bot->txt_nick, bot->txt_ident, bot->txt_host,
	       bot->txt_to);

  b =
    mongodb_bson_build (bot, "%s=key %s=value %s=comment", key, value, str);

  if (!b)
    return NULL;

  str = mongodb_bson_print_init (b);

  mongodb_bson_insert (bot, "test.darqbotidentify", b);

  bson_destroy (b);

  return NULL;
}
