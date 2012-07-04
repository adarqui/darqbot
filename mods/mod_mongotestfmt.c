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
#include "mod_mongotestfmt.h"

void
__mongotestfmt_init__ (void)
{

/*
  mod_mongotestfmt_info.name = "mod_mongotestfmt";
  mod_mongotestfmt_info.trigger = strdup ("^mongotestfmt");
*/
  strlcpy_buf (mod_mongotestfmt_info.name, "mod_mongotestfmt");
  strlcpy_buf (mod_mongotestfmt_info.trigger, "^mongotestfmt");

  mod_mongotestfmt_info.init = mongotestfmt_init;
  mod_mongotestfmt_info.fini = mongotestfmt_fini;
  mod_mongotestfmt_info.help = mongotestfmt_help;
  mod_mongotestfmt_info.run = mongotestfmt_run;


  mod_mongotestfmt_info.output = NULL;
  mod_mongotestfmt_info.input = NULL;


  debug (NULL, "__mongotestfmt_init__: Loaded mod_mongotestfmt\n");

  return;
}



bot_t *
mongotestfmt_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mongotestfmt_init: Entered\n");
  return NULL;
}

bot_t *
mongotestfmt_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mongotestfmt_fini: Entered\n");
  return NULL;
}

bot_t *
mongotestfmt_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mongotestfmt_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^mongotestfmt";

  return NULL;
}

