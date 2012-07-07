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
#include "mod_intel.h"

void __intel_init__(void)
{
	strlcpy_buf(mod_intel_info.name, "mod_intel");
	strlcpy_buf(mod_intel_info.trigger, "^intel");

	mod_intel_info.init = intel_init;
	mod_intel_info.fini = intel_fini;
	mod_intel_info.help = intel_help;
	mod_intel_info.run = intel_run;

	mod_intel_info.output = NULL;
	mod_intel_info.input = NULL;

	debug(NULL, "__intel_init__: Loaded mod_intel\n");

	return;
}

bot_t *intel_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "intel_init: Entered\n");
	return NULL;
}

bot_t *intel_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "intel_fini: Entered\n");
	return NULL;
}

bot_t *intel_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "intel_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^intel({true,false,self,strlen,strcmp,bzero,memset,mul,imul,sub,add},<optional args>) ...";

	return NULL;
}

bot_t *intel_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;
	char *opt_val = NULL;

	debug(bot, "intel_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "intel_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "true")) {
		opt = MOD_INTEL_TRUE;
	} else if (!strncasecmp_len(dl_options_ptr, "false")) {
		opt = MOD_INTEL_FALSE;
	} else if (!strncasecmp_len(dl_options_ptr, "self")) {
		opt = MOD_INTEL_SELF;
	} else if (!strncasecmp_len(dl_options_ptr, "strlen")) {
		opt = MOD_INTEL_STRLEN;
	} else if (!strncasecmp_len(dl_options_ptr, "strcmp")) {
		opt = MOD_INTEL_STRCMP;
	} else if (!strncasecmp_len(dl_options_ptr, "toupper")) {
		opt = MOD_INTEL_TOUPPER;
	} else if (!strncasecmp_len(dl_options_ptr, "bzero")) {
		opt = MOD_INTEL_BZERO;
	} else if (!strncasecmp_len(dl_options_ptr, "memset")) {
		opt = MOD_INTEL_MEMSET;
	} else if (!strncasecmp_len(dl_options_ptr, "mul")) {
		opt = MOD_INTEL_MUL;
	} else if (!strncasecmp_len(dl_options_ptr, "imul")) {
		opt = MOD_INTEL_IMUL;
	} else if (!strncasecmp_len(dl_options_ptr, "sub")) {
		opt = MOD_INTEL_SUB;
	} else if (!strncasecmp_len(dl_options_ptr, "add")) {
		opt = MOD_INTEL_ADD;
	}

	opt_val = strchr(dl_options_ptr, ',');
	if (opt_val)
		opt_val++;

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = intel_change_string(bot, l_str_ptr, opt, opt_val);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *intel_change_string(bot_t * bot, char *string, int opt, char *opt_val)
{
	char *res_str = NULL;
	int res_int = 0, res_int_set = 0;
	unsigned int res_uint = 0, res_uint_set = 0;
	char *sep_ptr;
	int opt_val_int = 0;

	if (!bot)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (!string)
		return NULL;

	switch (opt) {
	case MOD_INTEL_TRUE:
		{
			res_int = intel_true();
			res_int_set = 1;
			break;
		}
	case MOD_INTEL_FALSE:
		{
			res_int = intel_false();
			res_int_set = 1;
			break;
		}
	case MOD_INTEL_SELF:
		{
			res_str = (void *)intel_self((void *)string);
			break;
		}
	case MOD_INTEL_STRLEN:
		{
			res_int = intel_strlen(string);
			res_int_set = 1;
			break;
		}
	case MOD_INTEL_STRCMP:
		{
			res_int = intel_strcmp(string, opt_val);
			res_int_set = 1;

			printf("string=%s, opt_val=%s\n", string, opt_val);

			break;
		}
	case MOD_INTEL_BZERO:
		{
			if (!opt_val)
				break;
			opt_val_int = atoi(opt_val);
			if (opt_val_int <= 0)
				break;
			res_int =
			    intel_bzero(bot->txt_data_out,
					opt_val_int >
					strlen(bot->txt_data_out) ? strlen(bot->
									   txt_data_out)
					: opt_val_int);
			return NULL;
			break;
		}
	case MOD_INTEL_MEMSET:
		{
			if (!opt_val)
				break;
			opt_val_int = *opt_val;
			printf("opt_val=%c\n", opt_val_int);
			res_str =
			    intel_memset(string, opt_val_int, strlen(string));
			printf("res_str=%p, %s\n", res_str, res_str);
			break;
		}
	case MOD_INTEL_MUL:
		{
			char *sa, *sb;
			unsigned int a, b;

			if (!sNULL(opt_val))
				return NULL;
			sa = strtok(opt_val, " ");
			if (!sNULL(sa))
				return NULL;
			sb = strtok(NULL, "");
			if (!sNULL(sb))
				return NULL;

			a = atoi(sa);
			b = atoi(sb);

			if (opt == MOD_INTEL_MUL)
				res_uint = (unsigned int)intel_mul(a, b);

			res_uint_set = 1;
			break;
		}
	case MOD_INTEL_IMUL:
	case MOD_INTEL_SUB:
	case MOD_INTEL_ADD:
		{
			char *sa, *sb;
			int a, b;

			if (!sNULL(opt_val))
				return NULL;
			sa = strtok(opt_val, " ");
			if (!sNULL(sa))
				return NULL;
			sb = strtok(NULL, "");
			if (!sNULL(sb))
				return NULL;

			a = atoi(sa);
			b = atoi(sb);

			switch (opt) {
			case MOD_INTEL_IMUL:
				res_int = intel_imul(a, b);
				break;
			case MOD_INTEL_SUB:
				res_int = intel_sub(a, b);
				break;
			case MOD_INTEL_ADD:
				res_int = intel_add(a, b);
				break;
			default:
				break;
			}

			printf("a=%i, b=%i, res_int=%i\n", a, b, res_int);
			res_int_set = 1;
			break;
		}
	default:
		break;
	}

	if (res_str) {
		res_str = strdup(res_str);
	} else if (res_int_set) {
		res_str = str_unite("%i", res_int);
	} else if (res_uint_set) {
		res_str = str_unite("%u", res_uint);
	}

	return res_str;
}
