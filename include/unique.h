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
#ifndef UNIQUE_H
#define UNIQUE_H

#include "bot.h"

enum unique_id_flags {
UNIQUE_ID_NICK= 0x01,
UNIQUE_ID_CHAN=0x02,
UNIQUE_ID_HOST=0x04,
UNIQUE_ID_TAG=0x08,
UNIQUE_ID_ID=0x10,
};

typedef struct unique {

int id_flags;

dlist_t * dptr_self;

char * nick;
char * chan;
char * host;
char * tag;
int ID;

int data_type;
void * data;

} unique_t;

enum unique_bu2str_flags {
UNIQUE_BU2STR_NEWLINE = 0x01,
};


unique_t * unique_create(bot_t *, dlist_t **, int) ;

unique_t * unique_find(bot_t *, dlist_t ** );

void * unique_get(bot_t *, dlist_t **);

unique_t * unique_delete(bot_t *, dlist_t **, void (*)(void *) );

void unique_free(void * ) ;

void unique_destroy(bot_t *, dlist_t **, void (*)(void *)) ;

char * unique_bu2str(unique_t *,int);


#endif
