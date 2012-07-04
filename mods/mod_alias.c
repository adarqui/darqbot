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
#include "mod_alias.h"


void
__alias_init__ (void)
{

strlcpy_buf(mod_alias_info.name, "mod_alias");
strlcpy_buf(mod_alias_info.trigger, "^alias");

module_add_subtrigs(&mod_alias_info, "^a");

  mod_alias_info.init = alias_init;
  mod_alias_info.fini = alias_fini;
  mod_alias_info.help = alias_help;
  mod_alias_info.run = alias_run;


  mod_alias_info.output = NULL;
  mod_alias_info.input = NULL;


  debug (NULL, "__alias_init__: Loaded mod_alias\n");

  return;
}



bot_t *
alias_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "alias_init: Entered\n");

  return NULL;
}

bot_t *
alias_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "alias_fini: Entered\n");

  return NULL;
}

bot_t *
alias_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "alias_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^alias || ^alias(get) <key> || ^alias(add) <key> || ^alias(del) <key> || ^alias(getmore) <key> || ^a <key>";

  return NULL;
}

bot_t *
alias_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "alias_run: Entered\n");


  if (!dlist_node || !bot)
    return NULL;


  if (gi->mongo_isconnected == 0)
    {
      debug (bot, "alias_run: Mongo is not connected\n");
      return NULL;
    }

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "alias_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;

  if (!strcasecmp (bot->trig_called, "^a"))
    {
      opt = ALIAS_OPT_RUN;
    }
  else
    {
      opt = ALIAS_OPT_GET;
    }

  MOD_OPTIONS_TOP_HALF;

  if (xstrcasestr_len (dl_options_ptr, "run", len_options_area))
    {
      opt = ALIAS_OPT_RUN;
    }
  else if (xstrcasestr_len (dl_options_ptr, "get", len_options_area))
    {
      opt = ALIAS_OPT_GET;

    }
  else if (xstrcasestr_len (dl_options_ptr, "add", len_options_area))
    {
      opt = ALIAS_OPT_ADD;
    }

  else if (xstrcasestr_len (dl_options_ptr, "del", len_options_area))
    {
      opt = ALIAS_OPT_DEL;
    }

  if (xstrcasestr_len (dl_options_ptr, "getmore", len_options_area))
    {

      opt = ALIAS_OPT_GETMORE;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = alias_change_string (dlist_node, bot, opt, l_str_ptr);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}





char *
alias_change_string (dlist_t * dlist_node, bot_t * bot, int opt, char *string)
{
  char *str = NULL;

  char *tok_1, *tok_1_dup, *tok_2 = NULL;
  int o, c;

  debug (bot, "alias_change_string: Entered, string=%s\n", string);

  if (!dlist_node || !bot)
    return NULL;

  environ_add_args1 (bot, string, bot->trig_called);
  environ_debugprint (bot);

  o = strlen (string);

  tok_1 = strtokbionic (string, " ", &c);
  if (!tok_1)
    return NULL;

  tok_1_dup = strdup (tok_1);
  if (!tok_1_dup)
    goto cleanup;

  tok_1[strlen (tok_1)] = c;

  if (!str_isclean (tok_1_dup, iskeychars))
    {
      debug (bot, "alias_run: Key contains illegal chars, returning\n");
      goto cleanup;
    }

  capsup (tok_1_dup);

  tok_2 = strtokbionic (NULL, "", &c);

  switch (opt)
    {
    case ALIAS_OPT_RUN:
      {
	str = alias_op_run (dlist_node, bot, tok_1_dup);
	break;
      }
    case ALIAS_OPT_GET:
      {
	str = alias_op_get (bot, tok_1_dup);
	break;
      }
    case ALIAS_OPT_DEL:
      {
	str = alias_op_del (bot, tok_1_dup);
	break;
      }
    case ALIAS_OPT_ADD:
      {
	str = alias_op_add (bot, tok_1_dup, tok_2);
	break;
      }
    case ALIAS_OPT_GETMORE:
      {
	str = alias_op_getmore (bot, tok_1_dup);
	break;
      }
    default:
      break;
    }


cleanup:
  if (tok_1_dup)
    free (tok_1_dup);

  return str;
}




char *
alias_op_run (dlist_t * dlist_node, bot_t * bot, char *string)
{
  dlist_t *dptr;
  char *ptr, *str = NULL, *new_str_subbed = NULL;

  debug (NULL, "alias_op_run: Entered, string=%s\n", string);

  if (!bot || !string)
    return NULL;


  str = mongodb_retrieve_key (bot, "test.darqbotalias", string, "value");

  if (!str)
    return NULL;

  new_str_subbed = environ_sub_envs (bot, str);
  if (!new_str_subbed)
    {
      goto cleanup;
    }


  str_untransform_pipes (str);

bz(bot->txt_data_in);
strlcat_bot(bot->txt_data_in,bot->txt_data_out);
  strlcat_bot (bot->txt_data_in, new_str_subbed);

  dlist_fornext (dlist_next (dlist_node), dptr)
  {
    ptr = (char *) dlist_data (dptr);
    charcat_bot (bot->txt_data_in, '|');
    strlcat_bot (bot->txt_data_in, ptr);
  }

  printf ("\n\nsuperout=[%s]\n\n", bot->txt_data_in);

  pmodule_cur_run2 (bot);

  bz (bot->txt_data_in);

  if (str)
    free (str);

  return NULL;

cleanup:
  if (new_str_subbed)
    free (new_str_subbed);

  if (str)
    free (str);

  return NULL;
}

char *
alias_op_get (bot_t * bot, char *string)
{
  char *str = NULL;

  if (!bot || !string)
    return NULL;


  str = mongodb_retrieve_key (bot, "test.darqbotalias", string, "value");

  if (!str)
    return NULL;

  str_untransform_pipes (str);

  return str;
}

char *
alias_op_del (bot_t * bot, char *string)
{
  char *str = NULL;

  if (!bot || !string)
    return NULL;

  mongodb_remove_key (bot, "test.darqbotalias", string);

  return str;
}

char *
alias_op_add (bot_t * bot, char *string, char *value)
{
  char *str = NULL;


  debug (NULL, "alias_op_add: Entered, string=%s, value=%s\n", string,
	     value);

  if (!bot || !string || !value)
    return NULL;


  str_transform_pipes (value);
  mongodb_insert_key (bot, "test.darqbotalias", string, value,
			"%s!%s@%s!%s", bot->txt_nick, bot->txt_ident,
			bot->txt_host, bot->txt_to);

  return str;
}

char *
alias_op_getmore (bot_t * bot, char *string)
{
  char *str = NULL;

  if (!bot || !string)
    return NULL;

  str = mongodb_retrieve_key (bot, "test.darqbotalias", string, "comment");

  if (!str)
    return NULL;

  str_untransform_pipes (str);

  return str;
}
