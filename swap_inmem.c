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

swap_inmem_t *swap_inmem_create(char *name, int id, void *data_ptr)
{

	swap_inmem_t *swapim = NULL;

	if (!name || id < 0 || !data_ptr) {
	}

	swapim = (swap_inmem_t *) calloc(1, sizeof(swap_inmem_t));
	swapim->name = strdup(name);
	swapim->id = id;
	swapim->data = data_ptr;

	return swapim;
}

void swap_inmem_add(swap_inmem_t * swapim)
{

	if (!swapim)
		return;

	dlist_Dinsert_after(&dl_swapim, swapim);

	return;
}

swap_inmem_t *swap_inmem_create_and_add(char *name, int id, void *data_ptr)
{

	swap_inmem_t *swapim = NULL;

	swapim = swap_inmem_create(name, id, data_ptr);
	if (!swapim)
		return NULL;

	swap_inmem_add(swapim);

	return swapim;
}

dlist_t *swap_inmem_get(char *name, int id)
{
	dlist_t *dptr = NULL;
	swap_inmem_t *swapim = NULL;

	if (!name || id < 0)
		return NULL;

	dlist_fornext(dl_swapim, dptr) {
		swapim = (swap_inmem_t *) dlist_data(dptr);
		if (!strcasecmp(swapim->name, name) && swapim->id == id) {
			return dptr;
		}
	}

	return NULL;
}

void swap_inmem_fini(void)
{
	return;
}

void swap_inmem_free(void *arg)
{
	swap_inmem_t *swapim = NULL;
	if (!arg)
		return;

	swapim = (swap_inmem_t *) swapim;

	if (swapim->name)
		free(swapim->name);

	free(swapim);
	return;
}

void swap_inmem_remove(dlist_t ** dptr)
{
	dlist_t *dptr_res;
	if (!dptr)
		return;
	dptr_res = dlist_remove(&dl_swapim, *dptr);
	if (dptr_res)
		free(dptr_res);
	*dptr = NULL;
	return;
}

void swap_inmem_get_assign_and_remove(char *name, int id, void **data_ptr)
{
	swap_inmem_t *swapim = NULL;
	dlist_t *dptr_swapim = NULL;
	if (!name || !data_ptr) ;

	*data_ptr = NULL;
	dptr_swapim = swap_inmem_get(name, id);
	if (dptr_swapim) {
		swapim = (swap_inmem_t *) dlist_data(dptr_swapim);
		*data_ptr = swapim->data;
		swap_inmem_remove(&dptr_swapim);
	}

	return;
}

void swap_inmem_console_info(bot_t * bot)
{
	dlist_t *dptr_swapim = NULL;
	swap_inmem_t *swapim = NULL;
	int i = 0;

	debug(NULL, "swap_inmem_console_info: Entered\n");

	dlist_fornext(dl_swapim, dptr_swapim) {
		swapim = (swap_inmem_t *) dlist_data(dptr_swapim);
		bot_dl_print(NULL,
			     "swap_inmem: %i. name=%s, id=%i, data_ptr=%p\n", i,
			     swapim->name, swapim->id, swapim->data);
	}

	return;
}
