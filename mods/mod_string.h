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
#ifndef MOD_STRING_H
#define MOD_STRING_H

#include "bot.h"


module_t mod_string_info;

bot_t *string_init (dlist_t *, bot_t *);
bot_t *string_fini (dlist_t *, bot_t *);
bot_t *string_help (dlist_t *, bot_t *);
bot_t *string_run (dlist_t *, bot_t *);

void __string_init__ (void) __attribute__ ((constructor));

enum mod_string_opt { 
MOD_STRING_OPT_BEFORE=0,
MOD_STRING_OPT_AFTER,
};

enum mod_string_subs {
MOD_STRING,
MOD_STRING_STRZERO,
MOD_STRING_STRLEN,
MOD_STRING_STRCHR,
MOD_STRING_STRRCHR,
MOD_STRING_STRSTR,
MOD_STRING_STRCASESTR,
MOD_STRING_INDEX,
MOD_STRING_RINDEX,
MOD_STRING_SUBCHAR,
MOD_STRING_MEMSET,
MOD_STRING_STRPREPEND,
MOD_STRING_SPACE,
MOD_STRING_REVERSE,
MOD_STRING_UNQUOTE,
};

char * string_op_strzero(bot_t *, char *, char *, int);
char * string_op_strlen(bot_t *, char *, char *, int);

char * string_op_AstrAint(bot_t *, char * , char * , int , char *(*fn)(const char *, int)) ;
char * string_op_AstrAstr(bot_t *, char * , char * , int , char *(*fn)(const char *, const char *)) ;

char * string_op_space(bot_t *, char *, char *, int);
char * string_op_subchar(bot_t *, char *, char *, int);
char * string_op_reverse(bot_t *, char *, char *, int);
char * string_op_unquote(bot_t *, char *, char *, int);

char * string_strprepend(const char *, const char *);
char * string_memset(const char *, int);
int subchar_get_char(char *, int *);

#endif
