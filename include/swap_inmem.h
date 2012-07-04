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
#ifndef SWAP_INMEM_H
#define SWAP_INMEM_H

#include "bot.h"

typedef struct swap_inmem_t {
char * name;
int id;
void * data;
} swap_inmem_t;

dlist_t * dl_swapim;

swap_inmem_t * swap_inmem_create(char *name, int id, void * data_ptr);
void swap_inmem_add(swap_inmem_t * swapim) ;
swap_inmem_t * swap_inmem_create_and_add(char * name, int id, void * data_ptr) ;
dlist_t * swap_inmem_get(char * name, int id) ;

void swap_inmem_free(void *);
void swap_inmem_remove(dlist_t **);
void swap_inmem_fini(void);

void swap_inmem_get_assign_and_remove(char *, int, void **);

void swap_inmem_console_info(bot_t *) ;

#endif
