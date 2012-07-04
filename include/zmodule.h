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
#ifndef ZMODULE_H
#define ZMODULE_H

#include "bot.h"

/* zmodule */
void console_zmodreload(char *);

void console_zmodunload(char *);

void console_zmodload(char *);

void zmodule_iolist(void);

void zmodule_timerlist(void);

void zmodule_list(void);

void zmodule_lock(void);

void zmodule_unlock(void);

void zmodule_unload_all_except(char *, ...);

void zmodule_lock_arg(char *);

void zmodule_unlock_arg(char *);

void zmodule_prune(void);

#endif
