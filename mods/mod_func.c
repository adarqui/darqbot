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
#include "mod_func.h"

void __func_init__(void)
{
	strlcpy_buf(mod_func_info.name, "mod_func");
	strlcpy_buf(mod_func_info.trigger, "^func");

	mod_func_info.init = func_init;
	mod_func_info.fini = func_fini;
	mod_func_info.help = func_help;
	mod_func_info.run = func_run;

	mod_func_info.output = NULL;
	mod_func_info.input = NULL;

	debug(NULL, "__func_init__: Loaded mod_func\n");

	return;
}

bot_t *func_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "func_init: Entered\n");

	func_switch("stdc");

	return NULL;
}

bot_t *func_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "func_fini: Entered\n");

/* important since this module will be used to hook various functions */
fns_load_defaults();

	return NULL;
}

bot_t *func_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "func_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^func({switch,true,false,self,strlen,strcmp,strchr,strrchr,strcpy,strncpy,bzero,memset,eflags,mul,imul,sub,add,div,idiv,mod,imod,neg,and,or,xor,not,jmp,sar,shr,sal,shl,ror,rol,bswap,isupper,islower,toupper,tolower},<optional args>) ... || ^func(switch,{c,c2,stdc,intel,intel2})";

	return NULL;
}

bot_t *func_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;
	char *opt_val = NULL;

	debug(bot, "func_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "func_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "switch")) {
		opt = MOD_FUNC_SWITCH;
	} else if (!strncasecmp_len(dl_options_ptr, "true")) {
		opt = MOD_FUNC_TRUE;
	} else if (!strncasecmp_len(dl_options_ptr, "false")) {
		opt = MOD_FUNC_FALSE;
	} else if (!strncasecmp_len(dl_options_ptr, "self")) {
		opt = MOD_FUNC_SELF;
	} else if (!strncasecmp_len(dl_options_ptr, "strlen")) {
		opt = MOD_FUNC_STRLEN;
	} else if (!strncasecmp_len(dl_options_ptr, "strcmp")) {
		opt = MOD_FUNC_STRCMP;
	} else if (!strncasecmp_len(dl_options_ptr, "strncmp")) {
		opt = MOD_FUNC_STRNCMP;
	} else if (!strncasecmp_len(dl_options_ptr, "strchr")) {
		opt = MOD_FUNC_STRCHR;
	} else if (!strncasecmp_len(dl_options_ptr, "strrchr")) {
		opt = MOD_FUNC_STRRCHR;
	} else if (!strncasecmp_len(dl_options_ptr, "strcpy")) {
		opt = MOD_FUNC_STRCPY;
	} else if (!strncasecmp_len(dl_options_ptr, "strncpy")) {
		opt = MOD_FUNC_STRNCPY;
	} else if (!strncasecmp_len(dl_options_ptr, "bzero")) {
		opt = MOD_FUNC_BZERO;
	} else if (!strncasecmp_len(dl_options_ptr, "memset")) {
		opt = MOD_FUNC_MEMSET;
	} else if (!strncasecmp_len(dl_options_ptr, "eflags")) {
		opt = MOD_FUNC_EFLAGS;
	} else if (!strncasecmp_len(dl_options_ptr, "mul")) {
		opt = MOD_FUNC_MUL;
	} else if (!strncasecmp_len(dl_options_ptr, "imul")) {
		opt = MOD_FUNC_IMUL;
	} else if (!strncasecmp_len(dl_options_ptr, "sub")) {
		opt = MOD_FUNC_SUB;
	} else if (!strncasecmp_len(dl_options_ptr, "add")) {
		opt = MOD_FUNC_ADD;
	} else if (!strncasecmp_len(dl_options_ptr, "div")) {
		opt = MOD_FUNC_DIV;
	} else if (!strncasecmp_len(dl_options_ptr, "idiv")) {
		opt = MOD_FUNC_IDIV;
	} else if (!strncasecmp_len(dl_options_ptr, "mod")) {
		opt = MOD_FUNC_MOD;
	} else if (!strncasecmp_len(dl_options_ptr, "imod")) {
		opt = MOD_FUNC_IMOD;
	} else if (!strncasecmp_len(dl_options_ptr, "neg")) {
		opt = MOD_FUNC_NEG;
	} else if (!strncasecmp_len(dl_options_ptr, "and")) {
		opt = MOD_FUNC_AND;
	} else if (!strncasecmp_len(dl_options_ptr, "or")) {
		opt = MOD_FUNC_OR;
	} else if (!strncasecmp_len(dl_options_ptr, "xor")) {
		opt = MOD_FUNC_XOR;
	} else if (!strncasecmp_len(dl_options_ptr, "not")) {
		opt = MOD_FUNC_NOT;
	} else if (!strncasecmp_len(dl_options_ptr, "sar")) {
		opt = MOD_FUNC_SAR;
	} else if (!strncasecmp_len(dl_options_ptr, "shr")) {
		opt = MOD_FUNC_SHR;
	} else if (!strncasecmp_len(dl_options_ptr, "sal")) {
		opt = MOD_FUNC_SAL;
	} else if (!strncasecmp_len(dl_options_ptr, "shl")) {
		opt = MOD_FUNC_SHL;
	} else if (!strncasecmp_len(dl_options_ptr, "ror")) {
		opt = MOD_FUNC_ROR;
	} else if (!strncasecmp_len(dl_options_ptr, "rol")) {
		opt = MOD_FUNC_ROL;
	} else if (!strncasecmp_len(dl_options_ptr, "jmp")) {
		opt = MOD_FUNC_JMP;
	} else if (!strncasecmp_len(dl_options_ptr, "bswap")) {
		opt = MOD_FUNC_BSWAP;
	}
