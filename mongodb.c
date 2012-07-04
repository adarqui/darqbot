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
mongodb_init (bot_t * bot)
{
  int status;

  status = mongo_connect (&gi->mongo_conn, "127.0.0.1", 27017);

  if (status != MONGO_OK)
    {
      debug (bot, "mongodb_init: Failed\n");
      gi->mongo_isconnected = 0;
      return NULL;
    }

  debug (bot, "mongodb_init: Successfully connected to mongo\n");

  gi->mongo_isconnected = 1;

  return bot;
}




bot_t *
mongodb_fini (bot_t * bot)
{

/* SUPERFIX
*/

  mongo_destroy (&gi->mongo_conn);
memset(&gi->mongo_conn,0,sizeof(gi->mongo_conn));

  debug (bot, "mongodb_fini: Destroying mongo connection\n");

  return bot;
}




int
mongodb_insert_key (bot_t * bot, char *db, char *key, char *value,
		      char *fmt, ...)
{
  bson b;
  mongo_cursor cursor;
  va_list ap;
  char buf[1024], *buf_ptr = "NULL";

  if (!db || !key || !value)
    {
      return -1;
    }

  debug (bot, "mongodb_insert_key: Entered\n");

  if (fmt)
    {
      bz (buf);
      va_start (ap, fmt);
      vsnprintf_buf (buf, fmt, ap);
      va_end (ap);
      buf_ptr = buf;
    }

  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {
      debug (bot, "mongodb_insert_key: Key already exist\n");
      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);
      return -1;
    }

  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_append_string (&b, "value", value);
  bson_append_string (&b, "comment", buf_ptr);

  bson_finish (&b);

  mongo_insert (&gi->mongo_conn, db, &b);

  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return 0;
}






char *
mongodb_retrieve_key (bot_t * bot, char *db, char *key, char *which)
{
  char *str = NULL;
  bson b;
  mongo_cursor cursor;

  if (!db || !key || !which)
    {
      return NULL;
    }

  debug (bot,
	     "mongodb_retrieve_key: Entered :db=%s. key=%s, which=%s\n", db,
	     key, which);


  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {
      bson_iterator i;

      debug (bot, "mongodb_retrieve_key: Found!\n");
      if (bson_find (&i, mongo_cursor_bson (&cursor), which))
	{
	  str = (char *) bson_iterator_string (&i);
	  str = strdup (str);
	}

      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);

      return str;
    }

  debug (bot, "mongodb_retrieve_key: Key not found\n");

  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return NULL;
}









int
mongodb_remove_key (bot_t * bot, char *db, char *key)
{
  bson b;
  mongo_cursor cursor;

  if (!db || !key)
    {
      return -1;
    }

  debug (bot, "mongodb_remove_key: Entered\n");


  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {
      bson_iterator i;

      debug (bot, "mongodb_remove_key: Found!\n");
      if (bson_find (&i, mongo_cursor_bson (&cursor), "value"))
	{
	  mongo_remove (&gi->mongo_conn, db, &b);
	}

      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);

      return 0;
    }

  debug (bot, "mongodb_remove_key: Key not found\n");

  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return -1;
}














int
mongodb_insert_key_stat (bot_t * bot, char *db, char *key, int value)
{
  bson b;

  if (!db || !key || value < 0)
    {
      return -1;
    }

  debug (bot, "mongodb_insert_key: Entered\n");



  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_append_int (&b, "value", value);

  bson_finish (&b);

  mongo_insert (&gi->mongo_conn, db, &b);

  bson_destroy (&b);

  return 0;
}






int
mongodb_retrieve_key_stat (bot_t * bot, char *db, char *key)
{
  bson b;
  mongo_cursor cursor;
  int ret = 0;

  if (!db || !key)
    {
      return -1;
    }

  debug (bot, "mongodb_retrieve_key: Entered\n");


  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {
      bson_iterator i;

      debug (bot, "mongodb_retrieve_key: Found!\n");
      if (bson_find (&i, mongo_cursor_bson (&cursor), "value"))
	{
	  ret = (int) bson_iterator_int (&i);
	}

      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);

      return ret;
    }

  debug (bot, "mongodb_retrieve_key: Key not found\n");

  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return -1;
}











