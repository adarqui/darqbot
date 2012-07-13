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
#ifndef GMOD_GFDPASSD_H
#define GMOD_GFDPASSD_H

#define __USE_BSD
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/tcp.h>
#define __USE_BSD
#ifndef u_short
#define u_short unsigned short
#endif
#include <netinet/ip.h>
#undef __USE_BSD
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>

#include "bot.h"

#include <linux/if_ether.h>

typedef struct gmod_gfdpassd {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	int null;

} gfdpassd_t;

module_t gmod_gfdpassd_info;

void __gfdpassd_init__(void) __attribute__ ((constructor));

bot_t *gfdpassd_init(dlist_t *, bot_t *);
bot_t *gfdpassd_fini(dlist_t *, bot_t *);
bot_t *gfdpassd_help(dlist_t *, bot_t *);
bot_t *gfdpassd_run(dlist_t *, bot_t *);
bot_t *gfdpassd_control(dlist_t *, bot_t *, int, va_list);

bot_t *gfdpassd_input(dlist_t *, bot_t *);
bot_t *gfdpassd_output(dlist_t *, bot_t *);

bot_t *gfdpassd_destroy_up(dlist_t *, bot_t *);
bot_t *gfdpassd_destroy_down(dlist_t *, bot_t *);

bot_t *gfdpassd_control_up(dlist_t *, bot_t *);
bot_t *gfdpassd_control_down(dlist_t *, bot_t *);

bot_t *gfdpassd_off(dlist_t *, bot_t *);

int gfdpassd_input_client(gfdpassd_t *);
char *gfdpassd_change_string(bot_t *, char *, int);
char *gfdpassd_process_options(gfdpassd_t *, char *);
void gfdpassd_process_options_parse(gfdpassd_t *, char *);
void gfdpassd_process_options_parse_null(gfdpassd_t *, char *);

bot_t *gfdpassd_destroy_up_gfdpassd(gfdpassd_t *);
bot_t *gfdpassd_destroy_down_gfdpassd(gfdpassd_t *);

void gfdpassd_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void gfdpassd_free(void *);

bot_t *gfdpassd_input_resp_nop(gfdpassd_t *);
bot_t *gfdpassd_input_resp_get(gfdpassd_t *, fdpass_control_op_t *);

#endif
