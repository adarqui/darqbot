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
#ifndef GMOD_GRAW_H
#define GMOD_GRAW_H

#include "bot.h"

/*
enum gmod_graw_stuff
{
  GRAW_CLIENT = 1,
  GRAW_SERVER,
  GRAW_RUN
};
*/

typedef struct gmod_graw
{
  bot_t *bot;
  dlist_t *dptr_gmod;
  bot_gmod_elm_t *gmod;

  int initialized;
  int link_type;
  int link_connected;
  int link_connecting;
  int noqueue;
  int destroy_down;
  int destroy_up;

  dlist_t *dl_server;
  dlist_t *dl_server_cur;

  int fd_link_flags;

  fd_link_t link;

  dlist_t *dl_fd_link;

} graw_t;

module_t gmod_graw_info;

bot_t *graw_init (dlist_t *, bot_t *);
bot_t *graw_fini (dlist_t *, bot_t *);
bot_t *graw_help (dlist_t *, bot_t *);
bot_t *graw_run (dlist_t *, bot_t *);
bot_t *graw_control (dlist_t *, bot_t *, int, va_list);

bot_t *graw_input (dlist_t *, bot_t *);
bot_t *graw_output (dlist_t *, bot_t *);

bot_t *graw_destroy_up (dlist_t *, bot_t *);
bot_t *graw_destroy_down (dlist_t *, bot_t *);

bot_t *graw_control_up (dlist_t *, bot_t *);
bot_t *graw_control_down (dlist_t *, bot_t *);

bot_t *graw_off (dlist_t *, bot_t *);

int graw_input_client (graw_t *);
char *graw_change_string (bot_t *, char *, int);
char *graw_process_options (graw_t *, char *);
void graw_process_options_parse (graw_t *, char *);
void graw_process_options_parse_proto (graw_t *, char *);
void graw_process_options_parse_host (graw_t *, char *);
void graw_process_options_parse_path (graw_t *, char *);
int graw_connect (graw_t *);
int graw_listen (graw_t *);
int graw_accept (graw_t *);
int graw_send_link (graw_t *, char *, int);
int graw_recv_link (graw_t *, char *, int);
int graw_send (graw_t *);
int graw_recv (graw_t *);
pid_t graw_spawner (graw_t *);
pid_t graw_accept_spawner (graw_t *);
pid_t graw_connect_spawner (graw_t *);
int graw_network_raw_listen (char *, int);
int graw_network_unix_listen (char *);
int graw_network_raw_connect (char *, int);
int graw_network_unix_connect (char *);
int graw_set_evhooks_client (graw_t *);
int graw_set_evhooks (graw_t *);
int graw_set_evhooks_accept (graw_t *, fd_link_t *);
int graw_unset_evhooks (graw_t *);
void graw_evhook_link (int, short, void *);
void graw_evhook_accept (int, short, void *);
void graw_disconnect (bot_t *);

bot_t *graw_destroy_up_graw (graw_t *);
bot_t *graw_destroy_down_graw (graw_t *);

void graw_gmod_init (bot_t *, bot_gmod_elm_t *, dlist_t *);

void graw_update_graw (graw_t *, bot_t *);

#endif