else if(!strncasecmp_len(dl_options_ptr, "bsf")) {
opt = MOD_FUNC_BSF;
}
else if(!strncasecmp_len(dl_options_ptr, "bsr")) {
opt = MOD_FUNC_BSR;
}
else if(!strncasecmp_len(dl_options_ptr, "btc")) {
opt = MOD_FUNC_BTC;
}
else if(!strncasecmp_len(dl_options_ptr, "btr")) {
opt = MOD_FUNC_BTR;
}
else if(!strncasecmp_len(dl_options_ptr, "bts")) {
opt = MOD_FUNC_BTS;
}
else if(!strncasecmp_len(dl_options_ptr, "bt")) {
opt = MOD_FUNC_BT;
}

 else if (!strncasecmp_len(dl_options_ptr, "isupper")) {
		opt = MOD_FUNC_ISUPPER;
	} else if (!strncasecmp_len(dl_options_ptr, "islower")) {
		opt = MOD_FUNC_ISLOWER;
	} else if (!strncasecmp_len(dl_options_ptr, "toupper")) {
		opt = MOD_FUNC_TOUPPER;
	} else if (!strncasecmp_len(dl_options_ptr, "tolower")) {
		opt = MOD_FUNC_TOLOWER;
	} else if (!strncasecmp_len(dl_options_ptr, "isdigit")) {
		opt = MOD_FUNC_ISDIGIT;
	}

	else if (!strncasecmp_len(dl_options_ptr, "isbinary")) {
		opt = MOD_FUNC_ISBINARY;
	} else if (!strncasecmp_len(dl_options_ptr, "isalpha")) {
		opt = MOD_FUNC_ISALPHA;
	} else if (!strncasecmp_len(dl_options_ptr, "isalnum")) {
		opt = MOD_FUNC_ISALNUM;
	} else if (!strncasecmp_len(dl_options_ptr, "isascii")) {
		opt = MOD_FUNC_ISASCII;
	} else if (!strncasecmp_len(dl_options_ptr, "ispunct")) {
		opt = MOD_FUNC_ISPUNCT;
	} else if (!strncasecmp_len(dl_options_ptr, "isblank")) {
		opt = MOD_FUNC_ISBLANK;
	} else if (!strncasecmp_len(dl_options_ptr, "isspace")) {
		opt = MOD_FUNC_ISSPACE;
	} else if (!strncasecmp_len(dl_options_ptr, "isprint")) {
		opt = MOD_FUNC_ISPRINT;
	} else if (!strncasecmp_len(dl_options_ptr, "isxdigit")) {
		opt = MOD_FUNC_ISXDIGIT;
	} else if (!strncasecmp_len(dl_options_ptr, "isgraph")) {
		opt = MOD_FUNC_ISGRAPH;
	}

