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
#ifndef PMODULE_H
#define PMODULE_H

#include "bot.h"


module_t *pmodule_load (char *);

module_t *pmodule_unload(module_t *);

void pmodule_iolist(void);

void pmodule_timerlist(void);

void pmodule_list(void);


char *pmodule_path (char *);

void pmodule_load_all (void);

void bot_pxmodule_free(void *);

bot_t * pmodule_unload_all(void);

void pmodule_update(module_t *, char *) ;

dlist_t * pmodule_find_by_trig_dptr(char *);

module_t * pmodule_find_by_trig(char *);

module_t * pmodule_find_by_name(char *);

void pmodule_cur_set(bot_t *(*fn)(dlist_t *, bot_t *), dlist_t * , bot_t *);

void pmodule_cur_clear(void);

bot_t * pmodule_cur_run(void);

bot_t * pmodule_cur_run2(bot_t *);

void pmodule_lock(char *);

void pmodule_unlock(char *);

void pmodule_update_stats(void);

void pmodule_sort(void);


#endif
