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
#ifndef TIMER_H
#define TIMER_H

#include "bot.h"

typedef struct timer_info {
volatile int toggle;
time_t last_timer;
time_t last_sort;
} timer_info_t;

void timer_handler(int);

void timer_handler_new(int);

void timer_set(void);

void timer_unset(void);

void timer_shouldwerun(void);

void timer_sort_modules(void);
void (*n_timer_sort_modules)(void);

#endif
