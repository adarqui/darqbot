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
#ifndef CONTROL_H
#define CONTROL_H

enum control_sub_types {
CONTROL_SUB_FDPASS=1,
};

typedef struct bot_control {
dlist_t * dl_subs;
} control_t;

typedef struct bot_subcontrol{ 
int type;
int val;
void * val_data; 
} control_sub_t;

control_t * control_init(void);

control_sub_t *control_sub_init(void);

dlist_t * control_add_fdpass(control_t *, int);

int control_get_fdpass(dlist_t **);

int control_get_fdpass_find(dlist_t **);

dlist_t * control_bot_add(bot_t *, control_t *);

void control_bot_del(bot_t *, dlist_t *);

void control_fini(void *);

void control_sub_fini(void *);

#endif
