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

int
debug_err (bot_t * bot, const char *msg, ...)
{
  va_list ap;

  if (!gd)
    return 0;

  if (!gd->debug)
    return 0;

  if (gd->debug == DEBUG_DEBUG_ONLY)
    {
      if (!gi)
	return 0;
      if (!gi->bot_current)
	return 0;
      if (!gi->bot_current->debug)
	return 0;
    }

  va_start (ap, msg);

  if (bot)
    {
      if (bot->logfile)
	{
	  FILE *fp;
	  fp = fopen (bot->logfile, "a+");
	  if (fp)
	    {
	      fprintf (fp, "debug_err: ");
	      vfprintf (fp, msg, ap);
	      va_end (ap);
	      exit (-1);
	    }
	}
    }

  printf ("debug_err: ");
  vprintf (msg, ap);
  va_end (ap);

  exit (-1);
}



int
debug (bot_t * bot, const char *msg, ...)
{
/* print a debug message, either to stdout or bot->logfile */
  va_list ap;
  int n;
  char buf[MAX_BUF_SZ + 1];

char * argv0=NULL;

/* FUNK: eventually add console opts to this, BOT_CONSOLE_OPT_LINK etc */

  if (!msg)
    return -1;

  if (!gd)
    return 0;

  if (!gd->debug)
    return 0;

  if (gd->debug == DEBUG_DEBUG_ONLY)
    {
      if (!gi)
	return 0;
      if (!gi->bot_current)
	return 0;
      if (!gi->bot_current->debug)
	return 0;
    }

  va_start (ap, msg);
  n = vsnprintf_buf (buf, msg, ap);

  va_end (ap);

  if (n > 0)
    buf[n] = '\0';

  if (bot)
    {
      if (bot->logfile)
	{
	  FILE *fp;
	  fp = fopen (bot->logfile, "a+");
	  if (fp)
	    {
	      fprintf (fp, "debug: ");
	      fprintf (fp, "%s", buf);
	      fflush (fp);
	      fclose (fp);
	      return 0;
	    }

	}
    }

if(gi->argv) {
if(gi->argv[0]) {
argv0  = gi->argv[0];
}
}

if(!argv0)
argv0 = "argvNULL";

  printf ("[%s](%i) debug: ", argv0, gi->pid_child > 0? gi->pid_child : gi->pid);
  printf ("%s", buf);

if(gi->bot_current) {
if(gi->bot_current->brake) {
char buf[8];
while(1) {
//if(c == '\n') break;
bz(buf);
if(fgets(buf,sizeof(buf)-1,stdin)==NULL) break;
if(!strncasecmp_len(buf, "off")) { gi->bot_current->brake = 0; break; }
}
}
}

  return n;
}






int
debug_trace (void **array, size_t count)
{
  char **strings;
  int tab;

  if (!array || count <= 0)
    return -1;

  if (!gd->trace_fp)
    return -1;

  strings = backtrace_symbols (array, count);
  if (!strings)
    return -1;

  for (tab = 0; tab < global_debug.nesting; tab++)
    {
      fprintf (gd->trace_fp, " ");
    }

  fprintf (gd->trace_fp, "%s\n", strings[0]);
  fflush (gd->trace_fp);

  free (strings);

  return 0;
}



void debug_trace_init_mandatory(void) {
/* this can be run outside of chroot or inside */
  gd = &global_debug;

return;
}

void
debug_trace_init (void)
{
  time_t t;

  if (gd->trace_file)
    free (gd->trace_file);

  gd->trace_file = strdup (DEBUG_BACKTRACE_DEFAULT);

  if (gd->trace_fp)
    fclose (gd->trace_fp);

/* FIX THIS, call this func again after chroot, with new path, make that 'a' for appending */
  gd->trace_fp = fopen (gd->trace_file, "w");

  if (gd->trace_fp)
    {
      time (&t);
      fprintf (gd->trace_fp,
	       "# debug_trace_init: Initiated: pid=%i, ctime=%s",
	       getpid (), ctime (&t));
    }

  setvbuf (gd->trace_fp, NULL, _IONBF, 0);

  gd->trace = DEBUG_TRACE_ALL;
  gd->debug = DEBUG_DEBUG_ALL;

  return;
}








void
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
  time_t t;
  void *array[1];
  char **strings;

  if (!gd)
    return;

  gd->nesting++;

  if (!gd->trace)
    return;

  if (gd->trace == DEBUG_TRACE_ONLY)
    {
      if (!gi->bot_current)
	return;

      if (gi->bot_current->trace == 0)
	return;
    }

  time (&t);

  array[0] = this_fn;
  strings = backtrace_symbols (array, 1);

  if (!strings)
    return;


  if (gd->trace_fp)
    fprintf (gd->trace_fp, "%i:%li:enter:%s\n", gd->nesting, t, strings[0]);

  free (strings);

  return;
}

