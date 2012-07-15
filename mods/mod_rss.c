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
#include "mod_rss.h"

void __rss_init__(void)
{

	strlcpy_buf(mod_rss_info.name, "mod_rss");
	strlcpy_buf(mod_rss_info.trigger, "^rss");

	mod_rss_info.init = rss_init;
	mod_rss_info.fini = rss_fini;
	mod_rss_info.help = rss_help;
	mod_rss_info.run = rss_run;

	mod_rss_info.output = NULL;
	mod_rss_info.input = NULL;

	mod_rss_info.max_nesting = 5;

	debug(NULL, "__rss_init__: Loaded mod_rss\n");

	return;
}

bot_t *rss_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rss_init: Entered\n");
	return NULL;
}

bot_t *rss_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rss_fini: Entered\n");
	return NULL;
}

bot_t *rss_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rss_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^rss <url> || ^rss(tiny,adarq,update,add,del,info,nofork) <...>";

	return NULL;
}

bot_t *rss_run(dlist_t * dlist_node, bot_t * bot)
{

	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt, opt_2;

	debug(bot, "rss_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "rss_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot)) {
		return NULL;
	}

	if (bot->var_nesting_cur > mod_rss_info.max_nesting) {
		printf("MAXNESTING: var_nesting_cur=%i, max_nesting=%i\n",
		       bot->var_nesting_cur, mod_rss_info.max_nesting);
		return NULL;
	}

	rss_url = NULL;

	opt = 5;

	if (rss_method) {
		opt_2 = rss_method;
	} else {
		opt_2 = RSS_OPT_METHOD_ADARQ;
	}

	MOD_OPTIONS_TOP_HALF;

	opt = atoi(dl_options_ptr);

	if (xstrcasestr(dl_options_ptr, "tiny")) {
		opt_2 |= RSS_OPT_METHOD_TINYURL;
		rss_method = RSS_OPT_METHOD_TINYURL;
	} else if (xstrcasestr(dl_options_ptr, "adarq")) {
		opt_2 |= RSS_OPT_METHOD_ADARQ;
		rss_method = RSS_OPT_METHOD_ADARQ;
	}

	if (xstrcasestr(dl_options_ptr, "update")) {
		opt_2 |= RSS_OPT_UPDATE;
	} else if (xstrcasestr(dl_options_ptr, "add")) {
		opt_2 |= RSS_OPT_ADD;
	} else if (xstrcasestr(dl_options_ptr, "del")) {
		opt_2 |= RSS_OPT_DEL;
	} else if (xstrcasestr(dl_options_ptr, "info")) {
		opt_2 |= RSS_OPT_INFO;
	}

	if (xstrcasestr(dl_options_ptr, "nofork")) {
		opt_2 |= RSS_OPT_NOFORK;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	if (opt == 0)
		opt = 5;

	if (rss_dl_urls) {
		dlist_fini(&rss_dl_urls, rss_url_free);
		rss_dl_urls = NULL;
	}

	if (rss_fp) {
		fclose(rss_fp);
		rss_fp = NULL;
	}

	MOD_PARSE_TOP_HALF_NODL;

	if (opt_2 & RSS_OPT_NOFORK) {
		l_new_str = rss_change_string(bot, l_str_ptr, opt, opt_2);
	} else {
		if (!bot_fork_clean(bot)) {
			l_new_str =
			    rss_change_string(bot, l_str_ptr, opt, opt_2);
		}
	}

	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *rss_change_string(bot_t * bot, char *string, int opt, int opt_2)
{

	char *str = NULL;
	char *sep_ptr;

	debug(NULL, "rss_change_string: opt=%i, opt_2=%i\n", opt, opt_2);

	if (!string || !bot)
		return NULL;

	if (!opt)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (opt_2 & RSS_OPT_ADD) {
		str = rss_op_add(bot, string);
	} else if (opt_2 & RSS_OPT_DEL) {
		str = rss_op_del(bot, string);
	} else if (opt_2 & RSS_OPT_INFO) {
		str = rss_op_info(bot, string);
	} else {
		str = rss_op_run(bot, string, opt, opt_2);
	}

	return str;
}

char *rss_op_add(bot_t * bot, char *string)
{
	DB *db;
	int n;
	char *db_name;
	char *str = NULL;
	char *key_name, *key_value;

	debug(NULL, "rss_op_add: Entered\n");
	if (!bot || !string)
		return NULL;

	key_name = strtok(string, " ");
	if (!key_name)
		return NULL;

	key_value = strtok(NULL, "");
	if (!key_value)
		return NULL;

	db_name = rss_ret_urldb_str();
	if (!db_name)
		return NULL;

	db = xdb_open(db_name);
	if (!db)
		return NULL;

	n = xdb_write(db, key_name, key_value);

	xdb_fini(db);

	debug(NULL, "rss_op_add: Done: added %s %s", key_name, key_value);

	return str;
}

char *rss_op_info(bot_t * bot, char *string)
{
	DB *db;
	xdb_pair_t *pair;
	int i, count;
	char *db_name = NULL;
	char buf[MAX_BUF_SZ], *str = NULL, *string_ptr;

	debug(NULL, "rss_op_info: Entered: %s\n", string);

	if (!bot || !string)
		return NULL;

	db_name = rss_ret_urldb_str();
	if (!db_name)
		return NULL;

	db = xdb_open(db_name);
	if (!db) {
		return NULL;
	}

	bz(buf);

	string_ptr = _eat_whitespace(string);
	if (!_sNULL(string_ptr)) {
/* return all keys */
		count = xdb_count(db);
		if (count <= 0)
			goto cleanup;

		for (i = 1; i <= count; i++) {
			pair = xdb_get_recnum(db, i);
			if (!pair)
				continue;

			strlcatfmt_buf(buf, "%s ", pair->key);
			xdb_pair_destroy(pair);
		}
	} else {
/* return one key */
		_strstrip_chars(string, " \r\n\t");
		pair = xdb_get(db, string);
		if (pair) {
			strlcatfmt_buf(buf, "%s %s", pair->key, pair->value);
			xdb_pair_destroy(pair);
		}
	}

 cleanup:
	if (db)
		xdb_fini(db);

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *rss_op_run(bot_t * bot, char *string, int opt, int opt_2)
{
/* opt = how many links to return, opt_2 = type of tinyurl service to use */
	dlist_t *dptr;

	char *str = NULL, *ptr, *ptr2;
	char buf[MAX_BUF_SZ];

	rss_url_t *rssurl_ptr;

	CURL *curl;
	CURLcode res;

	char *str_url, *str_url_dup = NULL, *str_key_dup = NULL;

	int i, j, k;

	if (!string || !bot)
		return NULL;

	if (!opt)
		return NULL;

	bz(buf);

	_strstrip_chars(string, " \r\n\t");

	ptr = strstr(string, "://");
	if (!ptr) {
/* user supplied key */
		str_url_dup = rss_ret_url_from_key(string);
		if (!str_url_dup)
			return NULL;
		str_url = str_url_dup;
		str_key_dup = strdup(string);
	} else {
/* user supplied url */
		printf("ptr=%s\n", ptr);

		ptr2 = ptr;
		while (isalnum(*(ptr2 - 1)))
			ptr2--;

		str_url = strtok(ptr2, " ");
		if (!str_url)
			return NULL;

		str_key_dup = _strrchr(str_url, '/');
		if (str_key_dup) {
			str_key_dup = strdup(str_key_dup + 1);
		}
	}

	debug(bot, "rss_change_string: about to initiate curl\n");

	curl = curl_easy_init();
	if (curl) {

		str =
		    str_unite_static("/tmp/mod_rss-%i%i.xml", getpid(), rand());
		if (!_sNULL(str))
			goto cleanup;

		rss_fp = fopen(str, "w");
		if (!rss_fp) {
			goto cleanup;
		}

		curl_easy_setopt(curl, CURLOPT_URL, str_url);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
				 rss_curl_write_rssurls);
		curl_easy_setopt(curl, CURLOPT_USERAGENT,
				 "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

		res = curl_easy_perform(curl);
		printf("str_url=%s, res=%i\n", str_url, res);

		if (rss_parse_xml(str) < 0) {
			debug(bot, "rss_change_string: FAILED\n");
			unlink(str);
			goto cleanup;
		}

		unlink(str);

		str[0] = '\0';

		if (rss_dl_urls) {
			rss_update(bot, &rss_dl_urls, opt_2);
		}

		if (rss_dl_urls) {
			if (opt > 0) {
/* pull from start of list */
				i = 0;
				dlist_fornext(rss_dl_urls, dptr) {
					rssurl_ptr = dlist_data(dptr);

					rss_url = NULL;

					if (i == opt)
						break;

					printf("str_url=%s, rssyrk->%s\n",
					       str_url, rssurl_ptr->url);

/* turn this into a misc.c function */
					for (j = 0, k = 0;
					     j < strlen(rssurl_ptr->url); j++) {
						if (rssurl_ptr->url[j] == '/')
							k++;
					}
					if (k < 3)
						continue;

					rss_opt_geturl(opt_2, curl, rssurl_ptr);
					if (rss_url) {
						rssurl_ptr->tinyurl = rss_url;
						i++;
					}

				}

				bz(buf);

				i = 0;
				dlist_fornext(rss_dl_urls, dptr) {
					rssurl_ptr = dlist_data(dptr);

					if (i == opt)
						break;

					if (rssurl_ptr->tinyurl) {

						printf("title=%s\n",
						       rssurl_ptr->title);

						if (str_key_dup) {
							strlcatfmt_buf(buf,
								       "[%s] ",
								       str_key_dup);
						}
						strlcatfmt_buf(buf, "%s (%s)\n",
							       rssurl_ptr->
							       title,
							       rssurl_ptr->
							       tinyurl);

						i++;
					}

				}
			} else if (opt < 0) {
/* pull from end of list */

				opt = opt * (-1);

				i = 0;
				dlist_forprev(dlist_tail(rss_dl_urls), dptr) {
					rssurl_ptr = dlist_data(dptr);

					rss_url = NULL;

					if (i == opt)
						break;

/* turn this into a misc.c function */
					for (j = 0, k = 0;
					     j < strlen(rssurl_ptr->url); j++) {
						if (rssurl_ptr->url[j] == '/')
							k++;
					}
					if (k < 3)
						continue;

					rss_opt_geturl(opt_2, curl, rssurl_ptr);
					if (rss_url) {
						rssurl_ptr->tinyurl = rss_url;
						i++;
					}

				}

				bz(buf);
				i = 0;
				dlist_forprev(dlist_tail(rss_dl_urls), dptr) {

					rssurl_ptr = dlist_data(dptr);

					if (i == opt)
						break;

					if (rssurl_ptr->tinyurl) {
						if (str_key_dup) {
							strlcatfmt_buf(buf,
								       "[%s] ",
								       str_key_dup);
						}
						strlcatfmt_buf(buf, "%s (%s)\n",
							       rssurl_ptr->
							       title,
							       rssurl_ptr->
							       tinyurl);

						i++;
					}

				}

			}

			if (_sNULL(buf) != NULL)
				str = strdup(buf);

		}

 cleanup:
		curl_easy_cleanup(curl);
	}

	if (str_url_dup)
		free(str_url_dup);
	if (str_key_dup)
		free(str_key_dup);

	return str;
}

size_t
rss_curl_write_tinyurls_adarq(void *buf, size_t size, size_t nmemb, void *udata)
{

	char *url;

	rss_url = NULL;

	if ((url = strstr(buf, "Your lil&#180; URL is: <a href=\""))) {
		url = url + strlen("Your lil&#180; URL is: <a href=\"");
		url = strtok(url, "\"");
		rss_url = strdup(url);
	}

	return size * nmemb;
}

size_t
rss_curl_write_tinyurls_tinyurl(void *buf, size_t size, size_t nmemb,
				void *udata)
{

	char *url;

	rss_url = NULL;
	if ((url = strstr(buf, "http://"))) {
		rss_url = strdup(url);
		_strstrip_chars(rss_url, "\r\n ");
	}

	return size * nmemb;
}

size_t rss_curl_write_rssurls(void *buf, size_t size, size_t nmemb, void *udata)
{
	int n;

/*
printf("man wtf: %s\n", buf);
*/

	n = write(fileno(rss_fp), buf, size * nmemb);
	if (n <= 0) {
		perror("rss_curl_write_rssurls() ");
	}

	return size * nmemb;
}

void rss_url_free(void *data)
{
	rss_url_t *rssurl;

	if (!data)
		return;
	rssurl = (rss_url_t *) data;
	if (rssurl->title)
		free(rssurl->title);
	if (rssurl->url)
		free(rssurl->url);
	if (rssurl->tinyurl)
		free(rssurl->tinyurl);

	return;
}

void processNode(xmlTextReaderPtr reader)
{
	/* handling of a node in the tree */
	rss_url_t *rssurl;

	xmlChar *name, *value;
	int type;
	int ret;

	char *title, *url;

	name = xmlTextReaderName(reader);
	type = xmlTextReaderNodeType(reader);

	if (name == NULL)
		return;

	value = xmlTextReaderValue(reader);

	if (type == 1 && !strcasecmp((char *)name, "title")) {
/* start element for title */

		while (1) {
			ret = xmlTextReaderRead(reader);
			if (ret != 1)
				break;
			type = xmlTextReaderNodeType(reader);
			name = xmlTextReaderName(reader);
			if (type == 3 && strstr((char *)name, "text")) {
				value = xmlTextReaderValue(reader);
				title = (char *)value;

				rssurl =
				    (rss_url_t *) calloc(1, sizeof(rss_url_t));

				while (1) {
					ret = xmlTextReaderRead(reader);

					if (ret != 1)
						break;
					type = xmlTextReaderNodeType(reader);
					name = xmlTextReaderName(reader);
					if (type == 1
					    && !strcasecmp((char *)name,
							   "link")) {
/* start element for title */
						while (1) {
							ret =
							    xmlTextReaderRead
							    (reader);
							if (ret != 1)
								break;
							type =
							    xmlTextReaderNodeType
							    (reader);
							name =
							    xmlTextReaderName
							    (reader);
							if (type == 3
							    && strstr((char *)
								      name,
								      "text")) {
								value =
								    xmlTextReaderValue
								    (reader);
								url = (char *)
								    value;
								rssurl->title =
								    strdup
								    (title);
								rssurl->url =
								    strdup(url);
								dlist_Dinsert_after
								    (&rss_dl_urls,
								     rssurl);

								break;
							}
						}
						break;

					}
				}
				break;
			}

		}

	}

	xmlFree(name);

}

int rss_parse_xml(char *filename)
{
	xmlTextReaderPtr reader;
	int ret;

	reader = xmlNewTextReaderFilename(filename);
	if (reader != NULL) {
		ret = xmlTextReaderRead(reader);
		while (ret == 1) {
			processNode(reader);
			ret = xmlTextReaderRead(reader);
		}
		xmlFreeTextReader(reader);
		if (ret != 0) {
			debug(NULL, "rss_parse_xml: [%s] : failed to parse\n",
			      filename);
			return -1;
		}
	} else {
		debug(NULL, "rss_parse_xml: Unable to open [%s]\n", filename);
		return -1;

	}
	return 0;
}

int rss_opt_geturl(int opt, CURL * curl, rss_url_t * rssurl_ptr)
{

	debug(NULL, "rss_opt_geturl: Entered\n");

	if (!curl || !rssurl_ptr)
		return 0;

	if (opt & RSS_OPT_METHOD_TINYURL) {
		return rss_opt_geturl_tinyurl(curl, rssurl_ptr);
	} else if (opt & RSS_OPT_METHOD_ADARQ) {
		return rss_opt_geturl_adarq(curl, rssurl_ptr);
	}

	return 0;
}

int rss_opt_geturl_adarq(CURL * curl, rss_url_t * rssurl_ptr)
{
	int res;
	char buf[MAX_BUF_SZ];

	if (!curl || !rssurl_ptr)
		return 0;

	bz(buf);

	snprintf_buf(buf, "longurl=%s", rssurl_ptr->url);

	curl_easy_setopt(curl, CURLOPT_URL, "http://adarq.org/t/");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
			 rss_curl_write_tinyurls_adarq);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

	res = curl_easy_perform(curl);

	return 1;
}

int rss_opt_geturl_tinyurl(CURL * curl, rss_url_t * rssurl_ptr)
{
	int res;
	char buf[MAX_BUF_SZ];

	if (!curl || !rssurl_ptr)
		return 0;

	bz(buf);

	snprintf_buf(buf, "url=%s", rssurl_ptr->url);

	curl_easy_setopt(curl, CURLOPT_URL,
			 "http://tinyurl.com/api-create.php?");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
			 rss_curl_write_tinyurls_tinyurl);

	res = curl_easy_perform(curl);

	return 1;
}

