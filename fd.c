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
/* these routines are used to track & destroy multiple file descriptors used by bot objects */

#include "bot.h"

fd_link_t *fd_link_add(dlist_t ** dl_fd_link, int fd, struct event ev,
		       struct sockaddr_in *sin, struct sockaddr_un *sun)
{
	fd_link_t *bfl = NULL;

	debug(NULL, "fd_link_add: Entered: fd=%i\n", fd);

	if (!dl_fd_link || fd < 0)
		return NULL;

	bfl = (fd_link_t *) calloc(1, sizeof(fd_link_t));
	if (!bfl)
		return NULL;

	bfl->fd = fd;

/* memcpy this? FIXFIX */
	bfl->ev = ev;

	if (sin) {
		bfl->type = AF_INET;
		bfl->sin.sin_family = AF_INET;
		memcpy(&bfl->sin, sin, sizeof(struct sockaddr_in));
	} else if (sun) {
		bfl->type = AF_UNIX;
		bfl->sun.sun_family = AF_UNIX;
		memcpy(&bfl->sun, sun, sizeof(struct sockaddr_un));
	}

	dlist_Dinsert_after(dl_fd_link, bfl);

	puts("FDLINK: ADDED");
	fd_link_print(bfl);

	return bfl;
}

fd_link_t *fd_link_find(dlist_t ** dl_fd_link, int fd)
{
	dlist_t *dptr_bfl;
	fd_link_t *bfl;

	debug(NULL, "fd_link_find: Entered: fd=%i\n", fd);

	if (!dl_fd_link || fd < 0)
		return NULL;

	dlist_fornext(*dl_fd_link, dptr_bfl) {
		bfl = (fd_link_t *) dlist_data(dptr_bfl);
		if (bfl->fd == fd) {
			debug(NULL, "fd_link_find: FDLINK: FOUND: %i\n",
			      bfl->fd);
			fd_link_print(bfl);
			return bfl;
		}
	}

	return NULL;
}

int fd_link_del(dlist_t ** dl_fd_link, int fd, void (*cleanup) (fd_link_t *))
{
	dlist_t *dptr_bfl = NULL;
	fd_link_t *bfl = NULL;

	debug(NULL, "fd_link_del: Entered: fd=%i\n", fd);

	if (!dl_fd_link || fd < 0 || !cleanup)
		return -1;

	dlist_fornext(*dl_fd_link, dptr_bfl) {
		bfl = (fd_link_t *) dlist_data(dptr_bfl);

		if (bfl->fd == fd) {
			cleanup(bfl);
			dlist_remove_and_free(dl_fd_link, &dptr_bfl, free);
			debug(NULL, "fd_link_find: FDLINK: DEL'D: %i\n", fd);
			break;
		}

	}

	return 1;
}

void fd_link_destroy(dlist_t ** dl_fd_link, void (*cleanup) (fd_link_t *))
{
	dlist_t *dptr_bfl = NULL, *dptr_tmp;
	fd_link_t *bfl = NULL;

	debug(NULL, "fd_link_destroy: Entered\n");

	if (!dl_fd_link || !cleanup) {
		return;
	}

	dlist_fornext_retarded(*dl_fd_link, dptr_bfl, dptr_tmp) {
		if (!dptr_bfl)
			break;

		bfl = (fd_link_t *) dlist_data(dptr_bfl);

		cleanup(bfl);
		dlist_remove_and_free(dl_fd_link, &dptr_bfl, free);
	}

	debug(NULL, "fd_link_destroy: Finished\n");
	return;
}

void fd_link_cleanup(fd_link_t * bfl)
{
	debug(NULL, "fd_link_cleanup: Entered\n");
	if (!bfl)
		return;

	if (bfl->fd > 0) {
		safe_close(bfl->fd);

		event_del(&bfl->ev);

		memset(bfl, 0, sizeof(fd_link_t));
	}

	return;
}

void fd_link_set_cur(dlist_t ** dl_fd_link, fd_link_t * bfl_arg, int fd)
{
/* searches a dlist_t, finds the appropriate bfl, sets current bot-> vars */
	fd_link_t *bfl = NULL;

	debug(NULL, "fd_link_set_cur: fd=%i\n", fd);

	if (!dl_fd_link || !bfl_arg || fd < 0)
		return;

	bfl = fd_link_find(dl_fd_link, fd);
	if (!bfl)
		return;

	bfl_arg->type = bfl->type;
	bfl_arg->fd = bfl->fd;
	memcpy(&bfl_arg->ev, &bfl->ev, sizeof(struct event));
	memcpy(&bfl_arg->sin, &bfl->sin, sizeof(struct sockaddr_in));
	memcpy(&bfl_arg->sun, &bfl->sun, sizeof(struct sockaddr_un));

	return;
}

void fd_link_print(fd_link_t * bfl)
{
	debug(NULL, "fd_link_print: Entered\n");
	if (!bfl)
		return;

	debug(NULL,
	      "bfl->fd=%i, bfl->sin.sin_addr=%s, bfl->sin.sin_port=%i, bfl->sun.un_path=%s\n",
	      bfl->fd, inet_ntoa(bfl->sin.sin_addr),
	      ntohs(bfl->sin.sin_port), bfl->sun.sun_path);

	return;
}

char *fd_link_get_latest_host(bot_t * bot, char *trigger, char *trigger_ext)
{
/* returns the latest "connection source" down the gmod list */
	dlist_t *dptr_gmod, *dptr;
	bot_gmod_elm_t *gmod;
	fd_link_t *bfl;
	char *str = NULL;

	debug(NULL, "fd_link_get_latest_host: Entered\n");

	if (!bot || !trigger)
		return NULL;

	dptr_gmod = gmodule_find_gmod_dptr(bot, trigger, trigger_ext);
	if (!dptr_gmod)
		return NULL;

	dlist_forprev(dptr_gmod, dptr) {
		gmod = (bot_gmod_elm_t *) dlist_data(dptr);
		if (!gmod)
			return NULL;

		bfl = &gmod->link;

		switch (bfl->type) {
		case AF_INET:
			{
				str = strdup(inet_ntoa(bfl->sin.sin_addr));
				return str;
			}
		case AF_UNIX:
			{
				if (sNULL(bfl->sun.sun_path) != NULL)
					str = strdup(bfl->sun.sun_path);
				return str;
			}
		default:
			continue;
		}

	}

	return str;
}
