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
#include "conf.h"

char *
bot_find_confdir (bot_t * bot)
{
  struct stat st;
  int n;
  char *str = NULL;

  debug (bot, "bot_find_confdir: Entered\n");

  if (!bot)
    return NULL;

  str = str_unite_static ("%s/%s", gi->confdir, BOT_DEF_CONFDIR);

  if (!sNULL(str))
    return NULL;

  debug (bot, "bot_find_confdir: trying str=%s\n", str);

  n = stat (str, &st);
  if (n == 0)
    return str;

  return NULL;
}



int
conf_parse (bot_t * bot)
{
  FILE *fp;
  char buf[MAX_BUF_SZ], *buf_ptr;
  int add;

  debug (bot, "conf_parse: Entered\n");


  if (!bot)
    {
      debug (bot, "conf_parse: BOT=NULL\n");
      return -1;
    }

  if (!sNULL(bot->confdir))
    {
      debug (bot, "conf_parse: BOT->confdir=NULL\n");
      return -1;
    }


if(!sNULL(bot->conffile))
snprintf_buf(bot->conffile, "%s/%s", bot->confdir, BOT_DEF_CONFFILE);

  fp = fopen (bot->conffile, "r");
  if (!fp)
    {
      debug (bot, "conf_parse: unable to open %s\n", bot->conffile);
      return -1;
    }

  debug (bot, "conf_parse: opened %s\n", bot->conffile);

  while (1)
    {
      bz(buf);
      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;

      strstrip_nl (buf);

      if (buf[0] == '+')
	add = 1;
      else if (buf[0] == '-')
	add = 0;
      else
	continue;


      buf_ptr = (buf + 1);

      if (!strncasecmp_len (buf_ptr, "on"))
	{
	  bot->on = 1;
	}
      else if (!strncasecmp_len (buf_ptr, "off"))
	{
	  bot->on = 0;
	}
      else if (!strncasecmp_len (buf_ptr, "tag"))
	{
	  bot_parse_tag (bot, &buf_ptr[fn_strlen ("tag") + 1], add);
	}
      else if (!strncasecmp_len (buf_ptr, "trace"))
	{
	  bot_parse_trace (bot, &buf_ptr[fn_strlen ("trace") + 1], add);
	}
      else if (!strncasecmp_len (buf_ptr, "debug"))
	{
	  bot_parse_debug (bot, &buf_ptr[fn_strlen ("debug") + 1], add);
	}

      else if (!strncasecmp_len (buf_ptr, "mute"))
	{
	  bot->mute = 1;
	}
      else if (!strncasecmp_len (buf_ptr, "logfile"))
	{
	  bot_parse_logfile (bot, &buf_ptr[fn_strlen ("logfile") + 1], add);
	}
      else if (!strncasecmp_len (buf_ptr, "var_usleep"))
	{
	  bot->var_usleep = atoi (&buf_ptr[fn_strlen ("var_usleep") + 1]);
	}

      else if (!strncasecmp_len (buf_ptr, "var_maxbufsz"))
	{
	  bot->var_maxbufsz = atoi (&buf_ptr[fn_strlen ("var_maxbufsz") + 1]);
	  bot->var_bufsz = bot->var_maxbufsz;
	}

      else if (!strncasecmp_len (buf_ptr, "var_maxflood"))
	{
	  bot->var_maxflood = atoi (&buf_ptr[fn_strlen ("var_maxflood") + 1]);

	}
      else if (!strncasecmp_len (buf_ptr, "var_maxnesting"))
	{
	  bot->var_maxnesting =
	    atoi (&buf_ptr[fn_strlen ("var_maxnesting") + 1]);
	}
      else if (!strncasecmp_len (buf_ptr, "var_nesting"))
	{
	  bot->var_nesting = atoi (&buf_ptr[fn_strlen ("var_nesting") + 1]);
	}
      else if (!strncasecmp_len (buf_ptr, "allowpm"))
	{
	  if (xstrcasestr (&buf_ptr[fn_strlen ("allowpm") + 1], "yes"))
	    bot->var_allowpm = 1;
	  else
	    bot->var_allowpm = 0;
	}
      else if (!strncasecmp_len (buf_ptr, "gmodules"))
	{
	  bot_parse_gmodules (bot, &buf[fn_strlen ("gmodules") + 1], 1,
			      ":::");
	}
else if(!strncasecmp_len(buf_ptr, "trig_prefix")) {
strlcpy_buf(bot->trig_prefix,&buf[fn_strlen("trig_prefix")+2]);
}

    }


  fclose (fp);
  debug (bot, "conf_parse: Success\n");

  conf_fix (bot);


  return 0;
}



