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
#ifndef BOT_H
#define BOT_H

#define DARQBOT_VERSION "0.1 (July 4 2012"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <stdarg.h>
#include <ctype.h>

#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event.h>
#include <dlfcn.h>

#include <wchar.h>
#include <locale.h>

#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/un.h>

#include <sys/uio.h>
#include <dirent.h>

#include <execinfo.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <setjmp.h>
#include <math.h>



//#define __USE_BSD
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/tcp.h>
#undef __USE_BSD
#define __USE_BSD
#ifndef u_short
#define u_short unsigned short
#endif
#include <netinet/ip.h>
#undef __USE_BSD
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>


#include <matheval.h>

typedef unsigned int u_int;
typedef unsigned long u_long;
#include <db.h>


#include "dlist.h"
#include "mongo.h"
#include "avl.h"


#define MAX_BUF_SZ 100024


enum bot_type {
BOT_TYPE_DAEMON=1,
BOT_TYPE_CLIENT=2,
BOT_TYPE_SERVER=3,
}

;

enum bot_node_type {
BOT_NODE_TYPE_ROOT=1,
BOT_NODE_TYPE_SUB=2,
};


/* structs */
typedef struct bot
{

unsigned int ID;

struct bot * parent;
dlist_t * dptr_self;
dlist_t * dl_subs;
unsigned char node_type;

unsigned char on;
char tag[32];
char master[32];

unsigned char mute;
unsigned char type;

/* tracing/debugging stuff */
unsigned char debug;
unsigned char trace;
unsigned char brake;

/* a protective mechanism in the case of some kind of looping pipe/read error */
int error_count;

/* conf stuff */
/* POINTERS- NOT DUPS */
  char confdir[132];
char conffile[32];
  char logfile[132];
  time_t uptime;

/* module stuff */
char * dl_module_arg;
char * dl_module_help;

dlist_t * dl_environ;

/* trigger stuff */
char trig_called[1024];

/* request/response stuff */
char irc_command[32];

unsigned char isforked_stop;
unsigned char isforked;
unsigned char isline;
unsigned char isprivmsg;
unsigned char ispm;
unsigned char iscomment;

/* vars: when adding vars, set them in bot_defaults (bot.c) and in the bot_parse_conf func (conf.c)
 * multi = allow multi line printing if set
 * usleep = usleep val between multi line sends 
 * maxbufsz = maximum buf size acceptable, for example, 512 for irc (set from conf/defaults
 * bufsz = can change anywhere from 0 to maxbufsz
 * maxflood = maximum number of lines to send when multi line sending 
 * maxnesting = maximum nesting depth, sets from conf/defaults
 * nesting = settable nesting val, from 0 to maxnesting
 * nesting_cur <- cant set through var, this is the current nesting level
 */
unsigned char var_multi;
int var_usleep;
int var_maxbufsz;
int var_bufsz;
int var_maxflood;
int var_maxnesting;
int var_nesting;
int var_nesting_cur;
unsigned char var_allowpm;

/* server message, if server message, isprivmsg=0, isline=0 */
char txt_server[32];
int txt_server_msg;
/* the "rest" will be in txt_data_out */

/* line message */
char txt_nick[32];
char txt_ident[32];
char txt_host[132];
char txt_to[132];
char txt_data_in[MAX_BUF_SZ+1];
int txt_data_in_sz;
char txt_data_out[MAX_BUF_SZ+1];
int txt_data_out_sz;
char shouldsend;

/* console printing stuff */
  int console_opts;

/* list of strings, which contain pmodule trigger formats, ie ^parse1(options) */
dlist_t * dl_pmodules;
dlist_t * dl_pmodules_cur;

/* contains a stack of 'trigger lines'.. a stack because parse2 etc can be nested */
dlist_t * stack_cmds;

dlist_t * dl_gmodules;
dlist_t * dl_gmodules_cur;
int dl_gmodules_keepalive;
int dl_gmodules_force;

/* control data */
dlist_t * dl_control;

/* trig stuff */
char trig_prefix[16];

} bot_t;


