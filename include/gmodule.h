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
#ifndef BOT_GMODULE_H
#define BOT_GMODULE_H


#include "bot.h"
#include "fd.h"


typedef struct bot_gmod_list_element {
int index;

int has_run;
int can_pass_up;
int can_pass_down;

int opt_keepalive;
int opt_opposite;

bot_t * bot;
char * syntax;
char trigger[16];
char trigger_ext[16];

fd_link_t link;

int data_type;
void * data;

dlist_t * dl_cache;

} bot_gmod_elm_t;

typedef struct bot_gmod_cache_element {
int data_type;
char * data;
int data_len;
} bot_gmod_cache_elm_t;


module_t *gmodule_load (char *);

module_t *gmodule_unload(module_t *);

void gmodule_iolist(void);

void gmodule_timerlist(void);

void gmodule_list(void);

char *gmodule_path (char *);

void gmodule_load_all (void);

void bot_gxmodule_free(void *);

bot_t * gmodule_unload_all(void);

void bot_gmodule_run_stack(bot_t *);

void bot_gmodule_run_stack_new(bot_t *);

void bot_gmodule_run_stack_once(bot_t *);

void bot_gmodule_run_stack_new_once(bot_t *);

void bot_gmodule_elm_free(void *);

void bot_gmodule_run_control(bot_t * , int , ...) ;


int gmodule_up(dlist_t *, bot_t *);

int gmodule_down(dlist_t *, bot_t *);

int gmodule_destroy_up(dlist_t *, bot_t *);

int gmodule_destroy_down(dlist_t *, bot_t *);

int gmodule_control_up(dlist_t *, bot_t *);

int gmodule_control_down(dlist_t *, bot_t *);

int gmodule_up_then_down(dlist_t *, bot_t *);

void gmodule_fix_data_up(bot_t *);

void gmodule_fix_data_down(bot_t *);

void gmodule_fix_data_out_to_in(bot_t *);

void gmodule_fix_data_in_to_out(bot_t *);

bot_gmod_elm_t * gmodule_get_gmod(dlist_t *);

bot_gmod_elm_t * gmodule_get_gmod_next(dlist_t *);

void gmodule_update(module_t *, char *) ;

dlist_t * gmodule_find_by_trig_dptr(char *);

module_t * gmodule_find_by_trig(char *);

module_t * gmodule_find_by_name(char *);

void gmodule_lock(char *);

void gmodule_unlock(char *);

bot_gmod_elm_t * gmodule_find_gmod(bot_t *, char *, char *);

dlist_t * gmodule_find_gmod_dptr(bot_t *, char *, char *);

bot_gmod_elm_t * gmodule_find_latest_gmod(bot_t *, char *, char *);

dlist_t * gmodule_find_latest_gmod_dptr(bot_t *, char *, char *);

void gmodule_gmod_print(bot_gmod_elm_t *);

void gmodule_update_stats(void);

void gmodule_sort(void);

dlist_t * bot_gmodule_dup_bot_gmodules(bot_t *) ;

void gmodule_off(bot_t *);

void gmodule_off_bot(bot_t *);

void gmodule_set_can_pass_up(bot_gmod_elm_t *);

void gmodule_set_can_pass_down(bot_gmod_elm_t *);

void gmodule_set_has_run(bot_gmod_elm_t *);

dlist_t * gmod_cache_enqueue(dlist_t **, int, char * , int);

bot_gmod_cache_elm_t * gmod_cache_dequeue(dlist_t **);

void gmod_cache_free(void *);

enum  gmod_keepalive_types
{
GMOD_KEEPALIVE_OFF=0,
GMOD_KEEPALIVE_ON = 1,
};

void gmodule_keepalive_on(bot_t *);

void gmodule_keepalive_off(bot_t *);

void gmodule_parse_common_options(bot_t *, bot_gmod_elm_t *, char * ) ;

int gmodule_up_force(dlist_t *, bot_t *);

int gmodule_down_force(dlist_t *, bot_t *);

void gxmodule_free(void *) ;


int gmodule_control_up_fd(dlist_t * , bot_t * , int , char * , char * ) ;

#endif
