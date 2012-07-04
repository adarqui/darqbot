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
#ifndef MOD_NOP_H
#define MOD_NOP_H

#include "bot.h"


module_t mod_nop_info;

bot_t *mod_nop_init (dlist_t *, bot_t *);
bot_t *mod_nop_fini (dlist_t *, bot_t *);
bot_t *mod_nop_help (dlist_t *, bot_t *);
bot_t *mod_nop_run (dlist_t *, bot_t *);
bot_t *mod_nop_output (dlist_t *, bot_t *);
bot_t *mod_nop_input (dlist_t *, bot_t *);


char *mod_nop_out_str_prepend;
char *mod_nop_out_str_append;
char *mod_nop_in_str_prepend;
char *mod_nop_in_str_append;
int mod_nop_out_prob;
int mod_nop_in_prob;


/* ^nop(match) #jumping #bosshogs poopface <-- will only work for #jumping, #bosshogs, or poopface pm */
char *mod_nop_chan_match;

void __nop_init__ (void) __attribute__ ((constructor));


#endif