int
mongodb_update_key_stat (bot_t * bot, char *db, char *key, int value)
{
  bson b;
  bson_iterator i;
  mongo_cursor cursor;
  int ret = 0;

  if (!db || !key || value < 0)
    {
      return -1;
    }

  debug (bot, "mongodb_update_key_stat: Entered\n");


  bson_init (&b);
  bson_append_string (&b, "key", key);
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) != MONGO_OK)
    {
      mongodb_insert_key_stat (bot, db, key, 0);

      mongo_cursor_init (&cursor, &gi->mongo_conn, db);
      mongo_cursor_set_query (&cursor, &b);

      if (mongo_cursor_next (&cursor) != MONGO_OK)
	goto cleanup;
    }


  debug (bot, "mongodb_update_key_stat: Found!\n");
  if (bson_find (&i, mongo_cursor_bson (&cursor), "value"))
    {
      bson c;

      ret = (int) bson_iterator_int (&i);
      ret++;

      bson_init (&c);
      bson_append_string (&c, "key", key);
      bson_append_int (&c, "value", ret);
      bson_finish (&c);

      debug (bot, "mongodb_update_key_stat: updating to %i\n", ret);

      mongo_update (&gi->mongo_conn, db, &b, &c, 0);

      bson_destroy (&c);
      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);

      return ret;
    }

  debug (bot, "mongodb_update_key_stat: Key not found\n");

cleanup:
  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return -1;
}









dlist_t *
mongodb_dump_keyvalue (bot_t * bot, char *db)
{
  dlist_t *dl, *dptr;
  keyvalue_t *keyvalue_ptr;
  mongo_cursor *cursor;
  bson b;
  bson_iterator bi;

  char *key;
  char *value;

  dl = dptr = NULL;
  cursor = NULL;

  if (!db)
    return NULL;

  debug (bot, "mongodb_dump_keyvalue: Entered\n");

  cursor =
    mongo_find (&gi->mongo_conn, db, bson_empty (&b),
		bson_empty (&b), 0, 0, 0);

  while (mongo_cursor_next (cursor) == MONGO_OK)
    {
      key = value = NULL;

      if (bson_find (&bi, mongo_cursor_bson (cursor), "key"))
	{
	  key = (char *) bson_iterator_string (&bi);
	}
      if (bson_find (&bi, mongo_cursor_bson (cursor), "value"))
	{
	  value = (char *) bson_iterator_string (&bi);
	}

      if (!key && !value)
	continue;

      keyvalue_ptr = (keyvalue_t *) calloc (1, sizeof (keyvalue_t));
      if (key)
	keyvalue_ptr->key = strdup (key);
      if (value)
	keyvalue_ptr->value = strdup (value);

      dlist_Dinsert_after (&dl, keyvalue_ptr);

    }


  mongo_cursor_destroy (cursor);

  return dl;
}



void
mongodb_destroy_keyvalue (bot_t * bot, dlist_t ** dl)
{

  if (!dl)
    return;

  dlist_fini (dl, mongodb_free_keyvalue);

  return;
}



void
mongodb_free_keyvalue (void *kv_arg)
{
  keyvalue_t *kv = (keyvalue_t *) kv_arg;
  if (!kv)
    return;

  if (kv->key)
    free (kv->key);
  if (kv->value)
    free (kv->value);

  free (kv);

  return;
}





