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

void module_sort(void)
{
	xmodule_sort(XMODULE_TYPE_MODULE);
	return;
}

void module_update_stats(void)
{
	xmodule_update_stats(XMODULE_TYPE_MODULE);
	return;
}

void module_lock(char *name)
{
	xmodule_lock(XMODULE_TYPE_MODULE, name);
	return;
}

void module_unlock(char *name)
{
	xmodule_unlock(XMODULE_TYPE_MODULE, name);
	return;
}

module_t *module_find_by_name(char *name)
{
	return xmodule_find_by_name(XMODULE_TYPE_MODULE, name);
}

module_t *module_find_by_trig(char *name)
{
	return xmodule_find_by_trig(XMODULE_TYPE_MODULE, name);
}

dlist_t *module_find_by_trig_dptr(char *name)
{
	return xmodule_find_by_trig_dptr(XMODULE_TYPE_MODULE, name);
}

void module_update(module_t * mod_struct, char *name)
{
	xmodule_update(XMODULE_TYPE_MODULE, mod_struct, name);
	return;
}

char *module_path(char *name)
{

	return xmodule_path(XMODULE_TYPE_MODULE, name);
}

module_t *module_load(char *name)
{
	return xmodule_load(XMODULE_TYPE_MODULE, name);
}

module_t *module_unload(module_t * mod)
{
	return xmodule_unload(XMODULE_TYPE_MODULE, mod, NULL);
}

void module_iolist(void)
{
	xmodule_iolist(XMODULE_TYPE_MODULE);
	return;
}

void module_timerlist(void)
{
	xmodule_timerlist(XMODULE_TYPE_MODULE);
	return;
}

void module_list(void)
{
	xmodule_list(XMODULE_TYPE_MODULE);
	return;
}

void module_load_all(void)
{

	xmodule_load_all(XMODULE_TYPE_MODULE);

	return;
}

bot_t *module_unload_all(void)
{
	return xmodule_unload_all(XMODULE_TYPE_MODULE);
}

/* console parsing stuff */
void console_modload(char *arg)
{

	console_xmodload(XMODULE_TYPE_MODULE, arg);

	return;
}

void console_modunload(char *arg)
{

	console_xmodunload(XMODULE_TYPE_MODULE, arg);
	return;
}

void console_modreload(char *arg)
{

	console_xmodreload(XMODULE_TYPE_MODULE, arg);

	return;
}

void modules_off(void)
{
	dlist_t *dptr = NULL;
	module_t *mod = NULL;

	debug(NULL, "modules_off: Entered\n");

	dlist_fornext(gi->dl_module, dptr) {

		if (!dptr)
			continue;

		mod = (module_t *) dlist_data(dptr);

		if (!mod)
			continue;
		if (mod->off) {
			mod->off(NULL, NULL);
		}
	}

	return;
}
