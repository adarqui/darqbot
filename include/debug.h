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
#ifndef DEBUG_H
#define DEBUG_H

#include "bot.h"


#define DEBUG_BACKTRACE_DEFAULT "/darqbot/.darqbot/debug/trace"

enum bot_trace_stuff {
DEBUG_TRACE_OFF=0,
DEBUG_TRACE_ALL=1,
DEBUG_TRACE_ONLY=2,
};

enum debug_stuff {
DEBUG_DEBUG_OFF=0,
DEBUG_DEBUG_ALL=1,
DEBUG_DEBUG_ONLY=2,
};


typedef struct debug_info {
int nesting;

char * trace_file;
FILE * trace_fp;

int trace;
int debug;

} debug_info_t;

debug_info_t global_debug;
debug_info_t * gd;


void debug_set_cur(bot_t *, void *);

int debug_err (bot_t *, const char *, ...) __attribute__((no_instrument_function));
int debug (bot_t *, const char *, ...) __attribute__((no_instrument_function));
int debug_trace(void **, size_t) __attribute__((no_instrument_function));

void debug_trace_init_mandatory(void) __attribute__((no_instrument_function));
void debug_trace_init(void) __attribute__((no_instrument_function));
void debug_trace_fini(void) __attribute__((no_instrument_function));

void debug_print_backtrace(void) __attribute__((no_instrument_function));

int bot_dl_print(bot_t *, const char *, ...) __attribute__((no_instrument_function));

void bot_dl_print_flush(bot_t *);
char * bot_dl_print_flush_str(bot_t *);
void bot_dl_print_destroy(bot_t *);
void bot_dl_print_change_strings(bot_t *, char *, char * ) ;

void
debug_hexdump_fp (FILE *, int, unsigned char *, unsigned int);

void __cyg_profile_func_enter (void *this_fn, void *call_site)
  __attribute__ ((no_instrument_function));

void __cyg_profile_func_exit (void *this_fn, void *call_site)
  __attribute__ ((no_instrument_function));


/*
#define ENTER if(1) { \
  void *_l_array[2]; size_t _l_size; \
  _l_size = backtrace(_l_array,1); \
  if(_l_size > 0) { \
   debug_backtrace(_l_array,_l_size); \
   } \
  global_debug.nesting++; \
 }
*/

/*
   _l_strings = backtrace_symbols(_l_array, _l_size); \
   for(tab=0;tab<=global_nesting;tab++) printf(" "); \
    printf("%s: Entered\n", strings[0]); \
   global_nesting++; } }
*/

/*
#define return_void if(1) { global_debug.nesting--; return; }
#define return_val(a) if(1) { global_debug.nesting--; return((a)); }
*/


#endif
