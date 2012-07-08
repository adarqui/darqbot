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
#ifndef GLOBAL_H
#define GLOBAL_H



typedef struct global_info {

void (*gcmd)(void*);

char ** argv;
int *argc;
char **env;

/* pid of the main proc, parent */
pid_t pid;
pid_t pid_child;

int on;

int atexit_code;

char conf_dir[132];
char conf_file[32];

char confdir[132];

dlist_t * bots;
bot_t * bot_current;

int chroot;

uid_t chroot_uid;
gid_t chroot_gid;
char chroot_dir[132];

struct event_base * ev_base;
struct event ev_console;
int fd_console;

char fd_unix_path[132];
struct sockaddr_un fd_unix_un;
struct event ev_unix;
int fd_unix;


int var_xmod_style;
  dlist_t *dl_module;
dlist_t * dl_pmodule;
dlist_t * dl_gmodule;

char * dl_module_str;
char * dl_pmodule_str;
char * dl_gmodule_str;

dlist_t * dl_mod_timers;
dlist_t * dl_gmod_gtimers;
dlist_t * dl_mod_iohooks;
dlist_t * dl_gmod_giohooks;

/* mongo db stuff */
mongo mongo_conn;
int mongo_isconnected;

dlist_t * dl_print;

/* global vars */
int var_timer_sleep;
int var_timer_sort;
int var_sigprotect;

jmp_buf sigprotect_buf;
//sigjmp_buf sigprotect_sigbuf;
jmp_buf sigprotect_sigbuf;

/* keep track of the last parser ran, for alias/guile etc */
bot_t * (*pmod_cur)(dlist_t *, bot_t *);
dlist_t * pmod_cur_dlist_node;
bot_t * pmod_cur_bot;

/* timer stuff */
timer_info_t timer;

/* pids */
dlist_t * dl_pid;

} global_info_t;

global_info_t global_info;
global_info_t * gi;

void global_defaults(void);

void global_conf_parse(void);

char * global_conf_dir(void);

int global_chroot(void);


int chroot(const char *);

int setreuid(uid_t, uid_t);

int setregid(gid_t, gid_t);

void global_core_unlimited(void);

void global_signal_hooks(void);

void global_signal_exit_hooks(void);

void global_on(void);

void global_off(void);

void global_fork_clean_stuff(void);

int global_parse_chroot(char *, int);

int global_parse_modules(char * , int);

int global_parse_pmodules(char * , int);

int global_parse_gmodules(char *, int);

int global_parse_trace(char *, int);

int global_parse_debug(char *, int);

int
global_parse_sigprotect (char *, int );

int
global_parse_timer_sort (char *, int );

int
global_parse_timer_sleep (char *, int );

int
global_parse_fd_unix_path (char *, int );

void global_signal_hooks_child(void);

void global_set_proc_ptrs(int *, char **, char **) ;



void global_gcmd_add(void (*fn)(void *),void *);
void global_gcmd(void);


void global_getopt(int , char **);

void global_set_evhook_console(void);

#endif
