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
#ifndef GMOD_GRELAY_H
#define GMOD_GRELAY_H

#include "bot.h"

module_t gmod_grelay_info;

bot_t *grelay_init(dlist_t *, bot_t *);
bot_t *grelay_fini(dlist_t *, bot_t *);
bot_t *grelay_help(dlist_t *, bot_t *);
bot_t *grelay_run(dlist_t *, bot_t *);
bot_t *grelay_control(dlist_t *, bot_t *, int, va_list);

bot_t *grelay_output(dlist_t *, bot_t *);
bot_t *grelay_input(dlist_t *, bot_t *);

bot_t *grelay_off(dlist_t *, bot_t *);
bot_t *grelay_destroy_up(dlist_t *, bot_t *);
bot_t *grelay_destroy_down(dlist_t *, bot_t *);
bot_t *grelay_control_up(dlist_t *, bot_t *);
bot_t *grelay_control_down(dlist_t *, bot_t *);

char *grelay_change_string(bot_t *, char *, int);

void __grelay_init__(void) __attribute__ ((constructor));

#endif
