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
#ifndef MOD_PRINT_H
#define MOD_PRINT_H

#include "bot.h"

module_t mod_print_info;

bot_t *print_init (dlist_t *, bot_t *);
bot_t *print_fini (dlist_t *, bot_t *);
bot_t *print_help (dlist_t *, bot_t *);
bot_t *print_run (dlist_t *, bot_t *);

char *print_change_string (char *, int, int, int);


void __print_init__ (void) __attribute__ ((constructor));


enum mod_print_type
{
  PRINT_OPT_BINARY = 1,
  PRINT_OPT_HEX,
  PRINT_OPT_CHAR,
  PRINT_OPT_REVERSE,
  PRINT_OPT_FORWARD
};


char *print_op_binary (int, int, char *);

char *print_op_binary_forward (int, char *);
char *print_op_binary_forward_char (char *);

char *print_op_binary_reverse (int, char *);
char *print_op_binary_reverse_char (char *);

#endif
