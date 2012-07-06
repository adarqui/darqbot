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
#ifndef MOD_CMPH_H
#define MOD_CMPH_H

#include "bot.h"
#include <cmph.h>

enum mod_cmph_stuff
{
  MOD_CMPH_BUILD = 1,
  MOD_CMPH_CLEAR,
  MOD_CMPH_FIND,
  MOD_CMPH_SIZE,
  MOD_CMPH_LIST,
};

dlist_t *dl_mod_cmph_unique;

module_t mod_cmph_info;

bot_t *cmph_init (dlist_t *, bot_t *);
bot_t *cmph_fini (dlist_t *, bot_t *);
bot_t *cmph_help (dlist_t *, bot_t *);
bot_t *cmph_run (dlist_t *, bot_t *);

char *cmph_change_string (bot_t *, char *, int);

void __cmph_init__ (void) __attribute__ ((constructor));


typedef struct __brz_data {
        CMPH_ALGO algo;      // CMPH algo for generating the MPHFs for the buckets (Just CMPH_FCH and CMPH_BMZ8)
        cmph_uint32 m;       // edges (words) count
        double c;      // constant c
        cmph_uint8  *size;   // size[i] stores the number of edges represented by g[i][...].
        cmph_uint32 *offset; // offset[i] stores the sum: size[0] + size[1] + ... size[i-1].
        cmph_uint8 **g;      // g function.
        cmph_uint32 k;       // number of components
/*
        hash_state_t **h1;
        hash_state_t **h2;
        hash_state_t * h0;
*/
} __brz_data_t;


typedef struct cmph_wrapper {
cmph_t * hash;
cmph_io_adapter_t *source;
cmph_config_t * config;
FILE * fp;
} cmphx_t;

char * cmph_op_build(bot_t *, cmphx_t **, char *);
char * cmph_op_clear(bot_t *, cmphx_t **, char *);
char * cmph_op_find(bot_t *, cmphx_t **, char *);
char * cmph_op_size(bot_t *, cmphx_t **, char *);
char * cmph_op_list(bot_t *, cmphx_t **, char *);

#endif