#include "conf.h"
#include "misc.h"
#include "protocol.h"
#include "xmodule.h"
#include "gmodule.h"
#include "pmodule.h"
#include "zmodule.h"
#include "module.h"
#include "bot_network.h"
#include "bot_text.h"
#include "mongodb.h"
#include "stat.h"
#include "timer.h"
#include "global.h"
#include "environ.h"
#include "tokenize.h"
#include "bot_unix.h"
#include "safe.h"
#include "debug.h"
#include "console.h"
#include "fns.h"
#include "bot_libc.h"
#include "files.h"
#include "xdb.h"
#include "timer.h"
#include "bit.h"
#include "relink.h"
#include "swap_inmem.h"
#include "fd.h"
#include "xpid.h"
#include "control.h"
#include "unique.h"
#include "chunks.h"
#include "swap_disk.h"
#include "sim.h"

#include "randchar.h"
#include "matchchar.h"

#include "my_unix.h"
#include "fn_all.h"
#include "fdpass.h"


enum console_opts
{
  BOT_CONSOLE_OPT_LINK = 0x01,
  BOT_CONSOLE_OPT_A = 0x02,
  BOT_CONSOLE_OPT_B = 0x04,
  BOT_CONSOLE_OPT_C = 0x08,
  BOT_CONSOLE_OPT_D = 0x10,
  BOT_CONSOLE_OPT_E = 0x20,
  BOT_CONSOLE_OPT_F = 0x40,
  BOT_CONSOLE_OPT_G = 0x80
};


/* defs/globals */
extern int errno;
extern char **environ;

extern int optind;
extern char * optarg;

unsigned char * g_test_mem_1;
unsigned char * g_test_mem_2;
unsigned char * g_test_mem_3;
unsigned int g_test_fd_1;
unsigned int g_test_fd_2;
unsigned int g_test_fd_3;



enum fd_link_flags {
FD_LINK_NORMAL=0,
FD_LINK_LIST,
};


/* ~/.darqbot/conf */
#define BOT_DEF_CONFDIR ".darqbot"
#define BOT_DEF_CONFFILE "conf"


/* decls */
bot_t *bot_init (void);

int bot_line_clear(bot_t *);

int bot_line_clear_bufs(bot_t *);

void bot_fini(void *);

void bot_fini_subs(void *);

void bot_sigend_handler(int);

void bot_atexit_handler(void);

void bot_sigpipe_handler(int);

void bot_sigchld_handler(int);

void bot_sigalrm_handler(int);

void bot_exit_handler(int);

void bot_fini_wrapper(void);

void bot_defaults(bot_t *);

void bot_test_stuff_init(void);


int change_server(bot_t *);

/*
#ifndef useconds_t
typedef int useconds_t;
#endif
*/

int bot_shouldreturn(bot_t *);

void bot_find_and_set_fd(char *, int);

int bot_parse_tag(bot_t *, char *, int);

bot_t * bot_init_and_turn_on(char * );

void bot_turn_on(bot_t *, int);

void bot_turn_off(bot_t *);

void bot_exchange_data(bot_t *, bot_t *);

dlist_t * bot_sub_add(bot_t *, bot_t *);

bot_t  * bot_dup_sub(bot_t * );

bot_t * bot_dup(bot_t *);

bot_t * bot_find_tag(char *);

bot_t * bot_find_sub_by_tag(bot_t * , char *) ;

bot_t * bot_find_sub_by_id(bot_t * , int) ;

pid_t bot_fork_clean(bot_t *);

void bot_fork_clean_exit(bot_t *);

void bot_copy_values(bot_t * bot, char * nick, char * ident, char * host, char * to, char * in, char * out);

dlist_t ** bot_find_all_dlist_by_bot_id(bot_t *, dlist_t ** ) ;

bot_t * bot_find_all_by_id(int);

enum bot_daemon_flags {
BOT_DAEMON_NOCHDIR = 0x01,
BOT_DAEMON_NOCLOSE = 0x02,
BOT_DAEMON_REMOVE_MODULES = 0x04,
BOT_DAEMON_REMOVE_BOTS = 0x08,
BOT_DAEMON_EVENT_RESTART = 0x10,
BOT_DAEMON_RENAME_PROC=0x20,
BOT_DAEMON_DESTROY_ALLOC=0x40,
BOT_DAEMON_UNIX_RESTART=0x80,
BOT_DAEMON_CLEAN_FDS=0x100,
};

int bot_daemon(int, char **, bot_t **, char *, char *);

/* global stuff */


#endif
