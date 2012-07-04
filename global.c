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


void
global_defaults (void)
{

  gi = &global_info;

  global_info.pid = getpid ();
  global_info.chroot = 1;

  global_info.chroot_uid = 1000;
  global_info.chroot_gid = 1000;

  global_info.ev_base = event_init ();

  global_info.fd_console = 0;

  global_info.var_timer_sleep = 30;
  global_info.var_timer_sort = 30;
  global_info.var_sigprotect = 1;

  timer_set ();

  safe_event_set (&global_info.ev_console, global_info.fd_console,
	     EV_READ | EV_PERSIST, bot_evhook_console, NULL);
  safe_event_add (&global_info.ev_console, NULL);

global_info.var_xmod_style = XMODULE_STYLE_LIST;

  return;
}


void
global_conf_parse (void)
{
  FILE *fp;
  char buf[MAX_BUF_SZ], *buf_ptr;
  int add;


debug(NULL, "global_conf_parse: Entered\n");

  if (!sNULL(global_info.conf_dir))
    {
      strlcpy_buf(global_info.conf_dir,global_conf_dir ());
    }

  if (!sNULL(global_info.conf_dir))
    {
      debug_err (NULL, "global_conf_parse: conf_dir=NULL\n");
    }

  if (!sNULL(global_info.conf_file))
    {
      strlcat_buf(global_info.conf_file,
	str_unite_static ("%s/%s", global_info.conf_dir, BOT_DEF_CONFFILE));
    }

  fp = fopen (global_info.conf_file, "r");
  if (!fp)
    {
      debug_err (NULL, "global_conf_parse: unable to open %s\n",
	       global_info.conf_file);

    }

  debug (NULL, "global_conf_parse: opened %s\n",
	     global_info.conf_file);

  while (1)
    {
      bz (buf);
      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;

debug(NULL, "%s", buf);

      strstrip_nl (buf);
      if (buf[0] == '+')
	add = 1;
      else if (buf[0] == '-')
	add = 0;
      else
	continue;

      buf_ptr = (buf + 1);

      if (!strncasecmp_len (buf_ptr, "chroot_uid"))
	{
	  global_info.chroot_uid =
	    atoi (&buf_ptr[fn_strlen ("chroot_uid") + 1]);
	  debug (NULL, "global_conf_parse: chroot_uid=%i\n",
		     global_info.chroot_uid);
	}
      else if (!strncasecmp_len (buf_ptr, "chroot_gid"))
	{
	  global_info.chroot_gid =
	    atoi (&buf_ptr[fn_strlen ("chroot_gid") + 1]);
	  debug (NULL, "global_conf_parse: chroot_gid=%i\n",
		     global_info.chroot_gid);
	}
      else if (!strncasecmp_len (buf_ptr, "chroot_dir"))
	{

	  strlcat_buf(global_info.chroot_dir, &buf_ptr[fn_strlen ("chroot_dir") + 1]);
	  debug (NULL, "global_conf_parse: chroot_dir=%s\n", global_info.chroot_dir);
	}
      else if (!strncasecmp_len (buf_ptr, "chroot"))
	{
	  global_parse_chroot (&buf_ptr[fn_strlen ("chroot") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "confdir"))
	{

	  strlcat_buf(global_info.confdir, &buf_ptr[fn_strlen ("confdir") + 1]);
	  debug (NULL, "global_conf_parse: confdir=%s\n", global_info.confdir);
	}
      else if (!strncasecmp_len (buf_ptr, "modules"))
	{
	  global_parse_modules (&buf_ptr[fn_strlen ("modules") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "pmodules"))
	{
	  global_parse_pmodules (&buf_ptr[fn_strlen ("pmodules") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "gmodules"))
	{
	  global_parse_gmodules (&buf_ptr[fn_strlen ("gmodules") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "trace"))
	{
	  global_parse_trace (&buf_ptr[fn_strlen ("trace") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "debug"))
	{
	  global_parse_debug (&buf_ptr[fn_strlen ("debug") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "sigprotect"))
	{
	  global_parse_sigprotect (&buf_ptr[fn_strlen ("sigprotect") + 1],
				       1);
	}
      else if (!strncasecmp_len (buf_ptr, "timer_sleep"))
	{
	  global_parse_timer_sleep (&buf_ptr
					[fn_strlen ("timer_sleep") + 1], 1);
	}
      else if (!strncasecmp_len (buf_ptr, "timer_sort"))
	{
	  global_parse_timer_sort (&buf_ptr[fn_strlen ("timer_sort") + 1],
				       1);
	}
      else if (!strncasecmp_len (buf_ptr, "fd_unix_path"))
	{
	  global_parse_fd_unix_path (&buf_ptr
					 [fn_strlen ("fd_unix_path") + 1], 1);
	}
else if(!strncasecmp_len(buf_ptr, "xmod_style")) {
xmodule_parse_style(&buf_ptr[fn_strlen("xmod_style")+1]);
}

    }


  fclose (fp);

  debug (NULL, "global_conf_parse: Success\n");


  return;
}









char *
global_conf_dir (void)
{
  struct stat st;
  int n=0;
  char *str=NULL, *home=NULL;

  home = getenv ("HOME");
  if (home)
    {

      str = str_unite_static ("%s/darqbot", home);

      debug (NULL, "global_conf_dir: trying str=%s\n", str);

      n = stat (str, &st);
      if (n == 0)
	return str;
    }

  str = str_unite_static ("./darqbot");

  debug (NULL, "global_conf_dir: trying str=%s\n", str);

  n = stat (str, &st);
  if (n == 0)
    return str;

  str = str_unite_static ("/darqbot/");

  debug (NULL, "global_conf_dir: trying str=%s\n", str);

  n = stat (str, &st);
  if (n == 0)
    return str;

  str = str_unite_static ("/");

  debug (NULL, "global_conf_dir: trying str=%s\n", str);

  n = stat (str, &st);
  if (n == 0)
    return str;

  return NULL;
}






int
global_chroot (void)
{
  gid_t gidlist[2];

  debug (NULL, "global_chroot: Entered\n");


  if (global_info.chroot)
    {
      if (chroot (global_info.chroot_dir) < 0)
	{
	  debug_err (NULL, "global_chroot: chroot Failed\n");
	  return -1;
	}
    }

  chdir ("/");

  gidlist[0] = global_info.chroot_gid;
  setgroups (1, (gid_t *) & gidlist);
  setgid (global_info.chroot_gid);
  setuid (global_info.chroot_uid);
  setreuid (global_info.chroot_uid, global_info.chroot_uid);
  setreuid (global_info.chroot_gid, global_info.chroot_gid);

  if (getuid () != global_info.chroot_uid)
    {
      debug_err (NULL, "global_chroot: setreU/Gid failed\n");
      return -1;
    }

  global_core_unlimited ();

  debug (NULL, "global_chroot: Dropped privs, success\n");


  return 0;
}







void
global_core_unlimited (void)
{
  struct rlimit rlim;

  rlim.rlim_cur = RLIM_INFINITY;
  rlim.rlim_max = RLIM_INFINITY;

  if (setrlimit (RLIMIT_CORE, &rlim) < 0)
    {
      debug (NULL, "global_core_unlimited: setrlimit: Failed\n");
    }

  return;
}







void
global_signal_hooks (void)
{
  int i;
/*
  signal (SIGFPE, bot_sigend_handler);
  signal (SIGSEGV, bot_sigend_handler);
  signal (SIGINT, bot_sigend_handler);
  signal (SIGPIPE, bot_sigpipe_handler);
  signal (SIGCHLD, bot_sigchld_handler);
  signal (SIGALRM, bot_exit_handler);
*/

  for (i = 0; i < _NSIG; i++)
    {
      signal (i, bot_sigend_handler);
    }

  signal (SIGPIPE, bot_sigpipe_handler);
  signal (SIGCHLD, bot_sigchld_handler);
  signal (SIGALRM, bot_exit_handler);

  atexit (bot_atexit_handler);
  return;
}




void
global_signal_hooks_child (void)
{
  int i;
/*
  signal (SIGFPE, bot_sigend_handler);
  signal (SIGSEGV, bot_sigend_handler);
  signal (SIGINT, bot_sigend_handler);
  signal (SIGPIPE, bot_sigpipe_handler);
  signal (SIGCHLD, bot_sigchld_handler);
  signal (SIGALRM, bot_exit_handler);
*/

  for (i = 0; i < _NSIG; i++)
    {
      signal (i, SIG_DFL);
    }

  signal (SIGPIPE, bot_sigpipe_handler);
  signal (SIGCHLD, bot_sigchld_handler);
  signal (SIGALRM, bot_exit_handler);

  atexit (bot_atexit_handler);
  return;
}








void
global_signal_exit_hooks (void)
{
  int i;
/*
  signal (SIGFPE, bot_exit_handler);
  signal (SIGSEGV, bot_exit_handler);
  signal (SIGINT, bot_exit_handler);
  signal (SIGUSR2, bot_exit_handler);
  signal (SIGPIPE, bot_exit_handler);
  signal (SIGCHLD, bot_exit_handler);
  signal (SIGALRM, bot_exit_handler);
*/

  for (i = 0; i < _NSIG; i++)
    {
      signal (i, bot_exit_handler);
    }

  atexit (bot_atexit_handler);

  return;
}







void
global_off (void)
{
  dlist_t *dptr_1, *dptr_2;
  bot_t *bot;

  bot_unix_fini ();

  dlist_fornext_retarded (global_info.bots, dptr_1, dptr_2)
  {
    if (!dptr_1)
      break;

    bot = (bot_t *) dlist_data (dptr_1);

    bot_fini (bot);

  }

  global_info.bots = NULL;

  bz(gi->conf_dir);

  bz(gi->conf_file);

  bz(gi->confdir );

  bz(gi->chroot_dir );

  bz(gi->fd_unix_path);

dlist_fini(&gi->dl_module, xmodule_free_destroy);
dlist_fini(&gi->dl_pmodule,xmodule_free_destroy);
dlist_fini(&gi->dl_gmodule,xmodule_free_destroy);
dlist_fini(&gi->dl_mod_timers, xmodule_free_destroy);
dlist_fini(&gi->dl_gmod_gtimers, xmodule_free_destroy);
dlist_fini(&gi->dl_mod_iohooks,xmodule_free_destroy);
dlist_fini(&gi->dl_gmod_giohooks,xmodule_free_destroy);

  mongodb_fini (NULL);

  gi->on = 0;


if(gi->dl_module_str)
free(gi->dl_module_str);

gi->dl_module_str=NULL;

if(gi->dl_pmodule_str)
free(gi->dl_pmodule_str);

gi->dl_pmodule_str=NULL;

if(gi->dl_gmodule_str)
free(gi->dl_gmodule_str);

gi->dl_gmodule_str=NULL;


if(gd->trace_fp)
fclose(gd->trace_fp);
gd->trace_file=NULL;

if(gd->trace_file)
free(gd->trace_file);
gd->trace_file = NULL;

  return;
}



void
global_on (void)
{
  DIR *dir;
  struct dirent *de_ptr;
  bot_t *bot;

  if (!sNULL(gi->conf_dir))
    global_conf_parse ();

  mongodb_init (NULL);

  module_load_all ();
  pmodule_load_all ();
  gmodule_load_all ();

  timer_sort_modules ();

  dir = opendir (gi->confdir);
  if (!dir)
    {
      debug (NULL, "global_on: Could not find conf directory\n");
      return;
    }

  if (bot_unix_init () < 0)
    {
      debug (NULL, "global_on: bot_unix_init: Failed\n");
closedir(dir);
      return;
    }

  gi->on = 1;

  while (1)
    {

      de_ptr = readdir (dir);
      if (de_ptr == NULL)
	break;

      if (!xstrcasestr (de_ptr->d_name, ".conf"))
	continue;

bot = bot_init_and_turn_on(de_ptr->d_name);
    }


closedir(dir);

  return;
}







void
global_fork_clean_stuff (void)
{

/*
event_del(&global_info.ev_console);
close(global_info.fd_console);

event_del(&global_info.ev_unix);
close(global_info.fd_unix);
*/

  return;
}













int
global_parse_chroot (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  if (!strcasecmp (ptr_1, "on"))
    {
      gi->chroot = 1;
    }
  else
    {
      gi->chroot = 0;
    }

  return 0;
}



int
global_parse_modules (char *str, int add)
{

debug(NULL, "global_parse_modules: Entered\n");

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

if(gi->dl_module_str)
free(gi->dl_module_str);

gi->dl_module_str = strdup(str);

  return 0;
}








int
global_parse_pmodules (char *str, int add)
{

debug(NULL, "global_parse_pmodules: Entered\n");

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

if(gi->dl_pmodule_str)
free(gi->dl_pmodule_str);

gi->dl_pmodule_str = strdup(str);


  return 0;
}






int
global_parse_gmodules (char *str, int add)
{

debug(NULL, "global_parse_gmodules: Entered\n");

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

if(gi->dl_gmodule_str)
free(gi->dl_gmodule_str);

gi->dl_gmodule_str = strdup(str);

  return 0;
}


int
global_parse_trace (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  if (!strncasecmp_len (str, "all"))
    {
      gd->trace = DEBUG_TRACE_ALL;
    }
  else if (!strncasecmp_len (str, "only"))
    {
      gd->trace = DEBUG_TRACE_ONLY;
    }
  else
    {
      gd->trace = DEBUG_TRACE_OFF;
    }

  return 0;
}


int
global_parse_debug (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  if (!strncasecmp_len (str, "all"))
    {
      gd->debug = DEBUG_DEBUG_ALL;
    }
  else if (!strncasecmp_len (str, "only"))
    {
      gd->debug = DEBUG_DEBUG_ONLY;
    }
  else
    {
      gd->debug = DEBUG_DEBUG_OFF;
    }

  return 0;
}





int
global_parse_sigprotect (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  if (!strncasecmp_len (str, "on"))
    {
      gi->var_sigprotect = 1;
    }
  else
    {
      gi->var_sigprotect = 0;
    }

  return 0;
}







int
global_parse_timer_sleep (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  gi->var_timer_sleep = atoi (str);
  if (gi->var_timer_sleep <= 0)
    gi->var_timer_sleep = 30;

  return 0;
}





int
global_parse_timer_sort (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  gi->var_timer_sort = atoi (str);
  if (gi->var_timer_sort <= 0)
    gi->var_timer_sort = 600;

  return 0;
}






int
global_parse_fd_unix_path (char *str, int add)
{
  char *ptr_1;

  if (!sNULL(str))
    return -1;

  if (add == 0)
    {				/*remove entry; */
      return 0;
    }

  ptr_1 = str;

  strlcat_buf(gi->fd_unix_path ,ptr_1);

  return 0;
}








void
global_set_proc_ptrs (int *argc, char **argv, char **env)
{
  gi->argv = argv;
  gi->argc = argc;
  gi->env = env;
  return;
}






void
global_gcmd_add (void (*fn) (void *), void *data)
{
  gi->gcmd = fn;
}

void
global_gcmd (void)
{
  void (*fn) (void *);

  fn = gi->gcmd;
  if (fn)
    {
      gi->gcmd = NULL;
      fn (NULL);
    }
  return;
}









void
global_getopt (int argc, char **argv)
{
  int opt;

  if (argc < 0 || !argv)
    return;

  while ((opt = getopt (argc, argv, "d:f:u:")) != -1)
    {
      switch (opt)
	{
	case 'd':
	  {
/* global conf dir */
	    strlcat_buf(gi->conf_dir,optarg);
	    break;
	  }
	case 'f':
	  {
/* global conf file */
	    strlcat_buf(gi->conf_file ,optarg);
	    break;
	  }
	case 'u':
	  {
/* global unix fdpassing socket path */
	    strlcat_buf(gi->fd_unix_path,optarg);
	    break;
	  }
	default:
	  break;
	}
    }


  return;
}
