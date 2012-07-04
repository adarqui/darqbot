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
#ifndef CONSOLE_H
#define CONSOLE_H

#include "bot.h"

void bot_evhook_console(int, short, void *);

void console_info_print(bot_t *);

/* xmodule */
void console_xmodreload(int type, char *);

void console_xmodunload(int type, char *);

void console_xmodload(int type, char *);


/* module */
void console_modreload(char *);

void console_modunload(char *);

void console_modload(char *);


/* pmodule */
void console_pmodreload(char *);

void console_pmodunload(char *);

void console_pmodload(char *);

/* gmodule */
void console_gmodreload(char *);

void console_gmodunload(char *);

void console_gmodload(char *);


/* console stuff */
void bot_evhook_console (int, short, void *);

int bot_help_console (bot_t *);

void console_info(bot_t *, char *);

void console_info_print(bot_t *);

void console_tag_parse(bot_t *, char *);

void console_parse(bot_t *, char *);

void console_print_all(int );

void console_print_all_bot(bot_t *, int );



#endif

