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
#ifndef XDB_H
#define XDB_H

typedef struct xdb_pair {
char * key;
char * value;
} xdb_pair_t;

DB * xdb_open(char *);

int xdb_write(DB *, char *, char *);

void xdb_fini(DB *);

xdb_pair_t * xdb_get(DB *, char *);

xdb_pair_t * xdb_get_recnum(DB * , int );

int xdb_count(DB *) ;

xdb_pair_t * xdb_pair_create(char * , int, char * , int) ;

void xdb_pair_destroy(void *);


xdb_pair_t * xdb_pair_find_by_key(dlist_t * dl, char * key) ;

#endif