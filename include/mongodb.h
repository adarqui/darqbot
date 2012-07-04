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
#ifndef MONGODB
#define MONGODB

#include "bot.h"

typedef struct keyvalue {
char * key;
char * value;
} keyvalue_t;

enum argument_opts {
MONGODB_OPT_ARGS=0x01,
MONGODB_OPT_STRINGS=0x02,
MONGODB_OPT_ARRAY=0x04,
MONGODB_OPT_DLIST=0x08,
};

enum argument_types {
MONGODB_ARG_STRING=1,
MONGODB_ARG_INT,
MONGODB_ARG_DOUBLE,
MONGODB_ARG_LONG,
MONGODB_ARG_OID,
MONGODB_ARG_UNKNOWN,
} ;

typedef struct mongo_argument {
int type;
char * name;
char * arg_str;
int arg_int;
double arg_double;
long arg_long;
//unsigned char arg_oid[12];
bson_oid_t arg_oid;
} mongo_argument_t;


char mpbuf[MAX_BUF_SZ+1];


bot_t * mongodb_init(bot_t *);

bot_t * mongodb_fini(bot_t *);

int mongodb_insert_key(bot_t *, char *, char *, char *, char *, ...);

char * mongodb_retrieve_key(bot_t *, char *, char *, char *);

int mongodb_remove_key(bot_t *, char *, char *);

int mongodb_insert_key_stat(bot_t *, char *, char *, int );

int mongodb_retrieve_key_stat(bot_t *, char *, char * );

int mongodb_update_key_stat(bot_t *, char *, char *, int);

dlist_t * mongodb_dump_keyvalue(bot_t *, char * );

void mongodb_destroy_keyvalue(bot_t * ,dlist_t **);

void mongodb_free_keyvalue(void *);

mongo_argument_t ** mongodb_fmt_retrieve(bot_t *, char *, mongo_argument_t *);

int mongodb_arg_destroy(bot_t *,  mongo_argument_t **);

void mongodb_arg_free(void *);

void mongodb_arg_print(bot_t *, mongo_argument_t **);

void mongodb_arg_print_arg(mongo_argument_t *);


mongo_argument_t ** mongodb_fmt_build_strings(bot_t *, char *);

mongo_argument_t ** mongodb_fmt_build_fmt(bot_t *, char *, ...);

int mongodb_fmt_insert_arg(bot_t *, char * db, mongo_argument_t **);

/* use these */
bson * mongodb_bson_build(bot_t *, char *, ...);

int mongodb_bson_insert(bot_t *, char * , bson *);

void mongodb_bson_unfinish(bson *);

char * mongodb_bson_dump(bson *);

char  * mongodb_bson_print_init(const bson *);

void mongodb_bson_print(const bson *);

void mongodb_bson_print_raw(const char * , int);

bson * mongodb_bson_get(bot_t *, char *, char *, int, void *);

void mongodb_drop_db(char *);

#endif
