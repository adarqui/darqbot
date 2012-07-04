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
#ifndef MY_UNIX_H
#define MY_UNIX_H

#include "bot.h"

int sig_stoi(char *);

char * sig_itos(int);

#define _PATH_DEVNULL "/dev/null"

int close_io_descriptors(char *);

int xdaemon(int, int) ;

void xsetproctitle(char *);

char * random_data(int );

void
clean_environ (void);

#endif
