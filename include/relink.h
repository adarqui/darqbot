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
#ifndef RELINK_H
#define RELINK_H

#include "bot.h"

#define RELINK_MAGIC 0x41deadbe
#define RELINK_MAX_PACKET_SIZE 512

enum relink_control_acket_ops {
RELINK_CONTROL_OP_NOP=0,
RELINK_CONTROL_OP_RELINK,
RELINK_CONTROL_OP_DESTROY,
RELINK_CONTROL_OP_QUERY,
RELINK_CONTROL_OP_HELLO,
RELINK_CONTROL_OP_RESULT,
RELINK_CONTROL_OP_DATA,
RELINK_CONTROL_OP_RELOAD,
} ;

enum relink_control_packet_types {
RELINK_CONTROL_TYPE_NOP = 0,
RELINK_CONTROL_TYPE_NORMAL,
RELINK_CONTROL_TYPE_SSL,
};

enum relink_control_packet_subtypes {
RELINK_CONTROL_SUBTYPE_NOP = 0,
RELINK_CONTROL_SUBTYPE_IRC,
};

enum relink_control_packet_query {
RELINK_CONTROL_QUERY_NOP = 0 ,
RELINK_CONTROL_QUERY_LINKS  ,
RELINK_CONTROL_QUERY_SUBS, 
RELINK_CONTROL_QUERY_RELOAD, 
};

/*
 * old = using an existing connection
 * new = using a new connection
 * failed = connection failed
 */
enum relink_control_packet_results {
RELINK_CONTROL_RESULT_NOP=0,
RELINK_CONTROL_RESULT_CONNECTION_OLD,
RELINK_CONTROL_RESULT_CONNECTION_NEW,
RELINK_CONTROL_RESULT_CONNECTION_ERROR,
RELINK_CONTROL_RESULT_UNINITIALIZED,
RELINK_CONTROL_RESULT_STRING,
};

enum relink_control_packet_destroy_types {
RELINK_CONTROL_DESTROY_NOP=0,
RELINK_CONTROL_DESTROY_LINK,
RELINK_CONTROL_DESTROY_SUB,
RELINK_CONTROL_DESTROY_ALL,
};

typedef struct relink_control_packet {
unsigned int magic;
unsigned int id;
unsigned int op;
unsigned int type;
unsigned int subtype;
} relink_control_pkt_t;

typedef struct relink_control_packet_relink {
char host[255];
int port;
} relink_control_pkt_relink_t;

int relink_req_pack_buf(char *, int, int, int, int) ;

int relink_req_pack_buf_hello(char *, int) ;

int relink_req_pack_buf_relink(char *, int, char *, int, int, int);

int relink_req_pack_buf_relink_new(char *, int, char *);

int relink_req_pack_buf_query(char *, int, int);

int relink_req_pack_buf_query_links(char * , int ) ;

int relink_req_pack_buf_query_subs(char * , int ) ;

int relink_req_pack_buf_query_reload(char * , int ) ;

int relink_req_pack_buf_destroy_link(char * , int , int ) ;

int relink_req_pack_buf_destroy_sub(char * , int , int ) ;

int relink_req_pack_buf_destroy_all(char * , int ) ;

int relink_req_pack_buf_reload(char *);

int relink_res_pack_buf(char *, int, int, int);

int relink_res_pack_buf_new(char * , int ) ;

int relink_res_pack_buf_old(char * , int ) ;

int relink_res_pack_buf_error(char * , int ) ;

int relink_res_buf_result_string(char *, int, char *);

int
relink_find_buf_result_string (char *, int, char **, int *);

int relink_pack_buf_data(char *, int, char *, int);

int relink_find_buf_data(char * , int, char ** , int *  ) ;

void relink_packet_print(char *, relink_control_pkt_t *);

void relink_packet_ntoh(relink_control_pkt_t *);

void relink_packet_hton(relink_control_pkt_t *);

int relink_ispacket(relink_control_pkt_t *);

int relink_packet_len(char * ) ;

dlist_t * relink_buf_to_dlist(char *, int) ;

#endif