mongo_argument_t **
mongodb_fmt_build_fmt (bot_t * bot, char *fmt, ...)
{
  dlist_t *dl_fmt, *dptr_fmt;
  va_list ap;
  mongo_argument_t **arg_array = NULL;
  char *str_fmt, *str_arg_key;
  int i;

  debug (bot, "mongodb_fmt_build_fmt: Entered\n");

  if (!fmt)
    return NULL;

  dl_fmt =
    tokenize (bot, fmt, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE,
		  "%");
  if (!dl_fmt)
    {
      return NULL;
    }

  va_start (ap, fmt);

  arg_array =
    (mongo_argument_t **) calloc (dlist_size (dl_fmt) + 1,
				  sizeof (mongo_argument_t *));
  if (!arg_array)
    goto cleanup;

  i = 0;
  dlist_fornext (dl_fmt, dptr_fmt)
  {

    str_fmt = dlist_data (dptr_fmt);
    if (!str_fmt)
      break;

    str_arg_key = str_fmt + 2;
    str_arg_key = eat_whitespace (str_arg_key);
    strstrip_chars (str_arg_key, " ");

    debug (bot, "mongodb_fmt_build_fmt: adding key=%s\n", str_arg_key);

    arg_array[i] = (mongo_argument_t *) calloc (1, sizeof (mongo_argument_t));

    arg_array[i]->name = strdup (str_arg_key);

    switch (*str_fmt)
      {
      case 's':
	{
	  arg_array[i]->type = MONGODB_ARG_STRING;
	  arg_array[i]->arg_str = strdup (va_arg (ap, char *));
	  break;
	}
      case 'i':
	arg_array[i]->type = MONGODB_ARG_INT;
	arg_array[i]->arg_int = va_arg (ap, int);
	{
	  break;
	}
      case 'f':
      case 'd':
	{
	  arg_array[i]->type = MONGODB_ARG_DOUBLE;
	  arg_array[i]->arg_double = va_arg (ap, double);
	  break;
	}
      case 'l':
	{
	  arg_array[i]->type = MONGODB_ARG_LONG;
	  arg_array[i]->arg_long = va_arg (ap, long);
	  break;
	}
      default:
	break;
      }

    i++;
  }


  va_end (ap);


cleanup:

  tokenize_destroy (bot, &dl_fmt);

  return arg_array;
}





