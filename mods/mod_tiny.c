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
#include "mod_tiny.h"

void __tiny_init__(void)
{

	strlcpy_buf(mod_tiny_info.name, "mod_tiny");
	strlcpy_buf(mod_tiny_info.trigger, "^tiny");

	mod_tiny_info.init = tiny_init;
	mod_tiny_info.fini = tiny_fini;
	mod_tiny_info.help = tiny_help;
	mod_tiny_info.run = tiny_run;

	mod_tiny_info.output = NULL;
	mod_tiny_info.input = NULL;

	debug(NULL, "__tiny_init__: Loaded mod_tiny\n");

	return;
}

bot_t *tiny_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tiny_init: Entered\n");
	return NULL;
}

bot_t *tiny_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tiny_fini: Entered\n");
	return NULL;
}

bot_t *tiny_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tiny_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^tiny <url>";

	return NULL;
}

bot_t *tiny_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "tiny_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "tiny_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = tiny_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *tiny_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL, *ptr, *ptr2;
	char buf[MAX_BUF_SZ];

	CURL *curl;
	CURLcode res;

	char *sep_ptr;
	char *str_url;

	if (!string || !bot)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bz(buf);

	ptr = strstr(string, "://");
	if (!ptr)
		return NULL;

	ptr2 = ptr;
	while (isalnum(*(ptr2 - 1)))
		ptr2--;

	str_url = strtok(ptr2, " ");
	if (!str_url)
		return NULL;

	snprintf_buf(buf, "longurl=%s", str_url);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://adarq.org/t/");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, tiny_curl_write);

		res = curl_easy_perform(curl);

		if (tiny_url) {
			str = tiny_url;
			tiny_url = NULL;
		}

		curl_easy_cleanup(curl);
	}

	return str;
}

size_t tiny_curl_write(void *buf, size_t size, size_t nmemb, void *udata)
{

	char *url;

	if ((url = strstr(buf, "Your lil&#180; URL is: <a href=\""))) {
		url = url + strlen("Your lil&#180; URL is: <a href=\"");
		url = strtok(url, "\"");
		tiny_url = strdup(url);
	}

	return size * nmemb;
}
