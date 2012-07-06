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
#ifndef XMODULE_H
#define XMODULE_H

#include "bot.h"

#define MODULE_SUBTRIG_MAX 15

enum xmodule_styles { 
XMODULE_STYLE_LIST = 0,
XMODULE_STYLE_TREE,
XMODULE_STYLE_HASH,
XMODULE_STYLE_ARRAY,
};

enum xmodule_control_types {
XMODULE_CONTROL_FDPASS=1,
}
;

typedef struct module
{
unsigned char active;
unsigned char type;
unsigned char locked;

char name[32];
char filename[132];
char trigger[16];
char trigger_root[16];
char trigger_ext[16];

/*
char subtrigs[MODULE_SUBTRIG_MAX+1][16];
*/
dlist_t * dl_subtrigs;
/* temporary ptr used by gmodule_xxx_up/down */

  bot_t *(*init) (dlist_t *, bot_t *);
  bot_t *(*fini) (dlist_t *, bot_t *);
  bot_t *(*help) (dlist_t *, bot_t *);
  bot_t *(*run) (dlist_t *, bot_t *);
  bot_t *(*control)(dlist_t *, bot_t *, int, va_list);
  bot_t *(*stat)(dlist_t *, bot_t *);
  bot_t *(*timer)(dlist_t *, bot_t *);

  bot_t *(*on)(dlist_t *, bot_t *);
  bot_t *(*off)(dlist_t *, bot_t *);

bot_t *(*destroy_up)(dlist_t *, bot_t *);
bot_t *(*destroy_down)(dlist_t *, bot_t *);

bot_t *(*control_up)(dlist_t *, bot_t *);
bot_t *(*control_down)(dlist_t *, bot_t *);

bot_t *(*output)(dlist_t *, bot_t *);
bot_t *(*input)(dlist_t *, bot_t *);

struct module * self;
dlist_t * dptr_self;

void * dl_handle;

int tmp_stat_count;

int max_nesting;

dlist_t **dl_pointer;

} module_t;


enum xmodule_types {
XMODULE_TYPE_MODULE=0x01,
XMODULE_TYPE_PMODULE=0x02,
XMODULE_TYPE_GMODULE=0x04,
XMODULE_TYPE_MIRROR=0x08,
};

/* xmodule stuff */
module_t * xmodule_load(int, char *);
module_t * xmodule_load_reload(int, module_t *);
module_t * xmodule_load_new(int, char *);
module_t * xmodule_load_dlso(module_t *);
module_t *xmodule_load_subtrigs(module_t * ) ;
void xmodule_del_subtrigs(module_t *);

void xmodule_free(void *);
void xmodule_free_destroy(void *);
void xmodule_free_destroy_dptr(void *);


module_t * xmodule_unload(int, module_t *,char *);
void xmodule_iolist(int);
void * xmodule_iolist_traverse(void *, void *);
void xmodule_timerlist(int);
void * xmodule_timerlist_traverse(void *, void *);

void xmodule_list(int);
void xmodule_load_all(int);
bot_t * xmodule_unload_all(int);
char * xmodule_path(int, char *);
module_t * xmodule_find_by_trig(int, char *);
dlist_t * xmodule_find_by_trig_dptr(int, char *);
module_t * xmodule_find_by_name(int, char *);
void xmodule_update(int, module_t *, char *) ;
void xmodule_lock(int, char *);
void xmodule_unlock(int ,char *);
void xmodule_unload_everything(void);
void xmodule_destroy_everything(void);
void xmodule_update_stats(int);
void xmodule_sort(int );
void * xmodule_sort_traverse(void * , void * );
void xmodule_parse_style(char *);

/* AVL stuff */
int xmodule_avl_compare(const void * , const void * , void *) ;
void xmodule_avl_free(void *, void * ) ;
void xmodule_avl_free_destroy(void *, void *);
void * xmodule_tree_match_by_name(void * , void * ) ;
void * xmodule_tree_print_node(void * ) ;
void xmodule_del_timer(module_t *, char *);
dlist_t * xmodule_find_timer(module_t *, char * );
void xmodule_add_timer(module_t *) ;
dlist_t * xmodule_find_iohook(module_t *, char * ) ;
void xmodule_add_iohook(module_t *) ;
void xmodule_del_iohook(module_t *, char *);
module_t * xmodule_dup_mirror(module_t *, char *, char *, char *);
void xmodule_add_subtrigs(module_t * ) ;
void * xmodule_list_traverse(void * , void * ) ;
dlist_t ** xmodule_type_to_dl(int);
dlist_t ** xmodule_typeio_to_dl(int);
dlist_t ** xmodule_typetimer_to_dl(int);
void * xmodule_unload_traverse(void  *, void *) ;
void * xmodule_unload_all_traverse(void *, void *);
dlist_t * xmodule_insert(dlist_t **, module_t *) ;
void xmodule_zero(module_t *);
void xmodule_transfer(module_t *,module_t *);


void xmodule_free(void *);

#endif
