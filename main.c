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
main (int argc, char *argv[])
{

/* fn pointers */
  gi = &global_info;

  fns_load_defaults ();

  debug_trace_init_mandatory ();
/* XXX
  debug_trace_init ();
*/

/* XXX
  bot_alloc_on ();
  bot_alloc_verbose_on ();
*/

  srand (getpid ());
  SSL_library_init ();
  SSL_load_error_strings ();
//  OpenSSL_add_all_algorithms ();

  setlocale (LC_ALL, "");

  global_signal_hooks ();

  global_defaults ();


  global_getopt (argc, argv);

  global_set_proc_ptrs (&argc, argv, environ);

  global_conf_parse ();

  if (global_chroot () < 0)
    {
      debug_err (NULL, "main: bot_chroot: Failed\n");
    }


  global_on ();

/*
  event_dispatch ();
*/
  setjmp (gi->sigprotect_buf);
  sigsetjmp (gi->sigprotect_sigbuf, 1);

  while (1)
    {

/*trying to fix gmod_grelinkd reload */
/*
global_gcmd();
*/

      gi->bot_current = NULL;
      pmodule_cur_clear ();
      event_loop (EVLOOP_ONCE);

      timer_shouldwerun ();

    }


  return 0;
}
