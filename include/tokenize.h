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
#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "bot.h"

/* tokenize normal, ..
 * leavequotes, leave " characters inside the tokens
 * leavesep, leave the separate in the token (at the end)
 * noquotes, take the quotations out
 * eatwhitespace, eat whitespace of each token
 */

enum tokenize_opts {
TOKENIZE_NORMAL=0x01,
TOKENIZE_LEAVEQUOTES=0x02,
TOKENIZE_LEAVESEP=0x04,
TOKENIZE_NOQUOTES=0x08,
TOKENIZE_EATWHITESPACE=0x10,
TOKENIZE_IGNORECASE=0x20,
TOKENIZE_MATCHANY=0x40,
TOKENIZE_FINDQUOTES=0x80,
};

enum tokenize_argv_stuff {
TOKENIZE_STR2ARGV_ARGV0 = 0x01,
};

dlist_t * tokenize(bot_t *, char *, int, char *);

void tokenize_destroy(bot_t *, dlist_t **);

char **tokenize_array(bot_t *, char *, int, char *, int *);

void tokenize_destroy_array(bot_t *, char **);

char * tokenize_find_closing_bracket(char *, int c);

char * tokenize_find_inside_of_brackets (char *, int) ;

char ** tokenize_str2argv(char *, int *, int);



enum tokenize_sort_strings_types {
TOKENIZE_SORT_STRINGS_FORWARD=0x01,
TOKENIZE_SORT_STRINGS_BACKWARD=0x02,
TOKENIZE_SORT_STRINGS_RANDWARD=0x04,
TOKENIZE_SORT_STRINGS_UNIQ=0x08,
};

void tokenize_sort_strings(char **, int *, int);

#endif
