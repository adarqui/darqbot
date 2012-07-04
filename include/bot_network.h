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
#ifndef BOT_NETWORK_H
#define BOT_NETWORK_H


#include "bot.h"

char *n_gethostbyname (char *);

int bot_network_raw_connect (char *, int, int, int);

int bot_network_raw_listen (char *, int);

int bot_network_raw_disconnect (int );

int bot_unset_evhooks (bot_t *);

int bot_set_evhooks(bot_t *, void (*fn)(int, short, void *));

#endif