else if(!strncasecmp_len(dl_options_ptr, "snull")) {
opt = MOD_FUNC_SNULL;
}
else if(!strncasecmp_len(dl_options_ptr, "strstrip_nl")) {
opt = MOD_FUNC_STRSTRIP_NL;
}
else if(!strncasecmp_len(dl_options_ptr, "strstrip_chars")) {
opt = MOD_FUNC_STRSTRIP_CHARS;
}
else if(!strncasecmp_len(dl_options_ptr, "eat_whitespace")) {
opt = MOD_FUNC_EAT_WHITESPACE;
}


 else if (!strncasecmp_len(dl_options_ptr, "exit")) {
		opt = MOD_FUNC_EXIT;
	}

	if (!opt)
		return NULL;

	opt_val = strchr(dl_options_ptr, ',');
	if (opt_val)
		opt_val++;

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = func_change_string(bot, l_str_ptr, opt, opt_val);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *func_change_string(bot_t * bot, char *string, int opt, char *opt_val)
{
	char *res_str = NULL;
	int res_int = 0, res_int_set = 0;
	unsigned int res_uint = 0, res_uint_set = 0;
	char *sep_ptr;
	int opt_val_int = 0;

	char *txt_data_out_ptr = NULL;

	if (!bot)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (!string)
		return NULL;

	txt_data_out_ptr = str_find_sep(bot->txt_data_out);
	if (!txt_data_out_ptr)
		txt_data_out_ptr = bot->txt_data_out;

	switch (opt) {
	case MOD_FUNC_SWITCH:{

			func_switch(opt_val);

			return NULL;
		}
	case MOD_FUNC_TRUE:
		{
			res_int = func_true();
			res_int_set = 1;
			break;
		}
	case MOD_FUNC_FALSE:
		{
			res_int = func_false();
			res_int_set = 1;
			break;
		}
	case MOD_FUNC_SELF:
		{
			res_str = (void *)func_self((void *)string);
			break;
		}
	case MOD_FUNC_STRLEN:
		{
			res_int = func_strlen(string);
			res_int_set = 1;
			break;
		}
	case MOD_FUNC_STRCMP:
		{
			res_int = func_strcmp(string, opt_val);
			res_int_set = 1;

			printf("string=%s, opt_val=%s\n", string, opt_val);

			break;
		}
	case MOD_FUNC_STRCPY:
		{
			func_strcpy(txt_data_out_ptr, opt_val);
			break;
		}
	case MOD_FUNC_BZERO:
		{
			if (!opt_val)
				break;
			opt_val_int = atoi(opt_val);
			if (opt_val_int <= 0)
				break;
			func_bzero(bot->txt_data_out,
				   opt_val_int >
				   strlen(bot->txt_data_out) ?
				   strlen(bot->txt_data_out) : opt_val_int);
			return NULL;
			break;
		}
case MOD_FUNC_SNULL: {

printf("XXX: %s\n", string);
res_str = func_sNULL(string);
if(!res_str) {
res_str = "NULL";
}

break;
}
case MOD_FUNC_STRSTRIP_NL:{ 
res_int = func_strstrip_nl(string);
//res_int_set = 1;
debug(NULL, "XXX: func_strstrip_nl: res_int=%i\n", res_int);
res_str = string;
break;
}
case MOD_FUNC_STRSTRIP_CHARS: {

if(_sNULL(opt_val)) {
res_int = func_strstrip_chars(string, opt_val);
res_str = string;
debug(NULL, "XXX: func_strstrip_chars: res_int=%i\n", res_int);
}

break;
}
case MOD_FUNC_EAT_WHITESPACE:{ 
res_str = func_eat_whitespace(string);
break;
}

	case MOD_FUNC_EFLAGS:
		{
			res_str = func_eflags();
			break;
		}
	case MOD_FUNC_JMP:{

			break;
		}

	case MOD_FUNC_MUL:
	case MOD_FUNC_DIV:
	case MOD_FUNC_MOD:
	case MOD_FUNC_ISUPPER:
	case MOD_FUNC_ISLOWER:
	case MOD_FUNC_TOUPPER:
	case MOD_FUNC_TOLOWER:
	case MOD_FUNC_ISDIGIT:
	case MOD_FUNC_ISBINARY:
	case MOD_FUNC_ISALPHA:
	case MOD_FUNC_ISALNUM:
	case MOD_FUNC_ISASCII:
	case MOD_FUNC_ISPUNCT:
	case MOD_FUNC_ISBLANK:
	case MOD_FUNC_ISSPACE:
	case MOD_FUNC_ISPRINT:
	case MOD_FUNC_ISXDIGIT:
	case MOD_FUNC_ISGRAPH:
	case MOD_FUNC_STRCHR:
	case MOD_FUNC_STRRCHR:
	case MOD_FUNC_STRNCMP:
	case MOD_FUNC_STRNCPY:
	case MOD_FUNC_MEMSET:
		{
			char *sa, *sb;
			unsigned int a, b;

			if (!_sNULL(opt_val))
				return NULL;
			sa = strtok(opt_val, " ");

			if (!_sNULL(sa))
				return NULL;

			if (opt == MOD_FUNC_ISUPPER) {
				res_uint = func_isupper((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISLOWER) {
				res_uint = func_islower((int)*sa);
				res_uint_set = 1;
				break;
			}

			else if (opt == MOD_FUNC_ISDIGIT) {
				res_uint = func_isdigit((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISBINARY) {
				res_uint = func_isbinary((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISALPHA) {
				res_uint = func_isalpha((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISALNUM) {
				res_uint = func_isalnum((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISASCII) {
				res_uint = func_isascii((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISPUNCT) {
				res_uint = func_ispunct((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISBLANK) {
				res_uint = func_isblank((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISSPACE) {
				res_uint = func_isspace((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISPRINT) {
				res_uint = func_isprint((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISXDIGIT) {
				res_uint = func_isxdigit((int)*sa);
				res_uint_set = 1;
				break;
			} else if (opt == MOD_FUNC_ISGRAPH) {
				res_uint = func_isgraph((int)*sa);
				res_uint_set = 1;
				break;
			}

			else if (opt == MOD_FUNC_TOUPPER) {
				res_uint = func_toupper((int)*sa);
				res_str = str_unite("%c", res_uint);
				break;
			} else if (opt == MOD_FUNC_TOLOWER) {
				res_uint = func_tolower((int)*sa);
				res_str = str_unite("%c", res_uint);
				break;
			} else if (opt == MOD_FUNC_STRCHR) {
				res_str = func_strchr(string, (int)*sa);
				break;
			} else if (opt == MOD_FUNC_STRRCHR) {
				res_str = func_strrchr(string, (int)*sa);
				break;
			}

			sb = strtok(NULL, "");
			if (!_sNULL(sb))
				return NULL;

			if (opt == MOD_FUNC_STRNCMP) {
				res_int = func_strncmp(string, sb, atoi(sa));
				res_int_set = 1;
				break;
			} else if (opt == MOD_FUNC_STRNCPY) {
/* XXX CAN BE EXPLOITED */
				func_strncpy(txt_data_out_ptr, sb, atoi(sa));
				break;
			} else if (opt == MOD_FUNC_MEMSET) {
				res_str = func_memset(string, *sb, atoi(sa));
				break;
			}

			a = atoi(sa);
			b = atoi(sb);

			switch (opt) {
			case MOD_FUNC_MUL:
				res_uint = (unsigned int)func_mul(a, b);
				break;
			case MOD_FUNC_DIV:
				res_uint = (unsigned int)func_div(a, b);
				break;
			case MOD_FUNC_MOD:
				res_uint = (unsigned int)func_mod(a, b);
				break;
			default:
				break;
			}

			res_uint_set = 1;
			break;
		}
	case MOD_FUNC_IMUL:
	case MOD_FUNC_SUB:
	case MOD_FUNC_ADD:
	case MOD_FUNC_IDIV:
	case MOD_FUNC_IMOD:
	case MOD_FUNC_NEG:
	case MOD_FUNC_AND:
	case MOD_FUNC_OR:
	case MOD_FUNC_XOR:
	case MOD_FUNC_NOT:
	case MOD_FUNC_SAR:
	case MOD_FUNC_SHR:
	case MOD_FUNC_SAL:
	case MOD_FUNC_SHL:
	case MOD_FUNC_ROR:
	case MOD_FUNC_ROL:
	case MOD_FUNC_BSWAP:
case MOD_FUNC_BSF:
case MOD_FUNC_BSR:
case MOD_FUNC_BT:
case MOD_FUNC_BTC:
case MOD_FUNC_BTR:
case MOD_FUNC_BTS:
		{
			char *sa, *sb;
			int a, b;

			if (!_sNULL(opt_val))
				return NULL;
			sa = strtok(opt_val, " ");
			if (!_sNULL(sa))
				return NULL;

			if (opt == MOD_FUNC_NEG) {
				res_int = func_neg(atoi(sa));
				res_int_set = 1;
				break;
			} else if (opt == MOD_FUNC_NOT) {
				res_int = func_not(atoi(sa));
				res_int_set = 1;
				break;
			} else if (opt == MOD_FUNC_BSWAP) {
				res_int = func_bswap(atoi(sa));
				res_int_set = 1;
				break;
			}
else if(opt == MOD_FUNC_BSF) {
res_int = func_bsf(atoi(sa));
res_int_set = 1;
break;
}
else if(opt == MOD_FUNC_BSR) {
res_int = func_bsr(atoi(sa));
res_int_set = 1;
break;
}


			sb = strtok(NULL, "");
			if (!_sNULL(sb))
				return NULL;

			a = atoi(sa);
			b = atoi(sb);

			switch (opt) {
			case MOD_FUNC_IMUL:
				res_int = func_imul(a, b);
				break;
			case MOD_FUNC_SUB:
				res_int = func_sub(a, b);
				break;
			case MOD_FUNC_ADD:
				res_int = func_add(a, b);
				break;
			case MOD_FUNC_IDIV:
				res_int = func_idiv(a, b);
				break;
			case MOD_FUNC_IMOD:
				res_int = func_imod(a, b);
				break;
			case MOD_FUNC_AND:
				res_int = func_and(a, b);
				break;
			case MOD_FUNC_OR:
				res_int = func_or(a, b);
				break;
			case MOD_FUNC_XOR:
				res_int = func_xor(a, b);
				break;
			case MOD_FUNC_SAR:
				res_int = func_sar(a, b);
				break;
			case MOD_FUNC_SHR:
				res_int = func_shr(a, b);
				break;
			case MOD_FUNC_SAL:
				res_int = func_sal(a, b);
				break;
			case MOD_FUNC_SHL:
				res_int = func_shl(a, b);
				break;
			case MOD_FUNC_ROR:
				res_int = func_ror(a, b);
				break;
			case MOD_FUNC_ROL:
				res_int = func_rol(a, b);
				break;
case MOD_FUNC_BT:
res_int = func_bt(a, b);
break;
case MOD_FUNC_BTC:
res_int = func_btc(a, b);
break;
case MOD_FUNC_BTR:
res_int = func_btr(a, b);
break;
case MOD_FUNC_BTS:
res_int = func_bts(a, b);
break;
			default:
				break;
			}

			printf("a=%i, b=%i, res_int=%i\n", a, b, res_int);
			res_int_set = 1;
			break;
		}
	case MOD_FUNC_EXIT:
		{
			func_exit(atoi(opt_val));
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

void func_switch(char *type)
{
	int rand_val = 0;

	if (!_sNULL(type))
		return;

	rand_val = rand();

	if (!strcasecmp(type, "c")) {
		func_true = c_true;
		func_false = c_false;
		func_self = c_self;

		func_strlen = c_strlen;
		func_strcmp = c_strcmp;
		func_strncmp = c_strncmp;
		func_strchr = c_strchr;
		func_strrchr = c_strrchr;
		func_strcpy = c_strcpy;
		func_strncpy = c_strncpy;

		func_bzero = c_bzero;
		func_memset = c_memset;

		func_mul = c_mul;
		func_imul = c_imul;

		func_sub = c_sub;
		func_add = c_add;

		func_div = c_div;
		func_idiv = c_idiv;
		func_mod = c_mod;
		func_imod = c_imod;

		func_neg = c_neg;
		func_eflags = c_eflags;

		func_and = c_and;
		func_or = c_or;
		func_xor = c_xor;
		func_not = c_not;

		func_sar = c_sar;
		func_shr = c_shr;
		func_sal = c_sal;
		func_shl = c_shl;
		func_ror = c_ror;
		func_rol = c_rol;

		func_bswap = c_bswap;
func_bsf = c_bsf;
func_bsr = c_bsr;
func_bt = c_bt;
func_btc = c_btc;
func_btr = c_btr;
func_bts = c_bts;


		func_isupper = c_isupper;
		func_islower = c_islower;
		func_toupper = c_toupper;
		func_tolower = c_tolower;

		func_isdigit = c_isdigit;
		func_isbinary = c_isbinary;
		func_isalpha = c_isalpha;
		func_isalnum = c_isalnum;
		func_isascii = c_isascii;
		func_ispunct = c_ispunct;
		func_isblank = c_isblank;
		func_isspace = c_isspace;
		func_isprint = c_isprint;
		func_isxdigit = c_isxdigit;
		func_isgraph = c_isgraph;
	} else if (!strcasecmp(type, "intel")) {
		func_true = intel_true;
		func_false = intel_false;
		func_self = intel_self;

		rand_val = rand_val % 2;
		if (rand_val >= 1) {
			func_strlen = intel_strlen;
		} else {
			func_strlen = intel_strlen2;
		}

		func_strcmp = intel_strcmp;
		func_strncmp = intel_strncmp;
		func_strchr = intel_strchr;
		func_strrchr = intel_strrchr;
		func_strcpy = intel_strcpy;
		func_strncpy = intel_strncpy;

		func_bzero = intel_bzero;
		func_memset = intel_memset;

		func_mul = intel_mul;
		func_imul = intel_imul;

		func_sub = intel_sub;
		func_add = intel_add;

		func_div = intel_div;
		func_idiv = intel_idiv;
		func_mod = intel_mod;
		func_imod = intel_imod;

		func_neg = intel_neg;
		func_eflags = c_eflags;

		func_and = intel_and;
		func_or = intel_or;
		func_xor = intel_xor;
		func_not = intel_not;

		func_sar = intel_sar;
		func_shr = intel_shr;
		func_sal = intel_sal;
		func_shl = intel_shl;
		func_ror = intel_ror;
		func_rol = intel_rol;

		func_bswap = intel_bswap;
func_bsf = intel_bsf;
func_bsr = intel_bsr;
func_bt = intel_bt;
func_btc = intel_btc;
func_btr = intel_btr;
func_bts = intel_bts;


		func_isupper = intel_isupper;
		func_islower = intel_islower;
		func_toupper = intel_toupper;
		func_tolower = intel_tolower;

		func_isdigit = intel_isdigit;
		func_isbinary = intel_isbinary;
		func_isalpha = intel_isalpha;
		func_isalnum = intel_isalnum;
/*
func_isascii = intel_isascii;
func_ispunct = intel_ispunct;
func_isblank = intel_isblank;
func_isspace = intel_isspace;
func_isprint = intel_isprint;
func_isxdigit = intel_isxdigit;
func_isgraph = intel_isgraph;
*/


		func_exit = intel_linux_exit;
	} else if (!strcasecmp(type, "intel2")) {
		func_true = intel2_true;
		func_false = intel2_false;
		func_self = intel2_self;

		rand_val = rand_val % 2;
		if (rand_val >= 1) {
			func_strlen = intel2_strlen;
		} else {
			func_strlen = intel2_strlen2;
		}

		func_strcmp = intel2_strcmp;
		func_strncmp = intel2_strncmp;
		func_strchr = intel2_strchr;
		func_strrchr = intel2_strrchr;
		func_strcpy = intel2_strcpy;
		func_strncpy = intel2_strncpy;

		func_bzero = intel2_bzero;
		func_memset = intel2_memset;

		func_mul = intel2_mul;
		func_imul = intel2_imul;

		func_sub = intel2_sub;
		func_add = intel2_add;

		func_div = intel2_div;
		func_idiv = intel2_idiv;
		func_mod = intel2_mod;
		func_imod = intel2_imod;

		func_neg = intel2_neg;
		func_eflags = c_eflags;

		func_and = intel2_and;
		func_or = intel2_or;
		func_xor = intel2_xor;
		func_not = intel2_not;

		func_sar = intel2_sar;
		func_shr = intel2_shr;
		func_sal = intel2_sal;
		func_shl = intel2_shl;
		func_ror = intel2_ror;
		func_rol = intel2_rol;

		func_bswap = intel2_bswap;
func_bsf = intel2_bsf;
func_bsr = intel2_bsr;
func_bt = intel2_bt;
func_btc = intel2_btc;
func_btr = intel2_btr;
func_bts = intel2_bts;


		func_isupper = intel2_isupper;
		func_islower = intel2_islower;
		func_toupper = intel2_toupper;
		func_tolower = intel2_tolower;

		func_isdigit = intel2_isdigit;
		func_isbinary = intel2_isbinary;
		func_isalpha = intel2_isalpha;
		func_isalnum = intel2_isalnum;
/*
func_isascii = intel2_isascii;
func_ispunct = intel2_ispunct;
func_isblank = intel2_isblank;
func_isspace = intel2_isspace;
func_isprint = intel2_isprint;
func_isxdigit = intel2_isxdigit;
func_isgraph = intel2_isgraph;
*/

/* misc.c */
func_sNULL = intel2_sNULL;
func_strstrip_nl = intel2_strstrip_nl;
func_strstrip_chars = intel2_strstrip_chars;
func_eat_whitespace = intel2_eat_whitespace;

		func_exit = intel2_linux_exit;

	} else if (!strcasecmp(type, "stdc")) {
		func_true = c_true;
		func_false = c_false;
		func_self = c_self;

		func_strlen = strlen;
		func_strcmp = strcmp;
		func_strncmp = strncmp;
		func_strchr = strchr;
		func_strrchr = strrchr;
		func_strcpy = strcpy;
		func_strncpy = strncpy;

		func_bzero = bzero;
		func_memset = memset;

		func_mul = c_mul;
		func_imul = c_imul;

		func_sub = c_sub;
		func_add = c_add;

		func_div = c_div;
		func_idiv = c_idiv;
		func_mod = c_mod;
		func_imod = c_imod;

		func_neg = c_neg;
		func_eflags = c_eflags;

		func_and = c_and;
		func_or = c_or;
		func_xor = c_xor;
		func_not = c_not;

		func_sar = c_sar;
		func_shr = c_shr;
		func_sal = c_sal;
		func_shl = c_shl;
		func_ror = c_ror;
		func_rol = c_rol;

		func_bswap = c_bswap;
func_bsf = c_bsf;
func_bsr = c_bsr;
func_bt = c_bt;

		func_isupper = isupper;
		func_islower = islower;
		func_toupper = toupper;
		func_tolower = tolower;

		func_isdigit = isdigit;
		func_isbinary = isbinary;
		func_isalpha = isalpha;
		func_isalnum = isalnum;
		func_isascii = isascii;
		func_ispunct = ispunct;
		func_isblank = isblank;
		func_isspace = isspace;
		func_isprint = isprint;
		func_isxdigit = isxdigit;
		func_isgraph = isgraph;

/* misc.c */
func_sNULL = sNULL;
func_strstrip_nl = strstrip_nl;
func_strstrip_chars = strstrip_chars;
func_eat_whitespace = eat_whitespace;

		func_exit = exit;
	} else if (!strcasecmp(type, "c2")) {
		func_add = c2_add;
		func_strlen = c2_strlen;
		func_strcpy = c2_strcpy;
		func_strncpy = c2_strncpy;
		func_memset = c2_memset;
	}

	return;
}
