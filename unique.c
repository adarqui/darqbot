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

unique_t *unique_create(bot_t * bot, dlist_t ** dl_unique, int id_flags)
{
	dlist_t *dptr_bu = NULL;
	unique_t *bu = NULL;

	if (!bot || !dl_unique)
		return NULL;

	bu = unique_find(bot, dl_unique);
	if (bu)
		return bu;

	bu = (unique_t *) calloc(1, sizeof(unique_t));
	if (!bu)
		return NULL;

	bu->id_flags = id_flags;

	if (id_flags & UNIQUE_ID_NICK) {
		bu->nick = strdup(bot->txt_nick);
	}
	if (id_flags & UNIQUE_ID_CHAN) {
		bu->chan = strdup(bot->txt_to);
	}
	if (id_flags & UNIQUE_ID_HOST) {
		bu->host = strdup(bot->txt_host);
	}
	if (id_flags & UNIQUE_ID_TAG) {
		bu->tag = strdup(bot->tag);
	}
	if (id_flags & UNIQUE_ID_ID) {
		bu->ID = bot->ID;
	}

	dptr_bu = dlist_Dinsert_after(dl_unique, bu);
	bu->dptr_self = dptr_bu;

	return bu;
}

unique_t *unique_find(bot_t * bot, dlist_t ** dl_unique)
{
	dlist_t *dptr = NULL;
	unique_t *bu = NULL;

	if (!bot || !dl_unique)
		return NULL;

	dlist_fornext(*dl_unique, dptr) {
		bu = (unique_t *) dlist_data(dptr);
		if (!bu)
			continue;

		if (bu->id_flags & UNIQUE_ID_NICK) {
			if (strcasecmp(bu->nick, bot->txt_nick))
				continue;
		}

		if (bu->id_flags & UNIQUE_ID_CHAN) {
			if (strcasecmp(bu->chan, bot->txt_to))
				continue;
		}

		if (bu->id_flags & UNIQUE_ID_HOST) {
			if (strcasecmp(bu->host, bot->txt_host))
				continue;
		}

		if (bu->id_flags & UNIQUE_ID_TAG) {
			if (strcasecmp(bu->tag, bot->tag))
				continue;
		}
		if (bu->id_flags & UNIQUE_ID_ID) {
			if (bu->ID != bot->ID)
				continue;
		}

		return bu;
	}

	return NULL;
}

void *unique_get(bot_t * bot, dlist_t ** dl_unique)
{
	unique_t *bu = NULL;

	if (!bot || !dl_unique)
		return NULL;

	bu = unique_find(bot, dl_unique);
	if (!bu)
		return NULL;

	return bu->data;
}

unique_t *unique_delete(bot_t * bot, dlist_t ** dl_unique, void (*fn) (void *))
{
	dlist_t *dptr = NULL;
	unique_t *bu = NULL;

	if (!bot || !dl_unique)
		return NULL;

	bu = unique_find(bot, dl_unique);
	if (!bu)
		return NULL;

	if (fn)
		fn(bu->data);

	dptr = dlist_remove(dl_unique, bu->dptr_self);
	if (dptr)
		free(dptr);

	unique_free(bu);

	return NULL;
}

void unique_free(void *arg)
{
	unique_t *bu = (unique_t *) arg;

	if (!bu)
		return;

	if (bu->nick)
		free(bu->nick);

	if (bu->chan)
		free(bu->chan);

	if (bu->host)
		free(bu->host);

	if (bu->tag)
		free(bu->tag);

	free(bu);

	return;
}

void unique_destroy(bot_t * bot, dlist_t ** dl_unique, void (*fn) (void *))
{
	dlist_t *dptr_a, *dptr_b, *dptr_c;
	unique_t *bu = NULL;

	if (!bot || !dl_unique || !fn)
		return;

	dlist_fornext_retarded(*dl_unique, dptr_a, dptr_b) {
		if (!dptr_a)
			break;
		dptr_c = dlist_remove(dl_unique, dptr_a);
		if (dptr_c) {
			bu = (unique_t *) dlist_data(dptr_c);
			fn(bu->data);
			unique_free(bu);
			free(dptr_c);
		}
	}

	return;
}

char *unique_bu2str(unique_t * bu, int flags)
{
	char *str = NULL;
	int c = '\0';

	if (!bu) {
		return NULL;
	}

	if (flags & UNIQUE_BU2STR_NEWLINE)
		c = '\n';

	str =
	    str_unite("tag=[%s] : host=[%s] : nick=[%s] : chan=[%s]%c", bu->tag,
		      bu->host, bu->nick, bu->chan, c);

	return str;
}