void rss_update(bot_t * bot, dlist_t ** dl, int opt)
{
	DB *db = NULL;
	dlist_t *dptr, *dptr_tmp;
	rss_url_t *rssurl_ptr;
	int n = 0;
	char *db_name = NULL, *channel_ptr = NULL;

	debug(NULL, "rss_update: Entered\n");

	if (!dl || !bot)
		return;
	if (!(*dl))
		return;

	channel_ptr = bot->txt_to;
	_strstrip_chars(channel_ptr, "/\\ \"'`");
	db_name =
	    str_unite("%s/mods/mod_rss_files/%s.%s.rssdb", gi->confdir,
		      bot->tag, channel_ptr);
	printf(".. derp %s\n", db_name);
	db = xdb_open(db_name);
	if (!db)
		return;

	dlist_fornext_retarded(*dl, dptr, dptr_tmp) {
		if (!dptr)
			goto cleanup;

		rssurl_ptr = (rss_url_t *) dlist_data(dptr);
		if (!rssurl_ptr)
			continue;

		n = xdb_write(db, rssurl_ptr->title, rssurl_ptr->url);
//n = xdb_write(db, "hello", "Hi");
		printf(".. derp %i, [%s] [%s]\n", n, rssurl_ptr->title,
		       rssurl_ptr->url);

		if (opt & RSS_OPT_UPDATE) {
			if (n < 0) {
				dlist_remove_and_free(dl, &dptr, rss_url_free);
			}
		}

	}

 cleanup:
	if (db_name)
		free(db_name);

	if (db)
		xdb_fini(db);

	return;
}

char *rss_ret_urldb_str(void)
{
	return str_unite("%s/mods/mod_rss_files/index.rssdb", gi->confdir);
}

char *rss_ret_url_from_key(char *key)
{
	DB *db = NULL;
	xdb_pair_t *pair = NULL;
	char *db_name = NULL;
	char *str = NULL;

	_strstrip_chars(key, " \r\n\t");

	db_name = rss_ret_urldb_str();
	if (!db_name)
		return NULL;
	db = xdb_open(db_name);
	if (!db)
		goto cleanup;

	pair = xdb_get(db, key);
	if (pair) {
		str = strdup(pair->value);
	}

 cleanup:
	if (db_name)
		free(db_name);

	if (pair)
		xdb_pair_destroy(pair);

	return str;
}
