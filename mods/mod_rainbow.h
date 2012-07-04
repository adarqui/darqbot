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
#ifndef MOD_RAINBOW_H
#define MOD_RAINBOW_H

#include "bot.h"


#define RC_MAX 30


enum opts
{
  RAINBOW_OPT_RAND = 0x01,
  RAINBOW_OPT_NOSPACE = 0x02,
  RAINBOW_OPT_NOCHAR = 0x04,
}
 ;

typedef struct rainbow_colors
{
  char *val;
  char *type;
} rainbow_colors_t;


module_t mod_rainbow_info;

bot_t *rainbow_init (dlist_t *, bot_t *);
bot_t *rainbow_fini (dlist_t *, bot_t *);
bot_t *rainbow_help (dlist_t *, bot_t *);
bot_t *rainbow_run (dlist_t *, bot_t *);


char *rainbow_change_string (char *, int, rainbow_colors_t *, int);
int rainbow_defaults (rainbow_colors_t *, int *, char *);
int rainbow_add_colors (dlist_t *, rainbow_colors_t *, int *, char *);

void __rainbow_init__ (void) __attribute__ ((constructor));

#endif