/* only operates on strings, returns array */
mongo_argument_t **
mongodb_fmt_build_strings (bot_t * bot, char *string)
{
  dlist_t *dl_args = NULL, *dl_fmt = NULL, *dptr_arg, *dptr_fmt;
  mongo_argument_t **arg_array = NULL;
  char *fmt;

  char *str_fmt, *str_arg, *str_arg_key, *str_arg_val;

  int i;


  dl_args =
    tokenize (bot, string,
		  TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ");
  if (!dl_args)
    return NULL;

  fmt = dlist_data (dl_args);
  if (!fmt)
    goto cleanup;

  dl_fmt = tokenize (bot, fmt, TOKENIZE_NORMAL, "%");
  if (!dl_fmt)
    goto cleanup;


  arg_array =
    (mongo_argument_t **) calloc (dlist_size (dl_fmt) + 1,
				      sizeof (mongo_argument_t *));
  if (!arg_array)
    goto cleanup;


  for (i = 0, dptr_fmt = dl_fmt, dptr_arg = dlist_next (dl_args);
       dptr_fmt != NULL && dptr_arg != NULL;
       dptr_fmt = dptr_fmt->next, dptr_arg = dptr_arg->next)
    {
      if (!dptr_arg || !dptr_fmt)
	break;
      str_fmt = dlist_data (dptr_fmt);


      if (!str_fmt)
	break;
      str_arg = dlist_data (dptr_arg);
      if (!str_arg)
	break;

      str_arg_key = str_fmt + 2;

      str_arg_key = eat_whitespace (str_arg_key);
      strstrip_chars (str_arg_key, " ");

      str_arg_val = str_arg;

      debug (bot,
		 "mongodb_fmt_build_strings: Adding key=%s, val=%s to bson object\n",
		 str_arg_key, str_arg_val);

      arg_array[i] =
	(mongo_argument_t *) calloc (1, sizeof (mongo_argument_t));

      arg_array[i]->name = strdup (str_arg_key);

      switch (*str_fmt)
	{
	case 's':
	  {
	    arg_array[i]->type = MONGODB_ARG_STRING;
	    arg_array[i]->arg_str = strdup (str_arg_val);
	    break;
	  }
	case 'i':
	  {
	    arg_array[i]->type = MONGODB_ARG_INT;
	    arg_array[i]->arg_int = atoi (str_arg_val);
	    break;
	  }
	case 'f':
	case 'd':
	  {
	    arg_array[i]->type = MONGODB_ARG_DOUBLE;
	    arg_array[i]->arg_double = atof (str_arg_val);
	    break;
	  }
	case 'l':
	  {
	    arg_array[i]->type = MONGODB_ARG_LONG;
	    arg_array[i]->arg_long = atol (str_arg_val);
	    break;
	  }
	default:
	  break;
	}
/* end strings opt */

      i++;
    }


cleanup:

  if (dl_fmt)
    tokenize_destroy (bot, &dl_fmt);

  if (dl_args)
    tokenize_destroy (bot, &dl_args);

  return arg_array;
}




int
mongodb_fmt_insert_arg (bot_t * bot, char *db,
			  mongo_argument_t ** arg_array)
{
/* example: ^e |^mongotestfmt "%s=name %i=age %f=grade" Darqbot 50 99.9 */

  int i;
  bson b;

  debug (bot, "mongodb_fmt_insert_arg: Entered\n");

  if (!db || !arg_array)
    return -1;

  bson_init (&b);

  for (i = 0; arg_array[i] != NULL; i++)
    {
      switch (arg_array[i]->type)
	{
	case MONGODB_ARG_STRING:
	  {
	    bson_append_string (&b, arg_array[i]->name,
				arg_array[i]->arg_str);
	    break;
	  }
	case MONGODB_ARG_INT:
	  {
	    bson_append_int (&b, arg_array[i]->name, arg_array[i]->arg_int);
	    break;
	  }
	case MONGODB_ARG_DOUBLE:
	  {
	    bson_append_double (&b, arg_array[i]->name,
				arg_array[i]->arg_double);
	    break;
	  }
	case MONGODB_ARG_LONG:
	  {
	    bson_append_long (&b, arg_array[i]->name, arg_array[i]->arg_long);
	    break;
	  }
	case MONGODB_ARG_OID:
	  {
// FIX
	    bson_append_oid (&b, arg_array[i]->name, &arg_array[i]->arg_oid);
	    break;
	  }
	default:
	  break;
	}
    }

  bson_finish (&b);

  i = mongo_insert (&gi->mongo_conn, db, &b);

  bson_destroy (&b);

  return 0;
}






mongo_argument_t **
mongodb_fmt_retrieve (bot_t * bot, char *db, mongo_argument_t * arg)
{
/* return a mongo argument array based on arg (arg is a key) */
  char *str = NULL;
  bson b;
  mongo_cursor cursor;

  mongo_argument_t **arg_array = NULL, *arg_ptr;
  dlist_t *arg_dlist = NULL, *dptr;


  if (!db || !arg)
    {
      return NULL;
    }

  debug (bot, "mongodb_fmt_retrieve: Entered :db=%s\n", db);


  bson_init (&b);
  switch (arg->type)
    {
    case MONGODB_ARG_STRING:
      bson_append_string (&b, arg->name, arg->arg_str);
      break;
    case MONGODB_ARG_INT:
      bson_append_int (&b, arg->name, arg->arg_int);
      break;
    case MONGODB_ARG_DOUBLE:
      bson_append_double (&b, arg->name, arg->arg_double);
      break;
    case MONGODB_ARG_LONG:
      bson_append_long (&b, arg->name, arg->arg_long);
      break;
    default:
      return NULL;
    }
  bson_finish (&b);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {
      bson_iterator it;

      debug (bot, "mongodb_fmt_retrieve: Found!\n");

      bson_iterator_init (&it, mongo_cursor_bson (&cursor));


      while (bson_iterator_next (&it))
	{
	  arg_ptr =
	    (mongo_argument_t *) calloc (1, sizeof (mongo_argument_t));
	  arg_ptr->name = strdup (bson_iterator_key (&it));
	  switch (bson_iterator_type (&it))
	    {

	    case BSON_STRING:
	      {
		arg_ptr->type = MONGODB_ARG_STRING;
		str = (char *) bson_iterator_string (&it);
		arg_ptr->arg_str = strdup (str);
		break;
	      }
	    case BSON_INT:
	      {
		arg_ptr->type = MONGODB_ARG_INT;
		arg_ptr->arg_int = (int) bson_iterator_int (&it);
		break;
	      }
	    case BSON_DOUBLE:
	      {
		arg_ptr->type = MONGODB_ARG_DOUBLE;
		arg_ptr->arg_double = (double) bson_iterator_double (&it);
		break;
	      }
	    case BSON_LONG:
	      {
		arg_ptr->type = MONGODB_ARG_LONG;
		arg_ptr->arg_long = (long) bson_iterator_long (&it);
		break;
	      }
	    case BSON_OID:
	      {
		arg_ptr->type = MONGODB_ARG_OID;
		memcpy (&arg_ptr->arg_oid, bson_iterator_oid (&it),
			sizeof (arg_ptr->arg_oid));
		break;
	      }

	    default:
	      {
		arg_ptr->type = MONGODB_ARG_UNKNOWN;
		break;
	      }
	    }

	  dlist_Dinsert_after (&arg_dlist, arg_ptr);
	}



      dlist_fornext (arg_dlist, dptr)
      {
	arg_ptr = (mongo_argument_t *) dlist_data (dptr);
      }


      bson_destroy (&b);
      mongo_cursor_destroy (&cursor);

      arg_array =
	(mongo_argument_t **) dlist_convert_dlist_to_array (arg_dlist);
      dlist_fini (&arg_dlist, NULL);

      return arg_array;
    }



  bson_destroy (&b);
  mongo_cursor_destroy (&cursor);

  return NULL;
}






int
mongodb_arg_destroy (bot_t * bot, mongo_argument_t ** arg_array)
{
  int i;

  if (!arg_array)
    {
      return -1;
    }

  for (i = 0; arg_array[i] != NULL; i++)
    {
      mongodb_arg_free (arg_array[i]);
    }
  free (arg_array);

  return 0;
}



void
mongodb_arg_free (void *ptr)
{
  mongo_argument_t *arg_ptr;

  arg_ptr = (mongo_argument_t *) ptr;
  if (!arg_ptr)
    return;


  if (arg_ptr->name)
    free (arg_ptr->name);
  if (arg_ptr->arg_str)
    free (arg_ptr->arg_str);

  memset (arg_ptr, 0, sizeof (mongo_argument_t));

  free (arg_ptr);

  return;
}





void
mongodb_arg_print (bot_t * bot, mongo_argument_t ** arg_array)
{
  mongo_argument_t *arg_ptr;
  int i;

  if (!arg_array)
    return;

  for (i = 0; arg_array[i] != NULL; i++)
    {
      arg_ptr = (mongo_argument_t *) arg_array[i];
      mongodb_arg_print_arg (arg_ptr);
    }

  return;
}



void
mongodb_arg_print_arg (mongo_argument_t * arg)
{

  if (!arg)
    return;

  printf ("mongodb_arg_print_arg:\n"
	  "\targ->type=%i\n"
	  "\targ->name=%s\n"
	  "\targ->arg_str=%s\n"
	  "\targ->arg_int=%i\n"
	  "\targ->arg_double=%f\n"
	  "\targ->arg_long=%li\n",
	  arg->type,
	  arg->name,
	  arg->arg_str, arg->arg_int, arg->arg_double, arg->arg_long);

  return;
}















/*
 * THE ABOVE CODE IS BEYOND RETARDED, OPERATE ON BSON OBJECTS OR GTFO. 
 * 
 * leaving that code in this file, as a remembrance of the colossal waste of time that was
 */



/* wrapper function for building bson objects using stdarg */
bson *
mongodb_bson_build (bot_t * bot, char *fmt, ...)
{
  bson *b;
  dlist_t *dl_fmt = NULL, *dptr_fmt;
  va_list ap;
  char *str_fmt, *str_arg_key;

  debug (bot, "mongodb_bson_build: Entered\n");

  if (!fmt)
    return NULL;

  dl_fmt =
    tokenize (bot, fmt, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE,
		  "%");
  if (!dl_fmt)
    {
      return NULL;
    }

  b = (bson *) calloc (1, sizeof (bson));
  if (!b)
    goto cleanup;


  bson_init (b);

  va_start (ap, fmt);

  dlist_fornext (dl_fmt, dptr_fmt)
  {

    str_fmt = dlist_data (dptr_fmt);
    if (!str_fmt)
      break;

    str_arg_key = str_fmt + 2;

    str_arg_key = eat_whitespace (str_arg_key);
    strstrip_chars (str_arg_key, " ");

    switch (*str_fmt)
      {
      case 's':
	{
	  bson_append_string (b, str_arg_key, va_arg (ap, char *));
	  break;
	}
      case 'i':
	bson_append_int (b, str_arg_key, va_arg (ap, int));
	{
	  break;
	}
      case 'f':
      case 'd':
	{
	  bson_append_double (b, str_arg_key, va_arg (ap, double));
	  break;
	}
      case 'l':
	{
	  bson_append_long (b, str_arg_key, va_arg (ap, long));
	  break;
	}
      default:
	break;
      }

  }



  va_end (ap);


cleanup:

  bson_finish (b);

  tokenize_destroy (bot, &dl_fmt);

  return b;
}










int
mongodb_bson_insert (bot_t * bot, char *db, bson * b)
{

  if (!bot || !db || !b)
    return -1;

  bson_finish (b);

  mongo_insert (&gi->mongo_conn, db, b);

  return 0;
}


bson *
mongodb_bson_get (bot_t * bot, char *db, char *key, int type, void *val)
{
  bson *b = NULL, b2, *b3 = NULL;
  mongo_cursor cursor;

  char *arg_str;
  int arg_int;
  double arg_double = 0;
  long arg_long;

  if (!db || !key || !type || !val)
    return NULL;

  bson_init (&b2);
  switch (type)
    {
    case MONGODB_ARG_STRING:
      {
	arg_str = (char *) val;
	bson_append_string (&b2, key, arg_str);
	break;
      }
    case MONGODB_ARG_INT:
      {
	arg_int = (int) val;
	bson_append_int (&b2, key, arg_int);
	break;
      }
    case MONGODB_ARG_DOUBLE:
      {
/* arg_double = (double)val; */
	bson_append_double (&b2, key, arg_double);
	break;
      }
    case MONGODB_ARG_LONG:
      {
	arg_long = (long) val;
	bson_append_long (&b2, key, arg_long);
	break;
      }
    default:
      {
	goto cleanup;
	break;
      }
    }


  bson_finish (&b2);
  mongo_cursor_init (&cursor, &gi->mongo_conn, db);
  mongo_cursor_set_query (&cursor, &b2);

  if (mongo_cursor_next (&cursor) == MONGO_OK)
    {

      debug (bot, "mongodb_bson_get: Found!\n");

      b3 = (bson *) mongo_cursor_bson (&cursor);

      b = (bson *) calloc (1, sizeof (bson));
      bson_copy (b, b3);
    }


cleanup:

  bson_destroy (&b2);

  return b;
}










void
mongodb_bson_unfinish (bson * b)
{
  if (b)
    {
      b->finished = 0;
    }

  return;
}



char *
mongodb_bson_print_init (const bson * b)
{
  char *str = NULL;

  if (!b)
    return NULL;

  memset (mpbuf, 0, sizeof (mpbuf));

  mongodb_bson_print (b);

  if (sNULL(mpbuf)!=NULL)
    str = strdup (mpbuf);

  return str;
}


void
mongodb_bson_print (const bson * b)
{
  mongodb_bson_print_raw (b->data, 0);
}

void
mongodb_bson_print_raw (const char *data, int depth)
{

  bson_iterator i;
  const char *key;
  int temp;
  bson_timestamp_t ts;
  char oidhex[25];
  bson scope;

  bson_iterator_from_buffer (&i, data);

  while (bson_iterator_next (&i))
    {
      bson_type t = bson_iterator_type (&i);
      if (t == 0)
	break;
      key = bson_iterator_key (&i);

      for (temp = 0; temp <= depth; temp++)
	strlcat_buf (mpbuf, " ");
      strlcatfmt_buf (mpbuf, "%s : %d ... ", key, t);
      switch (t)
	{
	case BSON_DOUBLE:
	  strlcatfmt_buf (mpbuf, "%f", bson_iterator_double (&i));
	  break;
	case BSON_STRING:
	  strlcatfmt_buf (mpbuf, "%s", bson_iterator_string (&i));
	  break;
	case BSON_SYMBOL:
	  strlcatfmt_buf (mpbuf, "SYMBOL :%s",
			  bson_iterator_string (&i));
	  break;
	case BSON_OID:
	  bson_oid_to_string (bson_iterator_oid (&i), oidhex);
	  strlcatfmt_buf (mpbuf, "%s", oidhex);
	  break;
	case BSON_BOOL:
	  strlcatfmt_buf (mpbuf,  "%s", bson_iterator_bool (&i) ? "true" : "false");
	  break;
	case BSON_DATE:
	  strlcatfmt_buf (mpbuf, "%ld",
			  (long int) bson_iterator_date (&i));
	  break;
	case BSON_BINDATA:
	  strlcatfmt_buf (mpbuf, "BSON_BINDATA");
	  break;
	case BSON_UNDEFINED:
	  strlcatfmt_buf (mpbuf,  "BSON_UNDEFINED");
	  break;
	case BSON_NULL:
	  strlcatfmt_buf (mpbuf,  "BSON_NULL");
	  break;
	case BSON_REGEX:
	  strlcatfmt_buf (mpbuf, "BSON_REGEX: %s",
			  bson_iterator_regex (&i));
	  break;
	case BSON_CODE:
	  strlcatfmt_buf (mpbuf, "BSON_CODE: %s",
			  bson_iterator_code (&i));
	  break;
	case BSON_CODEWSCOPE:
	  strlcatfmt_buf (mpbuf, "BSON_CODE_W_SCOPE: %s",
			  bson_iterator_code (&i));
	  bson_init (&scope);
	  bson_iterator_code_scope (&i, &scope);
	  strlcatfmt_buf (mpbuf,  "\n -> SCOPE: ");
	  bson_print (&scope);
	  break;
	case BSON_INT:
	  strlcatfmt_buf (mpbuf,  "%d",
			  bson_iterator_int (&i));
	  break;
	case BSON_LONG:
	  strlcatfmt_buf (mpbuf, "%lld",
			  (uint64_t) bson_iterator_long (&i));
	  break;
	case BSON_TIMESTAMP:
	  ts = bson_iterator_timestamp (&i);
	  strlcatfmt_buf (mpbuf, "i: %d, t: %d", ts.i,
			  ts.t);
	  break;
	case BSON_OBJECT:
	case BSON_ARRAY:
	  strlcatfmt_buf (mpbuf,  "\n");
	  bson_print_raw (bson_iterator_value (&i), depth + 1);
	  break;
	default:
	  break;
	}
      strlcatfmt_buf (mpbuf,  "\n");
    }

}











void
mongodb_drop_db (char *db)
{
  int i;

  if (!db)
    return;

  i = 0;
  mongo_cmd_drop_db (&gi->mongo_conn, db);

  return;
}
