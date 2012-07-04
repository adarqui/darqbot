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
#ifndef CONF_H
#define CONF_H

#include "bot.h"

/* decls */

char *bot_find_confdir (bot_t *);

int conf_parse (bot_t *);

int bot_parse_logfile (bot_t *, char *, int);

int bot_parse_trace(bot_t *, char *, int);

int bot_parse_debug(bot_t *, char *, int);

int bot_parse_pmodules(bot_t *, char *, int);

int bot_parse_gmodules(bot_t *, char *, int, char *);

void conf_fix(bot_t *);

char * conf_get_proto(int);

char * conf_get_type(int);

#endif
