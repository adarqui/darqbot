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
#include "mod_sp.h"

void
__sp_init__ (void)
{

strlcpy_buf(mod_sp_info.name, "mod_sp");
strlcpy_buf(mod_sp_info.trigger, "^sp");

  mod_sp_info.init = sp_init;
  mod_sp_info.fini = sp_fini;
  mod_sp_info.help = sp_help;
  mod_sp_info.run = sp_run;


  mod_sp_info.output = NULL;
  mod_sp_info.input = NULL;

  mod_sp_info.max_nesting = 5;

  debug (NULL, "__sp_init__: Loaded mod_sp\n");

  bz2(sp_info);

  return;
}



bot_t *
sp_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sp_init: Entered\n");

  sp_fill_info (bot);

  if (sp_info.should_on)
    {
      sp_run_on (bot);
      sp_info.should_on = 0;
    }

  return NULL;
}

bot_t *
sp_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sp_fini: Entered\n");

  sp_run_off (bot);

  return NULL;
}

bot_t *
sp_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sp_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^sp || ^sp(on:off:tags:deps:tree:info)";

  return NULL;
}

bot_t *
sp_run (dlist_t * dlist_node, bot_t * bot)
{
  char *str_ptr, *dl_module_arg_after_options, *dl_options_ptr, *new_str;
  int opt, n;

  debug (bot, "sp_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;


  stat_inc(bot,bot->trig_called);


  debug (bot,
	     "sp_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;


  if (bot->var_nesting_cur > mod_sp_info.max_nesting)
    return NULL;


  opt = 0;
  opt |= OPT_TAGS;
  opt |= OPT_DEPS;

  new_str = NULL;

  MOD_OPTIONS_TOP_HALF;

  if (xstrcasestr_len (dl_options_ptr, "on", len_options_area))
    {
      sp_run_on (bot);
      return NULL;
    }
  else if (xstrcasestr_len (dl_options_ptr, "off", len_options_area))
    {
      sp_run_off (bot);
      return NULL;
    }
  else if (xstrcasestr_len (dl_options_ptr, "tags", len_options_area))
    {
      opt |= OPT_TAGS;
    }
  else if (xstrcasestr_len (dl_options_ptr, "deps", len_options_area))
    {
      opt |= OPT_DEPS;
    }
  else if (xstrcasestr_len (dl_options_ptr, "tree", len_options_area))
    {
      opt |= OPT_TREE;
    }
  else if (xstrcasestr_len (dl_options_ptr, "info", len_options_area))
    {
      opt |= OPT_INFO;
    }


  MOD_OPTIONS_BOTTOM_HALF;


  if (opt & OPT_INFO)
    {
      sp_run_info (bot, eat_whitespace (dl_module_arg_after_options));
      return bot;
    }

  str_ptr = eat_whitespace (dl_module_arg_after_options);
  if (!str_ptr)
    str_ptr = "";

  if (sNULL(bot->txt_data_out)!=NULL)
    {
      new_str = str_unite ("%s%s\n", bot->txt_data_out, str_ptr);
    }
  else
    new_str = str_unite ("%s\n", str_ptr);

  if (new_str)
    {
      char buf[1024];
      char *buf_ptr;
      if (sp_info.ison)
	{
	  debug (bot, "sp_run: sending [%s]\n", new_str);
	  write (sp_info.pipefd[1], new_str, strlen (new_str));

	  sleep (1);

	  memset (buf, 0, sizeof (buf));

	  n = read (sp_info.pipefd_b[0], buf, sizeof (buf) - 1);
	  buf_ptr = strchr (buf, ':');
	  if (buf_ptr)
	    {
	      if (strlen (buf_ptr) > 4)
		{
		  strzero_bot (bot->txt_data_out);
		  strlcat_bot (bot->txt_data_out, buf);
		}
	      else
		{
		  strzero_bot (bot->txt_data_out);
		}
	    }
	  else
	    {
	      strzero_bot (bot->txt_data_out);
	    }

	}

      free (new_str);
    }



  return bot;
}


void
sp_fill_info (bot_t * bot)
{
  int i;
  char *str, *tok;
  char buf[1024];

  if (sp_info.initialized)
    return;


  if (sp_info.fp)
    {

      fclose (sp_info.fp);
      sp_info.fp = NULL;
    }


  str = str_unite_static ("%s/mods/mod_sp_files/conf", gi->confdir);

  if (!sNULL(str))
    goto cleanup;

  sp_info.fp = fopen (str, "r");


  if (!sp_info.fp)
    goto cleanup;

  str = NULL;

  while (1)
    {
      memset (buf, 0, sizeof (buf));
      if (fgets (buf, sizeof (buf) - 1, sp_info.fp) == NULL)
	break;

      strstrip_nl (buf);

      if (!strncasecmp (buf, "exec", 4))
	{

/* very simple argv builder */
	  tok = buf + strlen ("exec ");

	  sp_info.argc = 0;
	  sp_info.argv = (char **) calloc (sizeof (char *), 10);

	  while (1)
	    {
	      tok = strtok (tok, " ");
	      if (!tok)
		break;

	      sp_info.argv[sp_info.argc] = strdup (tok);
	      sp_info.argc++;
	      tok = NULL;
	    }

	}
      else if (!strncasecmp (buf, "dir", 3))
	{
	  sp_info.dir = strdup (&buf[strlen ("dir ")]);
	}
      else if (!strncasecmp (buf, "on", 2))
	{
	  sp_info.should_on = 1;
	}
      else if (!strncasecmp (buf, "off", 3))
	{
	  sp_info.should_on = 0;
	}

    }


  if (sp_info.argv)
    {
      for (i = 0; sp_info.argv[i] != NULL; i++)
	{
	  printf ("%i. %s\n", i, sp_info.argv[i]);
	}
    }


cleanup:

  return;
}







int
sp_run_on (bot_t * bot)
{
  pid_t pid;

  debug (bot, "sp_run_on: Entered\n");

  if (sp_info.ison)
    return -1;

  if (!sp_info.dir)
    return -1;

  if (pipe (sp_info.pipefd) < 0)
    {
      return -1;
    }

  if (pipe (sp_info.pipefd_b) < 0)
    {
      return -1;
    }

  fcntl (sp_info.pipefd_b[0], F_SETFL, O_NONBLOCK);

  debug(NULL, "sp_run_on: pipe0=%i, pipe1=%i\n", sp_info.pipefd[0], sp_info.pipefd[1]);


  if (!(pid = bot_fork_clean (bot)))
    {
      chdir (sp_info.dir);


      close (sp_info.pipefd_b[0]);
      dup2 (sp_info.pipefd_b[1], 1);
      dup2 (sp_info.pipefd_b[1], 2);

      close (sp_info.pipefd[1]);
      close (0);
      dup2 (sp_info.pipefd[0], 0);

      execve (sp_info.argv[0], sp_info.argv, environ);
      perror ("execve: ");
      exit (0);
    }
  else
    {
      char buf[10024];

      close (sp_info.pipefd[0]);
      close (sp_info.pipefd_b[1]);

      sp_info.ison = pid;

// FIX: dup 1/2 at start of myparser.java, move them to something else, then move them back after initialization, to avoid this
      read (sp_info.pipefd_b[0], buf, sizeof (buf) - 1);

      debug (bot, "sp_run_on: ison=%i\n", sp_info.ison);
    }

  return 0;
}

int
sp_run_off (bot_t * bot)
{
  int i;

  debug (bot, "sp_run_off: Entered\n");

  if (sp_info.ison)
    {
      kill (sp_info.ison, SIGTERM);
      sp_info.ison = 0;
    }

  if (sp_info.fp)
{
    fclose (sp_info.fp);
sp_info.fp=NULL;
}

  if (sp_info.argv)
    {
      for (i = 0; sp_info.argv[i] != NULL; i++)
	{
	  free (sp_info.argv[i]);
	}
      free (sp_info.argv);
sp_info.argv=NULL;
    }

  if (sp_info.dir)
    free (sp_info.dir);
sp_info.dir = NULL;

  return 0;
}









bot_t *
sp_run_info (bot_t * bot, char *str)
{
  int i;
  if (!bot)
    return NULL;

  debug (bot, "sp_run_info: Entered : str=%s\n", str);

  if (str)
    {
      if (strlen (str) == 0)
	str = NULL;
    }

  for (i = 0; penn_meanings[i].abrv != NULL; i++)
    {
      if (str)
	{
	  if (!strcasecmp (penn_meanings[i].abrv, str))
	    {
	      snprintf_buf (bot->txt_data_out,
			     "<<%s>,<%s>,<%s>>\n", penn_meanings[i].abrv,
			     penn_meanings[i].meaning,
			     penn_meanings[i].example);
	    }
	}
      else
	{
	}

    }


  return bot;
}
