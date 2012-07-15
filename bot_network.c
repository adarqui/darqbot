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
#include "bot.h"

char *n_gethostbyname(char *host)
{
	struct hostent *h;
	char *x;

	if (!_sNULL(host))
		return NULL;

	h = gethostbyname(host);
	if (h) {
		x = inet_ntoa(*(struct in_addr *)*h->h_addr_list);
		return strdup(x);
	}

	return NULL;
}

int bot_set_evhooks(bot_t * bot, void (*fn) (int, short, void *))
{

	if (!bot)
		return -1;

	return 0;
}

int bot_unset_evhooks(bot_t * bot)
{
	dlist_t *dptr;
	bot_t *bot_sub_ptr, *bot_parent;

	if (!bot)
		return -1;

	if (bot->node_type == BOT_NODE_TYPE_SUB) {

		if (bot->dptr_self) {
/* this is the paren'ts dptr_self */
			bot_parent = (bot_t *) dlist_data(bot->dptr_self);

			dlist_fornext(bot_parent->dl_subs, dptr) {
				bot_sub_ptr = (bot_t *) dlist_data(dptr);

				if (bot_sub_ptr == bot) {
					dlist_remove(&bot_parent->dl_subs,
						     dptr);
					return 0;
				}

			}

		}
		return 0;

	}

	return 0;
}

int bot_network_raw_connect(char *host, int port, int type, int alarm_val)
{
	struct sockaddr_in sin;
	char *str_tmp;
	int fd, n;
	if (!_sNULL(host) || !port)
		return -1;

	bz2(sin);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	str_tmp = n_gethostbyname(host);
	if (!_sNULL(str_tmp))
		return -1;

	sin.sin_addr.s_addr = inet_addr(str_tmp);
	free(str_tmp);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;

	if (alarm_val > 0) {
		alarm(alarm_val);
	}

	n = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
	if (alarm_val > 0) {
		alarm(0);
	}

	if (n < 0) {
		safe_close(fd);
		return -1;
	}

	return fd;
}

int bot_network_raw_listen(char *host, int port)
{
	struct sockaddr_in sin;
	int fd;
	char *str_tmp = NULL;

	debug(NULL, "bot_network_raw_listen: Entered\n");

	if (!_sNULL(host) || port <= 0)
		return -1;

	bz2(sin);
	str_tmp = n_gethostbyname(host);
	if (!_sNULL(str_tmp))
		return -1;

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(str_tmp);
	sin.sin_port = htons(port);

	free(str_tmp);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd <= 0) {
		goto cleanup;
	}

	if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		goto cleanup;
	}

	if (listen(fd, 10) < 0) {
		goto cleanup;
	}

	debug(NULL, "bot_network_raw_listen: SUCCESS %i\n", fd);

	return fd;

 cleanup:

	if (fd > 0)
		safe_close(fd);

	return -1;
}

int bot_network_raw_disconnect(int fd)
{
	safe_close(fd);
	return 0;
}
