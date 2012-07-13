#ifndef FDPASS_H
#define FDPASS_H

#include "bot.h"

enum fdpass_control_types {
FDPASS_CONTROL_TYPE_REQUEST=1,
FDPASS_CONTROL_TYPE_RESPONSE,
FDPASS_CONTROL_TYPE_NOP
};

enum fdpass_control_subtypes {
FDPASS_CONTROL_SUBTYPE_PUT=1,
FDPASS_CONTROL_SUBTYPE_GET,
FDPASS_CONTROL_SUBTYPE_LIST,
FDPASS_CONTROL_SUBTYPE_NOP,
};

enum fdpass_control_subtypes_put {
FDPASS_CONTROL_PUT_FD=1,
FDPASS_CONTROL_PUT_NOP
};

enum fdpass_control_get_and_put_ops { 
FDPASS_CONTROL_GET_TCP = 1,
FDPASS_CONTROL_GET_UDP,
FDPASS_CONTROL_GET_RAW,
FDPASS_CONTROL_GET_NOP,
};


typedef struct fdpass_control_op_t {
unsigned char type;
unsigned char subtype;
unsigned char subtype_op;
char tag[16];
char tag_ext[16];
} fdpass_control_op_t;

typedef struct fdpass_control {
struct msghdr msg;
struct cmsghdr * cmsg;
char cmsg_buf[sizeof(struct cmsghdr) + sizeof(long)];
fdpass_control_op_t op;
struct iovec iov[1];
} fdpass_control_t;

fdpass_control_t * fdpass_init(char * , char *);
void fdpass_fini(fdpass_control_t **);

int fdpass_req_get(fdpass_control_t *fc, int ) ;
int fdpass_req_get_raw(fdpass_control_t *); 

int fdpass_resp_get(fdpass_control_t *, int , int ) ;
int fdpass_resp_get_raw(fdpass_control_t *, int );

int fdpass_resp_nop(fdpass_control_t *);

void fdpass_print(struct msghdr * );

/*
int fdpass_req_put(fdpass_control_t *, int);
int fdpass_req_put_fd(fdpass_control_t *);
int fdpass_req_put_nop(fdpass_control_t *);

int fdpass_req_get(fdpass_control_t *, int);
int fdpass_req_get_tcp(fdpass_control_t *);
int fdpass_req_get_udp(fdpass_control_t *);
int fdpass_req_get_raw(fdpass_control_t *);
*/

#endif
