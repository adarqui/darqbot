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
#include "mod_to.h"

void to_parse(bot_t * bot)
{
	char *str, buf[1024], *buf_ptr;
	int add = 0;

	debug(bot, "to_parse: Entered\n");

	if (to_info.initialized)
		return;

	if (to_info.fp_conf) {
		fclose(to_info.fp_conf);
		to_info.fp_conf = NULL;
	}

	if (to_info.fp_pms) {
		fclose(to_info.fp_pms);
		to_info.fp_pms = NULL;
	}

	if (to_info.conf) {
		free(to_info.conf);
		to_info.conf = NULL;
	}

	if (to_info.log_pms) {
		free(to_info.log_pms);
		to_info.log_pms = NULL;
	}

	if (to_info.dl_controlchans)
		dlist_fini(&to_info.dl_controlchans, free);

	if (to_info.dl_ignore)
		dlist_fini(&to_info.dl_ignore, free);

	to_info.allowpms = 0;

	str = str_unite("%s/mods/mod_to_files/conf", gi->confdir);

	debug(bot, "to_parse: to conf located at: %s\n", str);

	to_info.conf = str;

	to_info.fp_conf = fopen(str, "r");
	if (!to_info.fp_conf)
		return;

	debug(bot, "to_parse: conf FOUND!\n");

	str = str_unite("%s/mods/mod_to_files/pms.log", gi->confdir);

	debug(bot, "to_parse: to conf located at: %s\n", str);

	to_info.log_pms = str;

	to_info.fp_pms = fopen(to_info.log_pms, "a+");
	if (!to_info.fp_pms) {
		puts("dd");
		return;
	}

	puts("dd2");

	while (1) {
		memset(buf, 0, sizeof(buf));
		if (fgets(buf, sizeof(buf) - 1, to_info.fp_conf) == NULL)
			break;

		strstrip_nl(buf);

		if (buf[0] == '+')
			add = 1;
		else if (buf[0] == '-')
			add = 0;
		else
			continue;

		buf_ptr = (buf + 1);

		if (!strncasecmp
		    (buf_ptr, "controlchans", strlen("controlchans"))) {
			puts("dd4");
			to_parse_controlchans(bot,
					      &buf_ptr[strlen("controlchans") +
						       1], add);
		} else if (!strncasecmp(buf_ptr, "ignore", strlen("ignore"))) {
			to_parse_ignore(bot, &buf_ptr[strlen("ignore") + 1],
					add);
		} else
		    if (!strncasecmp(buf_ptr, "allowpms", strlen("allowpms"))) {
			to_info.allowpms = 1;
		}

	}

	to_info.initialized = 1;

	puts("dd3");

	return;
}

int to_parse_controlchans(bot_t * bot, char *str, int add)
{

	char *ptr_1, *ptr_2, *ptr_3, *chan;

	if (!str)
		return -1;

	puts("ee1");

	if (add == 0) {		/*remove entry; */
		return 0;
	}

	puts("ee2");

	ptr_1 = str;

	while (1) {
		ptr_2 = strtok(ptr_1, ",");
		if (!ptr_2)
			return 0;

		ptr_3 = eat_whitespace(ptr_2);

		puts("ee3");

		chan = strdup(ptr_3);

		debug(bot, "to_parse_controlchans: chan=%s\n", chan);

		dlist_Dinsert_after(&to_info.dl_controlchans, chan);

		printf("to: added %s\n", chan);

		ptr_1 = NULL;
	}

	puts("ee4");

	return 0;
}

int to_parse_ignore(bot_t * bot, char *str, int add)
{

	char *ptr_1, *ptr_2, *ptr_3, *host;

	if (!str)
		return -1;

	if (add == 0) {		/*remove entry; */
		return 0;
	}

	ptr_1 = str;

	while (1) {
		ptr_2 = strtok(ptr_1, ",");
		if (!ptr_2)
			return 0;

		ptr_3 = eat_whitespace(ptr_2);

		host = strdup(ptr_3);

		debug(bot, "to_parse_ignore: host=%s\n", host);

		dlist_Dinsert_after(&to_info.dl_ignore, host);

		ptr_1 = NULL;
	}

	return 0;
}

void to_print(bot_t * bot)
{
	dlist_t *dptr;
	char *val;

	debug(bot,
	      "to_print_info: conf=%s, pms=%s, allowpms=%i\n", to_info.conf,
	      to_info.log_pms, to_info.allowpms);

/* controlchans */
	debug(bot, "to_print_info: controlchans:\n");
	dlist_fornext(to_info.dl_controlchans, dptr) {
		val = dlist_data(dptr);
		debug(bot, "\t%s\n", val);
	}

/* ignore */
	debug(bot, "to_print_info: ignore:\n");
	dlist_fornext(to_info.dl_ignore, dptr) {
		val = dlist_data(dptr);
		debug(bot, "\t%s\n", val);
	}

	return;
}

