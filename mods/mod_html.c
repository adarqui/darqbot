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
#include "mod_html.h"

void __html_init__(void)
{

	strlcpy_buf(mod_html_info.name, "mod_html");
	strlcpy_buf(mod_html_info.trigger, "^html");

	mod_html_info.init = html_init;
	mod_html_info.fini = html_fini;
	mod_html_info.help = html_help;
	mod_html_info.run = html_run;

	mod_html_info.output = NULL;
	mod_html_info.input = NULL;

	mod_html_info.max_nesting = 2;

	debug(NULL, "__html_init__: Loaded mod_html\n");

	return;
}

bot_t *html_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "html_init: Entered\n");

	return NULL;
}

bot_t *html_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "html_fini: Entered\n");
	return NULL;
}

bot_t *html_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "html_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^html";

	return NULL;
}

bot_t *html_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *new_str = NULL;
	int opt;

	char **argv = NULL;
	int argc = 0;

	int c;

	html_info_t *si = NULL;

	debug(bot, "html_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "html_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	if (bot->var_nesting_cur > mod_html_info.max_nesting)
		return NULL;

	opt = 0;
	new_str = NULL;

	si = html_initx(bot);
	if (!si)
		return NULL;

	MOD_OPTIONS_TOP_HALF;

	argv =
	    tokenize_str2argv(dl_options_ptr, &argc, TOKENIZE_STR2ARGV_ARGV0);
	if (argv) {

		optind = 0;

		while ((c = getopt(argc, argv, "p:r:P:fhu:c:vs")) != -1) {
			switch (c) {
			case 'P':
				{
/* PROXY */
					opt |= MOD_HTML_OPT_PROXY;
					if (si->opt_proxy)
						free(si->opt_proxy);
					si->opt_proxy = strdup(optarg);
					break;
				}
			case 'r':
				{
					opt |= MOD_HTML_OPT_REFERER;
					if (si->opt_referer)
						free(si->opt_referer);
					si->opt_referer = strdup(optarg);
					break;
				}
			case 'f':
				{
					opt |= MOD_HTML_OPT_FOLLOWLOCATION;
					break;
				}
			case 'p':
				{
					opt |= MOD_HTML_OPT_POST;
					if (si->opt_postfields)
						free(si->opt_postfields);
					si->opt_postfields = strdup(optarg);
					break;
				}
			case 'h':
				{
					opt |= MOD_HTML_OPT_HELP;
					break;
				}
			case 'u':
				{
					opt |= MOD_HTML_OPT_USERAGENT;
					if (si->opt_useragent)
						free(si->opt_useragent);
					si->opt_useragent = strdup(optarg);
					break;
				}
			case 's':
				{
					opt |= MOD_HTML_OPT_SOCKS;
					break;
				}
			case 'c':
				{
					opt |= MOD_HTML_OPT_COOKIE;
					if (si->opt_cookie)
						free(si->opt_cookie);
					si->opt_cookie = strdup(optarg);
					break;
				}
			case 'v':
				{
					opt |= MOD_HTML_OPT_VERBOSE;
					break;
				}
			case 'H':
				{
					opt |= MOD_HTML_OPT_HEADER;
					break;
				}
			default:
				break;
			}

		}

		tokenize_destroy_array(NULL, argv);
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;

	if (!bot_fork_clean(bot)) {
		si->opt_flags = opt;
		l_new_str = html_change_string(bot, si, l_str_ptr);
	}

	MOD_PARSE_BOTTOM_HALF_NODL;

	if (si)
		html_finix(bot, si);

	return bot;
}

html_info_t *html_get(bot_t * bot, html_info_t * si)
{
	char *url = NULL;

	debug(NULL, "html_get: Entered\n");

	if (si->curl) {
		curl_easy_cleanup(si->curl);
	}

	si->curl = curl_easy_init();

	url = si->url;

	curl_easy_setopt(si->curl, CURLOPT_URL, url);
	curl_easy_setopt(si->curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(si->curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(si->curl, CURLOPT_COOKIEJAR, si->cookie_file);
	curl_easy_setopt(si->curl, CURLOPT_COOKIEFILE, si->cookie_file);
	curl_easy_setopt(si->curl, CURLOPT_HEADER, 1);
	curl_easy_setopt(si->curl, CURLOPT_WRITEFUNCTION, html_curl_write);
	curl_easy_setopt(si->curl, CURLOPT_WRITEDATA, si);

	curl_easy_setopt(si->curl, CURLOPT_READFUNCTION, html_curl_read);

	if (si->opt_flags & MOD_HTML_OPT_VERBOSE)
		curl_easy_setopt(si->curl, CURLOPT_VERBOSE, 1);

	if (si->opt_flags & MOD_HTML_OPT_FOLLOWLOCATION)
		curl_easy_setopt(si->curl, CURLOPT_FOLLOWLOCATION, 1);

	if (si->opt_flags & MOD_HTML_OPT_REFERER)
		curl_easy_setopt(si->curl, CURLOPT_REFERER, si->opt_referer);
	else
		curl_easy_setopt(si->curl, CURLOPT_AUTOREFERER, 1);

	if (si->opt_flags & MOD_HTML_OPT_USERAGENT)
		curl_easy_setopt(si->curl, CURLOPT_USERAGENT,
				 si->opt_useragent);

	if (si->opt_flags & MOD_HTML_OPT_COOKIE)
		curl_easy_setopt(si->curl, CURLOPT_COOKIE, si->opt_cookie);

	if (si->opt_flags & MOD_HTML_OPT_PROXY)
		curl_easy_setopt(si->curl, CURLOPT_PROXY, si->opt_proxy);

	if (si->opt_flags & MOD_HTML_OPT_SOCKS)
		curl_easy_setopt(si->curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

	if (si->opt_flags & MOD_HTML_OPT_POST) {
		curl_easy_setopt(si->curl, CURLOPT_POST, 1);
		curl_easy_setopt(si->curl, CURLOPT_POSTFIELDS,
				 si->opt_postfields);
	}

	if (si->opt_flags & MOD_HTML_OPT_HEADER) {
		curl_easy_setopt(si->curl, CURLOPT_HEADER, 1);
	} else {
		curl_easy_setopt(si->curl, CURLOPT_HEADER, 0);
	}

	si->curl_res = curl_easy_perform(si->curl);
	curl_easy_cleanup(si->curl);

	si->curl = NULL;

	return si;
}

char *html_change_string(bot_t * bot, html_info_t * si, char *string)
{
	char *str = NULL;
	char *sep_ptr;

	debug(NULL, "html_change_string: Entered\n");

	if (!bot || !si || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	si->url = strdup(string);

	if (si->opt_flags & MOD_HTML_OPT_HELP) {
		str = html_op_help(si);
		return str;
	}

	if (!html_get(bot, si)) {
		bot_fork_clean_exit(bot);
	}

	str = dlist_to_str(si->dl_text);

	return str;
}

size_t html_curl_write(void *buf, size_t size, size_t nmemb, void *udata)
{
	html_info_t *si;

	si = (html_info_t *) udata;

	if (!si)
		return size * nmemb;

	dl_str_unite(&si->dl_text, "%s", buf);

	return size * nmemb;
}

size_t html_curl_read(void *buf, size_t size, size_t nmemb, void *udata)
{

	return size * nmemb;
}

html_info_t *html_finix(bot_t * bot, html_info_t * si)
{

	if (!si)
		return NULL;

	if (si->url)
		free(si->url);

	if (si->cookie_file)
		free(si->cookie_file);

	if (si->opt_proxy)
		free(si->opt_proxy);

	if (si->opt_referer)
		free(si->opt_referer);

	if (si->opt_postfields)
		free(si->opt_postfields);

	if (si->opt_useragent)
		free(si->opt_useragent);

	if (si->opt_cookie)
		free(si->opt_cookie);

	dl_str_destroy(&si->dl_text);

	memset(si, 0, sizeof(si));
	free(si);

	return NULL;
}

char *html_fix_url(char *s)
{
	char *b = s;
	if (!s)
		return NULL;

	while (*s) {
		if (*s == ' ')
			*s = '-';
		s++;
	}

	return b;
}

html_info_t *html_initx(bot_t * bot)
{
	html_info_t *si;

	si = (html_info_t *) calloc(1, sizeof(html_info_t));
	if (!si)
		return NULL;

	si->curl = curl_easy_init();
	if (!si->curl)
		return NULL;

	si->cookie_file = strdup(tmpnam(NULL));

	return si;
}

char *html_op_help(html_info_t * si)
{
	char *str = NULL;
	debug(NULL, "html_op_help: Entered\n");
	if (!si)
		return NULL;

	str = str_unite("^html help: ^html(options) url\n"
			" (-P : <proxy_url>)\n"
			" (-r : <referer>)\n"
			" (-p : <postfields,post>)\n"
			" (-u : <useragent>)\n"
			" (-c : <cookie>)\n"
			" (-s : proxy uses socks)\n"
			" (-v : verbose)\n" " (-f : follow)\n"
			" (-h : help)\n");

	return str;
}
