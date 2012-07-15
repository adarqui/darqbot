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
#include "mod_tinypaw.h"

void __tinypaw_init__(void)
{

	strlcpy_buf(mod_tinypaw_info.name, "mod_tinypaw");
	strlcpy_buf(mod_tinypaw_info.trigger, "^tinypaw");

	mod_tinypaw_info.init = tinypaw_init;
	mod_tinypaw_info.fini = tinypaw_fini;
	mod_tinypaw_info.help = tinypaw_help;
	mod_tinypaw_info.run = tinypaw_run;

	mod_tinypaw_info.output = NULL;
	mod_tinypaw_info.input = NULL;

	debug(NULL, "__tinypaw_init__: Loaded mod_tinypaw\n");

	return;
}

bot_t *tinypaw_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tinypaw_init: Entered\n");
	return NULL;
}

bot_t *tinypaw_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tinypaw_fini: Entered\n");
	return NULL;
}

bot_t *tinypaw_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tinypaw_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^tinypaw";

	return NULL;
}

bot_t *tinypaw_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "tinypaw_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "tinypaw_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_ARG;
	l_new_str = tinypaw_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_ARG;

	return bot;
}

char *tinypaw_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL, *ptr, *ptr2;
	char buf[MAX_BUF_SZ];
	int i;

	CURL *curl;
	CURLcode res;

	char *sep_ptr;

	char *str_url;
	char *str_keywords;

	if (!string || !bot)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	ptr = strstr(string, "://");
	if (!ptr)
		return NULL;

	ptr2 = ptr;
	while (isalnum(*(ptr2 - 1)))
		ptr2--;

	str_url = strtok(ptr2, " ");
	if (!str_url)
		return NULL;

	snprintf_buf(buf, "longurl=%s&keywords=", str_url);

	ptr = buf + strlen(buf);

	str_keywords = strtok(NULL, "");
	if (!str_keywords) {
		if (!_sNULL(bot->txt_data_out))
			return NULL;
		strlcat_bot(buf, bot->txt_data_out);
	} else {
		strlcat_bot(buf, str_keywords);
	}

	for (i = 0; i < strlen(ptr); i++) {
		if (ptr[i] == '&')
			ptr[i] = ' ';
	}

	ptr2 = buf + strlen(buf);
	while (!isalnum(*ptr2)) {
		*ptr2 = '\0';
		ptr2--;
	}

	curl = curl_easy_init();
	if (curl) {

		curl_easy_setopt(curl, CURLOPT_URL, "http://adarq.org/tp/");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
				 tinypaw_curl_write);

		res = curl_easy_perform(curl);

		if (tinypaw_url) {
			str = tinypaw_url;
			tinypaw_url = NULL;
		}

		curl_easy_cleanup(curl);
	}

	return str;
}

size_t tinypaw_curl_write(void *buf, size_t size, size_t nmemb, void *udata)
{

	char *url;

	if ((url = strstr(buf, "Your tinypaw URL is: <a href=\""))) {
		url = url + strlen("Your tinypaw URL is: <a href=\"");
		url = strtok(url, "\"");
		tinypaw_url = strdup(url);
	}

	return size * nmemb;
}
