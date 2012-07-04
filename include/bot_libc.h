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
#ifndef BOT_LIBC_H
#define BOT_LIBC_H

#include "bot.h"

/* raw libc */



     size_t
     strlcpy(char *, const char *, size_t );


     size_t
     strlcat(char *, const char *, size_t );


#ifndef _GNU_SOURCE
#ifndef useconds_t
typedef int useconds_t;
#endif
#endif

int usleep(useconds_t);

#ifndef _GNU_SOURCE
char * strdup(const char *);
#endif

int strcasecmp(const char *, const char *);

int strncasecmp(const char *, const char *, size_t);

int kill(pid_t, int);

int fileno(FILE *);

#ifndef _GNU_SOURCE
int isascii(int);
#endif

int setgroups(size_t, const gid_t *);



char * strcasestr(const char *, const char *);

int getopt(int , char * const *, const char *);


FILE *fmemopen(void *, size_t , const char *);

#endif
