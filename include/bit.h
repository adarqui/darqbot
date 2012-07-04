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
#ifndef BOT_BIT_H
#define BOT_BIT_H

#include "bot.h"

typedef struct bit_vector {
unsigned char * vec;
unsigned int sz;
} bit_vector_t;

int bit_extract_bit_char(unsigned char, int);

int bit_pack_bit_char(unsigned char *, int, int);

int bit_pack_bit_char_set(unsigned char *, int) ;

int bit_pack_bit_char_clr(unsigned char *, int);

bit_vector_t * bit_vec_init(int);

void bit_vec_fini(bit_vector_t **);

int bit_vec_get_and_set(bit_vector_t *);

void bit_vec_clr(bit_vector_t *, int);

void bit_vec_zero(bit_vector_t *);

void bit_vec_set(bit_vector_t *, int);

int bit_vec_get(bit_vector_t *, int);

int bit_vec_scan(bit_vector_t *, int);

char * bit_vec_to_str(bit_vector_t *);

void bit_vec_from_str(bit_vector_t *, char *) ;

void bit_vec_from_num(bit_vector_t *, unsigned long);

unsigned long bit_vec_base_next(unsigned long);


void bit_vec_and(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_or(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_xor(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_not(bit_vector_t *);

void bit_vec_add(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_sub(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_mul(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_div(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_shr(bit_vector_t *, bit_vector_t *, bit_vector_t *);

void bit_vec_shl(bit_vector_t *, bit_vector_t *, bit_vector_t *);

int bit_vec_3safe(bit_vector_t *, bit_vector_t *, bit_vector_t *) ;

#endif
