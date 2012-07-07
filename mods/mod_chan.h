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
#ifndef MOD_CHAN_H
#define MOD_CHAN_H

#include "bot.h"

module_t mod_chan_info;

bot_t *mod_chan_init(dlist_t *, bot_t *);
bot_t *mod_chan_fini(dlist_t *, bot_t *);
bot_t *mod_chan_help(dlist_t *, bot_t *);
bot_t *mod_chan_run(dlist_t *, bot_t *);
bot_t *mod_chan_output(dlist_t *, bot_t *);
bot_t *mod_chan_input(dlist_t *, bot_t *);

char *chan_change_string(bot_t *, char *, int, char *);
char *chan_list(bot_t *, char *);
char *chan_users(bot_t *, char *);
char *chan_topic(bot_t *, char *, char *);
char *chan_join(bot_t *, char *);
char *chan_part(bot_t *, char *);
char *chan_mode(bot_t *, char *, char *);
char *chan_quit(bot_t *, char *);
char *chan_nick(bot_t *, char *);

enum {
	MOD_CHAN_NONE = 0,
	MOD_CHAN_LIST = 1,
	MOD_CHAN_USERS,
	MOD_CHAN_MODE,
	MOD_CHAN_JOIN,
	MOD_CHAN_PART,
	MOD_CHAN_TOPIC,
	MOD_CHAN_QUIT,
	MOD_CHAN_NICK,
};

/* ^chan(match) #jumping #bosshogs poopface <-- will only work for #jumping, #bosshogs, or poopface pm */
char *mod_chan_chan_match;

void __chan_init__(void) __attribute__ ((constructor));

#endif
