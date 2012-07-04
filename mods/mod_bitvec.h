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
#ifndef MOD_BITVEC_H
#define MOD_BITVEC_H

#include "bot.h"

module_t mod_bitvec_info;

bot_t *bitvec_init (dlist_t *, bot_t *);
bot_t *bitvec_fini (dlist_t *, bot_t *);
bot_t *bitvec_help (dlist_t *, bot_t *);
bot_t *bitvec_run (dlist_t *, bot_t *);


char *bitvec_change_string (bot_t *, char *, int);


void __bitvec_init__ (void) __attribute__ ((constructor));


enum mod_bitvec_types
{
  BITVEC_OPT_INIT = 1,
  BITVEC_OPT_FINI,
  BITVEC_OPT_SET,
  BITVEC_OPT_CLR,
  BITVEC_OPT_OR,
  BITVEC_OPT_XOR,
  BITVEC_OPT_AND,
  BITVEC_OPT_NOT,
  BITVEC_OPT_EQ,
  BITVEC_OPT_ZERO,
  BITVEC_OPT_COPY,
  BITVEC_OPT_INFO,
}
 ;

dlist_t *mod_bv_unique;

typedef struct mod_bv_data
{
  bit_vector_t *mod_bitvec_bv_1;
  bit_vector_t *mod_bitvec_bv_2;
  bit_vector_t *mod_bitvec_bv_res;
} mod_bv_data_t;

char *bitvec_op_init (mod_bv_data_t *, char *);
char *bitvec_op_fini (mod_bv_data_t *);
char *bitvec_op_info (mod_bv_data_t *);
char *bitvec_op_set (mod_bv_data_t *, char *);
char *bitvec_op_clr (mod_bv_data_t *, char *);
char *bitvec_op_eq (mod_bv_data_t *, char *);
char *bitvec_op_and (mod_bv_data_t *, char *);
char *bitvec_op_or (mod_bv_data_t *, char *);
char *bitvec_op_xor (mod_bv_data_t *, char *);
char *bitvec_op_not (mod_bv_data_t *, char *);
char *bitvec_op_zero (mod_bv_data_t *, char *);
char *bitvec_op_copy (mod_bv_data_t *, char *);

bit_vector_t *bitvec_which (mod_bv_data_t *, char *);


#endif