bot_t *
mongotestfmt_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "mongotestfmt_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "mongotestfmt_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "add"))
    {
      opt = MONGOTESTFMT_ADD;
    }
  else if (!strncasecmp_len (dl_options_ptr, "del"))
    {
      opt = MONGOTESTFMT_DEL;
    }
  else if (!strncasecmp_len (dl_options_ptr, "get"))
    {
      opt = MONGOTESTFMT_GET;
    }
  else if (!strncasecmp_len (dl_options_ptr, "update"))
    {
      opt = MONGOTESTFMT_UPDATE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "test"))
    {
      opt = MONGOTESTFMT_TEST;
    }
  else if (!strncasecmp_len (dl_options_ptr, "badd"))
    {
      opt = MONGOTESTFMT_ADDBSON;
    }
  else if (!strncasecmp_len (dl_options_ptr, "bget"))
    {
      opt = MONGOTESTFMT_GETBSON;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF;
  l_new_str = mongotestfmt_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
mongotestfmt_change_string (bot_t * bot, char *string, int opt)
{
  bson *b;

  dlist_t *dl_fmt = NULL, *dlptr_fmt, *dlptr_args = NULL;

  mongo_argument_t arg, **arg_array = NULL;
  char **char_array = NULL;

  char *str_fmt = NULL;

  char *str_ptr = NULL, *str = NULL;
  char buf[MAX_BUF_SZ];

  char *sep_ptr;


  debug (bot, "mongotestfmt_change_string: Entered\n");

  if (!bot || !string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));
  memset (&arg, 0, sizeof (arg));

  if (opt == MONGOTESTFMT_ADD)
    {


      puts ("TESTING MONGODB_FMT_BUILD_STRINGS:");
      arg_array = mongodb_fmt_build_strings (bot, string);
      if (!arg_array)
	{
	  puts ("arg_array = NULL");
	  return NULL;
	}

      mongodb_arg_print (bot, arg_array);

      mongodb_fmt_insert_arg (bot, "test.darqbottestmongo", arg_array);

      mongodb_arg_destroy (bot, arg_array);

      arg_array = NULL;

      puts ("TESTING MONGODB_FMT_BUILD_FMT:");
      arg_array =
	mongodb_fmt_build_fmt (bot, "%s=name %i=age %f=IQ %l=addr", "darq",
			       29, 107.9, 1924020102);
      if (!arg_array)
	{
	  puts ("arg_array = NULL");
	  return NULL;
	}

      mongodb_arg_print (bot, arg_array);

      mongodb_fmt_insert_arg (bot, "test.darqbottestmongo", arg_array);

      mongodb_arg_destroy (bot, arg_array);


    }

  else if (opt == MONGOTESTFMT_ADDBSON)
    {
      puts ("MONGOTESTFMT_ADDBSON");
      dl_fmt =
	tokenize (bot, string, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
      if (!dl_fmt)
	return NULL;

      puts ("xx1");

      str_fmt = dlist_data (dl_fmt);
      if (!str_fmt)
	goto cleanup;

      puts ("xx2");

      dlptr_args = dlist_next (dl_fmt);
      if (!dlptr_args)
	goto cleanup;

      puts ("xx3");

      dlist_fornext (dlptr_args, dlptr_fmt)
      {
	printf ("WTF %s\n", (char *) dlist_data (dlptr_fmt));
      }

      dlptr_args->sz = dl_fmt->sz - 1;
      char_array = (char **) dlist_convert_dlist_to_array (dlptr_args);
      printf ("%p\n", char_array);
      if (!char_array)
	goto cleanup;

      puts ("xx4");

//b = mongodb_bson_build_strings(bot, string);
//b =  mongodb_bson_build(bot, "%s=name %i=age %f=grade", "joe", 30, 55.5);
      b = mongodb_bson_build (bot, str_fmt,
			      char_array[0],
			      dlist_size (dl_fmt) <
			      2 ? NULL : char_array[1],
			      dlist_size (dl_fmt) <
			      3 ? NULL : char_array[2],
			      dlist_size (dl_fmt) <
			      4 ? NULL : char_array[3],
			      dlist_size (dl_fmt) <
			      5 ? NULL : char_array[4],
			      dlist_size (dl_fmt) <
			      6 ? NULL : char_array[5],
			      dlist_size (dl_fmt) <
			      7 ? NULL : char_array[6],
			      dlist_size (dl_fmt) < 8 ? NULL : char_array[7]);

      if (!b)
	{
	  puts ("b=NULL");
	  return NULL;
	}

      mongodb_bson_print (b);

      mongodb_bson_insert (bot, "test.darqbottestmongo", b);

      bson_destroy (b);
    }
  else if (opt == MONGOTESTFMT_GETBSON)
    {

      str_ptr = string;
      arg.name = strdup (str_ptr);

      str_ptr = strchr (arg.name, '=');
      if (!str_ptr)
	goto cleanup;

      *str_ptr = '\0';
      str_ptr++;
      if (strlen (str_ptr) == 0)
	goto cleanup;

      arg.arg_str = strdup (str_ptr);

      arg.type = MONGODB_ARG_STRING;

      printf ("GETBSON: attempting to retrieve key=%s, val=%s\n", arg.name,
	      arg.arg_str);

      b =
	mongodb_bson_get (bot, "test.darqbottestmongo", arg.name,
			  MONGODB_ARG_STRING, arg.arg_str);

      if (!b)
	goto cleanup;

      mongodb_bson_print (b);

/*
if(1) {
bson  b2;
char * str;
bson_iterator it;
bson_copy(&b2,b);
b2.finished = 0;
bson_append_string(&b2, "derp", "toot");
bson_append_int(&b2, "int", 32);
bson_append_double(&b2, "doub", 5.5);

bson_finish(&b2);
puts("printing bson copy");
mongodb_bson_print(&b2);

mongodb_bson_insert(bot, "test.darqbottestmongo", &b2);
}
*/


      bson_destroy (b);

    }

  else if (opt == MONGOTESTFMT_GET)
    {

      str_ptr = string;
      arg.name = strdup (str_ptr);

      str_ptr = strchr (arg.name, '=');
      if (!str_ptr)
	goto cleanup;

      *str_ptr = '\0';
      str_ptr++;
      if (strlen (str_ptr) == 0)
	goto cleanup;

      arg.arg_str = strdup (str_ptr);

      arg.type = MONGODB_ARG_STRING;

      printf ("attempting to retrieve key=%s, val=%s\n", arg.name,
	      arg.arg_str);

      arg_array = mongodb_fmt_retrieve (bot, "test.darqbottestmongo", &arg);

      printf ("hm %p\n", arg_array);

      mongodb_arg_print (bot, arg_array);

      mongodb_arg_destroy (bot, arg_array);

    }
  else if (opt == MONGOTESTFMT_TEST)
    {

      str_ptr = string;
      arg.name = strdup (str_ptr);

      str_ptr = strchr (arg.name, '=');
      if (!str_ptr)
	goto cleanup;

      *str_ptr = '\0';
      str_ptr++;
      if (strlen (str_ptr) == 0)
	goto cleanup;

      arg.arg_str = strdup (str_ptr);

      arg.type = MONGODB_ARG_STRING;

      printf ("attempting to retrieve key=%s, val=%s\n", arg.name,
	      arg.arg_str);

      arg_array = mongodb_fmt_retrieve (bot, "test.darqbottestmongo", &arg);

      printf ("hm %p\n", arg_array);

      mongodb_arg_print (bot, arg_array);

      if (arg_array[1]->arg_str)
	{
	  puts ("changing to funk");
	  free (arg_array[1]->arg_str);
	  arg_array[1]->arg_str = strdup ("funk");
	}

      mongodb_fmt_insert_arg (bot, "test.darqbottestmongo", arg_array);

      mongodb_arg_destroy (bot, arg_array);

    }


cleanup:

  if (arg.name)
    free (arg.name);
  if (arg.arg_str);
  free (arg.arg_str);


  if (sNULL (buf) != NULL)
    str = strdup (buf);


  return str;
}
