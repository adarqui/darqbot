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
#ifndef MOD_DUMBGAMES_H
#define MOD_DUMBGAMES_H

#include "bot.h"


module_t mod_dumbgames_info;

bot_t *dumbgames_init (dlist_t *, bot_t *);
bot_t *dumbgames_fini (dlist_t *, bot_t *);
bot_t *dumbgames_help (dlist_t *, bot_t *);
bot_t *dumbgames_run (dlist_t *, bot_t *);

void __dumbgames_init__ (void) __attribute__ ((constructor));

enum dumbgames_sub_types {
MOD_DUMBGAMES,
MOD_DUMBGAMES_DICE,
MOD_DUMBGAMES_ROULETTE,
};

char * dice_change_string(int);
char * roulette_change_string(int);

#endif