void __to_init__(void) __attribute__ ((constructor));

void __to_init__(void)
{

/*
  mod_to_info.name = "mod_to";
  mod_to_info.trigger = strdup ("^to");
*/
	strlcpy_buf(mod_to_info.name, "mod_to");
	strlcpy_buf(mod_to_info.trigger, "^to");

	mod_to_info.init = to_init;
	mod_to_info.fini = to_fini;
	mod_to_info.help = to_help;
	mod_to_info.run = to_run;

	mod_to_info.output = NULL;
	mod_to_info.input = NULL;

	debug(NULL, "__to_init__: Loaded mod_to\n");

	return;
}

bot_t *to_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "to_init: Entered\n");

	to_parse(bot);

	return NULL;
}

bot_t *to_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "to_fini: Entered\n");

	if (to_info.fp_conf) {
		fclose(to_info.fp_conf);
		to_info.fp_conf = NULL;
	}

	if (to_info.fp_pms) {
		fclose(to_info.fp_pms);
		to_info.fp_pms = NULL;
	}

	if (to_info.conf) {
		free(to_info.conf);
		to_info.conf = NULL;
	}

	if (to_info.log_pms) {
		free(to_info.log_pms);
		to_info.log_pms = NULL;
	}

	if (to_info.dl_controlchans)
		dlist_fini(&to_info.dl_controlchans, free);

	if (to_info.dl_ignore)
		dlist_fini(&to_info.dl_ignore, free);

	to_info.allowpms = 0;

	memset(&to_info, 0, sizeof(to_info));

	return NULL;
}

bot_t *to_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "to_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^to nick|#channel";

	return NULL;
}

bot_t *to_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr;
	char *val, *str_ptr = NULL;
	int found;

	char *tag;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "to_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "to_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	to_print(bot);

	tag = NULL;

/* turn tags into a dlist eventually, and the fd_new should become a dlist.. for sending to multiple channels/servers */
	MOD_OPTIONS_TOP_HALF;
	tag = dl_options_ptr;
	MOD_OPTIONS_BOTTOM_HALF;

	if (tag)
		debug(bot, "to_run: tag=%s\n", tag);

	if (!strlen(eat_whitespace(dl_module_arg_after_options))) {
		return NULL;
	}

	puts("1");

	dlist_fornext(to_info.dl_ignore, dptr) {
		val = dlist_data(dptr);
		if (xstrcasestr2(bot->txt_host, val)) {
			memset(bot->txt_to, 0, sizeof(bot->txt_to));
			return NULL;
		}
	}
	puts("2");

/*
URGENT FIXME : the conf isnt being parsed because module_load is not passing a bot pointer.. */
	found = 0;
	dlist_fornext(to_info.dl_controlchans, dptr) {
		val = dlist_data(dptr);
		if (xstrcasestr2(bot->txt_to, val)) {
			found = 1;
			break;
		}
	}
	puts("3");

	if (!found) {
		memset(bot->txt_to, 0, sizeof(bot->txt_to));
		return NULL;
	}

	puts("4");

/* turn tags into a dlist eventually, and the fd_new should become a dlist.. for sending to multiple channels/servers */
	if (tag) {
		bot_t *bot_tmp;
		dlist_fornext(gi->bots, dptr) {
			bot_tmp = (bot_t *) dlist_data(dptr);
			if (!bot_tmp)
				continue;

			printf("grr %s\n", bot_tmp->tag);

/*
XXX
	if (!strcasecmp (bot_tmp->tag, tag))
	  {
	    debug (bot, "to_run: %s found!\n", tag);
	    bot->new_bot = bot_tmp;
	    break;
	  }
*/

		}

	}

	str_ptr = eat_whitespace(dl_module_arg_after_options);

	if (str_ptr[0] != '\0') {
		memset(bot->txt_to, 0, sizeof(bot->txt_to));
		strlcat_bot(bot->txt_to, str_ptr);

		if (bot->txt_to[0] != '#' && to_info.fp_pms) {
/* PROBLEM, wont log shit AFTER ^to */
			fprintf(to_info.fp_pms,
				"PM from nick=%s@%s to=%s ::: data[%s]\n",
				bot->txt_nick, bot->txt_host, bot->txt_to,
				bot->txt_data_out);
			fflush(to_info.fp_pms);
		}

	}

	return bot;
}