void
__cyg_profile_func_exit (void *this_fn, void *call_site)
{
  time_t t;
  void *array[1];
  char **strings;

  if (!gd)
    return;

  gd->nesting--;

  if (!gd->trace)
    return;

  if (gd->trace == DEBUG_TRACE_ONLY)
    {
      if (!gi->bot_current)
	return;

      if (gi->bot_current->trace == 0)
	return;
    }

  time (&t);

  array[0] = this_fn;
  strings = backtrace_symbols (array, 1);

  if (!strings)
    return;

  if (gd->trace_fp)
    fprintf (gd->trace_fp, "%i:%li:exit:%s\n", gd->nesting, t, strings[0]);

  free (strings);

  return;
}





void
debug_set_cur (bot_t * bot, void *addr)
{
  if (!bot)
    return;

  if (!gi)
    return;

  gi->bot_current = bot;

/* this might wreck nesting.. equalize.. */
  if (gd->trace == DEBUG_TRACE_ONLY)
    __cyg_profile_func_enter (addr, NULL);
  gd->nesting--;

  return;
}







void
debug_print_backtrace (void)
{

  void *bt_array[20 + 1];
  char **bt_strings = NULL;
  size_t bt_size;

  int i;

  memset (bt_array, 0, 20 * sizeof (void *));
  bt_size = backtrace (bt_array, 20);

  bt_strings = backtrace_symbols (bt_array, bt_size);
  if (!bt_strings)
    return;

  for (i = 0; i < bt_size; i++)
    {
      debug (NULL, "%s\n", bt_strings[i]);
      fflush (stdout);
    }

  free (bt_strings);

  return;
}









int
bot_dl_print (bot_t * bot, const char *msg, ...)
{
  va_list ap;
  char buf[MAX_BUF_SZ + 1];
  int n;

  if (!msg)
    return -1;

  va_start (ap, msg);

  n = vsnprintf_buf (buf, msg, ap);
  va_end (ap);


  if (n > 0)
    {
      buf[n] = '\0';
      dlist_Dinsert_after (&gi->dl_print, strdup (buf));
    }

  return n;
}



void
bot_dl_print_flush (bot_t * bot)
{
  dlist_t *dptr;
  char *str;

  if (!gi->dl_print)
    return;

  dlist_fornext (gi->dl_print, dptr)
  {
    str = (char *) dlist_data (dptr);
    debug (bot, "%s", str);
  }

  dptr = dlist_tail (gi->dl_print);
  if (dptr)
    {
      str = (char *) dlist_data (dptr);

      if (str[strlen (str) - 1] != '\n')
	debug (bot, "\n");
    }

  dlist_fini (&gi->dl_print, free);

  return;
}



char *
bot_dl_print_flush_str (bot_t * bot)
{
  dlist_t *dptr;
  char *s1, *s2, *tmp_str;

  if (!gi->dl_print)
    return NULL;

  s1 = s2 = NULL;

  dlist_fornext (gi->dl_print, dptr)
  {
    s2 = (char *) dlist_data (dptr);

    if (!s2)
      continue;

    if (!s1)
      {
	s1 = strdup (s2);
      }
    else
      {
	tmp_str = s1;
	s1 = str_unite_foralloc ("%s%s", s1, s2);
	if (!s1)
	  {
	    s1 = tmp_str;
	  }
	else
	  {
	    free (tmp_str);
	  }
      }
  }

  dlist_fini (&gi->dl_print, free);

  return s1;
}


void
bot_dl_print_change_strings (bot_t * bot, char *s1, char *s2)
{
  dlist_t *dptr;
  char *str, *sub_str;

  if (!s1 || !s2)
    return;

  if (!strcmp (s1, s2))
    return;

  if (strlen (s1) != strlen (s2))
    return;

  dlist_fornext (gi->dl_print, dptr)
  {
    str = (char *) dlist_data (dptr);
    if (!str)
      continue;
    sub_str = str;

    while (1)
      {
	sub_str = strstr (sub_str, s1);
	if (!sub_str)
	  break;

	memcpy (sub_str, s2, strlen (s2));
      }

  }

  return;
}


void
bot_dl_print_destroy (bot_t * bot)
{

  if (!gi->dl_print)
    return;

  dlist_fini (&gi->dl_print, free);

  return;
}






void
debug_hexdump_fp (FILE * fp, int width, unsigned char *mem,
		      unsigned int len)
{
  int x;
  int y;

  if (!mem || width < 0 || len < 0)
    return;

  x = y = 0;

  fprintf (fp, "-> begin hex dump:\n");

  if (width <= 0)
    width = 6;

  y = 0;
  for (x = 0; x < len; x++)
    {
      if (y == width)
	{
	  fprintf (fp, "\n");
	  y = 0;
	}
      fprintf (fp, "%.2x  ", (unsigned char) mem[x]);
      y += 1;
    }
  fprintf (fp, "\n");

  fprintf (fp, "-> end hex dump:\n");

  return;
}
