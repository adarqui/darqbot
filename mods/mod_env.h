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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef MOD_ENV_H
#define MOD_ENV_H

#include "bot.h"

/*
#include "helvetica.inc"
#define font helv_font
*/

enum mod_env_stuff {
	MOD_ENV_SETENV = 1,
	MOD_ENV_UNSETENV,
	MOD_ENV_GETENV,
	MOD_ENV_CLEAR,
	MOD_ENV_SIZE,
	MOD_ENV_LIST,
};

dlist_t *dl_mod_env_unique;

module_t mod_env_info;

typedef struct mod_env_element {
	char name[32];
	char value[512];
} env_t;

bot_t *env_init(dlist_t *, bot_t *);
bot_t *env_fini(dlist_t *, bot_t *);
bot_t *env_help(dlist_t *, bot_t *);
bot_t *env_run(dlist_t *, bot_t *);

char *env_change_string(bot_t *, char *, int);
void __env_init__(void) __attribute__ ((constructor));

char *env_op_unsetenv(struct avl_table **, char *);
char *env_op_setenv(struct avl_table **, char *);
char *env_op_getenv(struct avl_table **, char *);
char *env_op_clear(struct avl_table **, char *);
char *env_op_size(struct avl_table **, char *);
char *env_op_list(struct avl_table **, char *);

void env_list_whole_tree(dlist_t **, const struct avl_table *);
static void env_list_tree_structure(dlist_t **, const struct avl_node *, int);
int env_compare(const void *, const void *, void *);
void env_freex(void *, void *);

#endif
