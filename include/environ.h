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
#ifndef ENVIRON_H
#define ENVIRON_H

#define ENVIRON_MAX_ARGV 9

bot_t * environ_set(bot_t *, char *, char *);

bot_t * environ_set_withnum(bot_t *, int, char *);

char * environ_get(bot_t *, char *);

bot_t * environ_unset(bot_t *, char *);

bot_t * environ_destroy(bot_t *);

bot_t * environ_destroy_args(bot_t *);

bot_t * environ_add_args1(bot_t *, char *, char *);

bot_t * environ_debugprint(bot_t *);

char  * environ_sub_envs(bot_t *, char *);

#endif