int
bot_parse_logfile (bot_t * bot, char *str, int add)
{

  char *ptr_1;

  debug (bot, "bot_parse_logfile: Entered\n");

  if (!bot || !sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = eat_whitespace (str);

bz(bot->logfile);

  if (sNULL(ptr_1))
    strlcpy_buf(bot->logfile, ptr_1);

  strstrip_chars (bot->logfile, " :,");

  return 0;
}



int
bot_parse_tag (bot_t * bot, char *str, int add)
{
  char *ptr_1;

  debug (bot, "bot_parse_tag: Entered\n");

  if (!bot || !sNULL(str))
    return -1;

  if (add == 0)
    {
      return 0;
    }

  ptr_1 = eat_whitespace (str);

  if (sNULL(ptr_1))
    {
      strstrip_chars (ptr_1, " :,!@#$%^&*()=+;':\",./<>?~`\\|");
    }

  strlcpy_buf(bot->tag,ptr_1);

  return 0;
}




int
bot_parse_trace (bot_t * bot, char *str, int add)
{
  char *ptr_1;

  if (!bot || !sNULL(str))
    return -1;

  if (add == 0)
    {
      return 0;
    }

  ptr_1 = eat_whitespace (str);

  if (!strcasecmp (ptr_1, "on"))
    {
      bot->trace = 1;
    }
  else
    {
      bot->trace = 0;
    }

  return 0;
}





int
bot_parse_debug (bot_t * bot, char *str, int add)
{
  char *ptr_1;

  if (!bot || !sNULL(str))
    return -1;

  if (add == 0)
    {
      return 0;
    }

  ptr_1 = eat_whitespace (str);


  if (!strcasecmp (ptr_1, "on"))
    {
      bot->debug = 1;
    }
  else
    {
      bot->debug = 0;
    }

  return 0;
}






int
bot_parse_gmodules (bot_t * bot, char *str, int add, char *delim)
{
  dlist_t *dl=NULL, *dptr;
  bot_gmod_elm_t *gmod = NULL;
  char *ptr_1, *ptr_2, *ptr_3;

char * str_tmp=NULL;

  if (!bot || !sNULL(str))
    return -1;

  if (!sNULL(delim))
    delim = ":::";

  ptr_1 = eat_whitespace (str);

  if (add != 0)
    {
/* this is for relinkd, which adds gmodules on the fly... we just want to append if we are grelinkd */
      if (bot->dl_gmodules)
	{
	  dlist_fini (&bot->dl_gmodules, bot_gmodule_elm_free);
	}
    }

  dl =
    tokenize (bot, ptr_1, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		  delim);

  dlist_fornext (dl, dptr)
  {
    gmod = (bot_gmod_elm_t *) calloc (1, sizeof (bot_gmod_elm_t));
str_tmp = (char *) dlist_data(dptr);
    gmod->syntax = (char *) strdup(str_tmp);
    dlist_Dinsert_after (&bot->dl_gmodules, gmod);

    ptr_2 = strchr (gmod->syntax, '(');
    if (!ptr_2)
      {
	ptr_2 = gmod->syntax + strlen (gmod->syntax);
      }

    ptr_3 = strstr_len (gmod->syntax, "#", ptr_2 - gmod->syntax);
    if (ptr_3)
      {
/*
	gmod->trigger_ext = strdup_len (ptr_3, (ptr_2 - ptr_3));
*/
strlcpy_buf(gmod->trigger_ext, str_unite_static("%.*s", (ptr_2-ptr_3), ptr_3));
	ptr_2 = ptr_3;
      }

/*
    gmod->trigger = strdup_len (gmod->syntax, (ptr_2 - gmod->syntax));
*/
strlcpy_buf(gmod->trigger, str_unite_static("%.*s", (ptr_2-gmod->syntax), gmod->syntax));

    if (sNULL(gmod->trigger_ext))
      {
	char *save, *tok;

	save = gmod->syntax;
	tok = strchr (gmod->syntax, '(');
	if (sNULL(tok))
	  {
	    gmod->syntax = str_unite ("%s%s", gmod->trigger, tok);
	  }
	if (gmod->syntax)
	  free (save);
      }

  }

tokenize_destroy(NULL, &dl);

  return 0;
}




char *
conf_get_proto (int proto)
{
  char *proto_str = "unknown";

  debug (NULL, "conf_get_proto: Entered\n");

  return proto_str;
}




char *
conf_get_type (int type)
{
  char *type_str = "unknown";

  debug (NULL, "conf_get_type: Entered\n");

  return type_str;
}


void
conf_fix (bot_t * bot)
{

  debug (bot, "conf_fix: Entred\n");

  if (!bot)
    return;

  return;
}
